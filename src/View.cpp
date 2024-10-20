#include "View.h"
#include "systems/Pathfinder.h"
#include <chrono>
#include <thread>
#include <random>
#include <cmath>
#include "systems/SoundManager.h"

//TODO: Implement A* movement to bullet, create a move bullet by step function

View::View(GtkWidget *window) {
    this->window = window;
    GtkWidget* vbox = createVBox(window);

    // Crear la etiqueta del temporizador con el tiempo inicial (por ejemplo, "05:00")
    timerLabel = gtk_label_new("05:00");

    // Configurar alineación de la etiqueta
    gtk_widget_set_halign(timerLabel, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(timerLabel, GTK_ALIGN_START);

    // Añadir la etiqueta del temporizador al vbox
    gtk_box_pack_start(GTK_BOX(vbox), timerLabel, FALSE, FALSE, 10);

    startTimer();

    GtkWidget* hbox = createHBox(vbox);
    createDrawingArea(hbox);
    createStatusBar(vbox);
    loadAssets();
    connectSignals();
    gtk_widget_show_all(window);
    initSound();
}


void View::setGridMap(GridGraph *map) {
    gridMap = map;
}

void View::setTanks(Tank* tanks) {
    this->tanks = tanks;
}

void View::setPlayers(Player* players) {
    this->players = players;
}


void View::update() const {
    gtk_widget_queue_draw(drawingArea);
}

void View::addTrace() const {
    if (const int currentIndex = traceDistance + 1 - bullet->getDistance() - 1;
        0 <= currentIndex && currentIndex < traceDistance) {
        bulletTrace[currentIndex] = bullet->getPosition();
    }
}



GtkWidget* View::createVBox(GtkWidget *window) {
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_vexpand(vbox, TRUE);

    return vbox;
}

GtkWidget* View::createHBox(GtkWidget *vbox) {
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(hbox), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 0);

    return hbox;
}

void View::createDrawingArea(GtkWidget *hbox) {
    drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, COLS * CELL_SIZE, ROWS * CELL_SIZE);
    gtk_box_pack_start(GTK_BOX(hbox), drawingArea, FALSE, FALSE, 0);
}

void View::createStatusBar(GtkWidget *vbox) {
    statusBar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), statusBar, TRUE, TRUE, 0);
}

GtkWidget* View::createPlayerLabel(int player) {
    const std::string labelText = "Player " + std::to_string(player + 1);
    GtkWidget* label = gtk_label_new(labelText.c_str());
    playerLabels[player] = label; // Almacenar el label en el arreglo
    return label;
}

void View::updatePlayerLabels() {
    for (int i = 0; i < 2; ++i) {
        if (i == currentPlayer) {
            // Resaltar el jugador en turno (por ejemplo, cambiar color a verde)
            GdkRGBA color;
            gdk_rgba_parse(&color, "green");
            gtk_widget_override_color(playerLabels[i], GTK_STATE_FLAG_NORMAL, &color);
        } else {
            // Restablecer el color del jugador que no está en turno
            gtk_widget_override_color(playerLabels[i], GTK_STATE_FLAG_NORMAL, nullptr);
        }
    }
}

GtkWidget* View::createPlayerBox(const int player) const {
    GtkWidget* playerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    for (int col = 0; col < 2; ++col) {
        GtkWidget* colBox = createTankBox(player, col);
        gtk_box_pack_start(GTK_BOX(playerBox), colBox, FALSE, FALSE, 0);

        if (col == 0) {
            GtkWidget* separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
            gtk_box_pack_start(GTK_BOX(playerBox), separator, FALSE, FALSE, 10);
        }
    }

    return playerBox;
}

GtkWidget* View::createTankBox(const int player, const int col) const {
    GtkWidget* colBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    for (int tankIndex = 0; tankIndex < 2; ++tankIndex) {
        const int tankId = player * 4 + col * 2 + tankIndex;
        const Tank& tank = tanks[tankId];
        GtkWidget* hbox = createTankDisplay(tank);
        gtk_box_pack_start(GTK_BOX(colBox), hbox, FALSE, FALSE, 0);
    }

    return colBox;
}

GtkWidget* View::createTankDisplay(const Tank& tank) const {
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GdkPixbuf* pixbuf = nullptr;
    switch (tank.getColor()) {
        case Yellow: pixbuf = assets.at("yellow_tank"); break;
        case Red: pixbuf = assets.at("red_tank"); break;
        case Cian: pixbuf = assets.at("cian_tank"); break;
        case Blue: pixbuf = assets.at("blue_tank"); break;
    }

    GtkWidget* image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);

    const std::string healthText = "Health: " + std::to_string(tank.getHealth());
    GtkWidget* label = gtk_label_new(healthText.c_str());
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    return hbox;
}

GtkWidget* View::createPowerUpLabel(const int player) {
    GtkWidget* label = gtk_label_new(players[player].getPowerUpName().c_str());
    powerUpLabels[player] = label;
    return label;
}


void View::loadAssets() {
    if (GdkPixbuf* originalCell = gdk_pixbuf_new_from_file("../assets/textures/accessible.png", nullptr)) {
        assets["cell"] = gdk_pixbuf_scale_simple(originalCell, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(originalCell);
    }

    if (GdkPixbuf* originalObstacle = gdk_pixbuf_new_from_file("../assets/textures/inaccessible.png", nullptr)) {
        assets["obstacle"] = gdk_pixbuf_scale_simple(originalObstacle, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(originalObstacle);
    }

    if (GdkPixbuf* yellowTank = gdk_pixbuf_new_from_file("../assets/tanks/yellow_tank.png", nullptr)) {
        assets["yellow_tank"] = gdk_pixbuf_scale_simple(yellowTank, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(yellowTank);
    }

    if (GdkPixbuf* redTank = gdk_pixbuf_new_from_file("../assets/tanks/red_tank.png", nullptr)) {
        assets["red_tank"] = gdk_pixbuf_scale_simple(redTank, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(redTank);
    }

    if (GdkPixbuf* blueTank = gdk_pixbuf_new_from_file("../assets/tanks/blue_tank.png", nullptr)) {
        assets["blue_tank"] = gdk_pixbuf_scale_simple(blueTank, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(blueTank);
    }

    if (GdkPixbuf* cianTank = gdk_pixbuf_new_from_file("../assets/tanks/cian_tank.png", nullptr)) {
        assets["cian_tank"] = gdk_pixbuf_scale_simple(cianTank, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
        g_object_unref(cianTank);
    }

    if (GdkPixbuf* bullet = gdk_pixbuf_new_from_file("../assets/bullet.png", nullptr)) {
        assets["bullet"] = gdk_pixbuf_scale_simple(bullet, CELL_SIZE / 2, CELL_SIZE / 2, GDK_INTERP_BILINEAR);
        g_object_unref(bullet);
    }

    // Cargar las imágenes de explosión
    for (int i = 1; i <= 7; ++i) {
        std::string key = "explosion_" + std::to_string(i);
        std::string filename = "../assets/explosion/explosion_" + std::to_string(i) + ".png";

        if (GdkPixbuf* explosionImage = gdk_pixbuf_new_from_file(filename.c_str(), nullptr)) {
            // Ajustar el tamaño al tamaño de la celda
            assets[key] = gdk_pixbuf_scale_simple(explosionImage, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
            g_object_unref(explosionImage);
        }
    }
}

void View::connectSignals() {
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(onDraw), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(onClick), this);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);
}


gboolean View::onDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    const auto view = static_cast<View*>(data);

    view->drawMap(cr);
    view->drawTanks(cr);
    view->drawExplosions(cr);
    view->updateStatusBar();
    view->updatePlayerLabels();
    view->drawBullet(cr);
    view->drawBulletTrace(cr);

    return FALSE;
}

void View::drawMap(cairo_t *cr) {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col <COLS; ++col) {
            const Node& node = gridMap->getNode(row, col);

            const GdkPixbuf* pixbuf = nullptr;
            if (!node.obstacle) {
                pixbuf = assets["obstacle"];
            } else {
                pixbuf = assets["cell"];
            }

            gdk_cairo_set_source_pixbuf(cr, pixbuf, col * CELL_SIZE, row * CELL_SIZE);
            cairo_paint(cr);
        }
    }
}
/*
void View::drawMap(cairo_t *cr) {
    // Establecer la fuente y el tamaño del texto
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12); // Ajusta el tamaño de la fuente según sea necesario

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            const Node& node = gridMap->getNode(row, col);

            // Obtener el ID del nodo actual
            int nodeId = gridMap->toIndex(row, col);

            // Coordenadas y tamaño del cuadrado
            double x = col * CELL_SIZE;
            double y = row * CELL_SIZE;
            double size = CELL_SIZE;

            // Determinar el color de relleno según si el nodo es seguro o no
            if (gridMap->isSafeNode(nodeId)) {
                // Nodo seguro: cuadrado verde con borde negro
                cairo_set_source_rgb(cr, 0.0, 1.0, 0.0); // Color verde
            } else {
                // Nodo no seguro: cuadrado rojo con borde negro
                cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // Color rojo
            }

            // Dibujar el cuadrado relleno
            cairo_rectangle(cr, x, y, size, size);
            cairo_fill_preserve(cr); // Rellenar y preservar el camino

            // Establecer el color y ancho del borde
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Negro
            cairo_set_line_width(cr, 1.0); // Ancho del borde

            // Dibujar el borde del cuadrado
            cairo_stroke(cr);

            // Dibujar el número de ID dentro del cuadrado
            // Establecer el color del texto (negro o blanco, según el fondo)
            if (gridMap->isSafeNode(nodeId)) {
                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Texto negro en fondo verde
            } else {
                cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Texto blanco en fondo rojo
            }

            // Convertir el nodeId a cadena de texto
            std::string idText = std::to_string(nodeId);

            // Obtener las dimensiones del texto para centrarlo
            cairo_text_extents_t extents;
            cairo_text_extents(cr, idText.c_str(), &extents);

            // Calcular la posición para centrar el texto
            double textX = x + (size - extents.width) / 2 - extents.x_bearing;
            double textY = y + (size - extents.height) / 2 - extents.y_bearing;

            // Dibujar el texto
            cairo_move_to(cr, textX, textY);
            cairo_show_text(cr, idText.c_str());
        }
    }
}*/

void View::drawTanks(cairo_t *cr) {
    for (int i = 0; i < 8; i++) {
        const Tank* tank = &tanks[i];

        // Saltar tanques destruidos
        if (tank->isDestroyed()) {
            continue;
        }

        const GdkPixbuf* pixbuf = nullptr;
        switch (tank->getColor()) {
            case Yellow: pixbuf = assets["yellow_tank"]; break;
            case Red: pixbuf = assets["red_tank"]; break;
            case Cian: pixbuf = assets["cian_tank"]; break;
            case Blue: pixbuf = assets["blue_tank"]; break;
        }

        // Rotar el pixbuf según el ángulo del tanque
        GdkPixbuf* rotated_pixbuf = nullptr;
        const double angle = tank->getRotationAngle();
        GdkPixbufRotation rotation;

        if (angle == 0.0) {
            rotation = GDK_PIXBUF_ROTATE_NONE;
        } else if (angle == 90.0) {
            rotation = GDK_PIXBUF_ROTATE_CLOCKWISE;
        } else if (angle == 180.0) {
            rotation = GDK_PIXBUF_ROTATE_UPSIDEDOWN;
        } else if (angle == 270.0) {
            rotation = GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE;
        } else {
            rotation = GDK_PIXBUF_ROTATE_NONE;
        }

        rotated_pixbuf = gdk_pixbuf_rotate_simple(pixbuf, rotation);

        // Dibujar el tanque rotado
        gdk_cairo_set_source_pixbuf(cr, rotated_pixbuf, tank->getColumn() * CELL_SIZE, tank->getRow() * CELL_SIZE);
        cairo_paint(cr);

        g_object_unref(rotated_pixbuf); // Liberar el pixbuf rotado

        // Dibujar marco rojo si el tanque está seleccionado
        if (tank->isSelected()) {
            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // Color rojo
            cairo_set_line_width(cr, 2.0);
            cairo_rectangle(cr, tank->getColumn() * CELL_SIZE, tank->getRow() * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            cairo_stroke(cr);
        }
    }
}


void View::updateStatusBar() {
    gtk_container_foreach(GTK_CONTAINER(statusBar), reinterpret_cast<GtkCallback>(gtk_widget_destroy), nullptr);

    for (int player = 0; player < 2; ++player) {
        GtkWidget* playerContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget* playerLabel = createPlayerLabel(player);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerLabel, FALSE, FALSE, 0);

        GtkWidget* playerBox = createPlayerBox(player);
        gtk_widget_set_halign(playerBox, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(playerBox, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerBox, TRUE, TRUE, 0);

        GtkWidget* powerUpLabel = createPowerUpLabel(player);
        gtk_box_pack_start(GTK_BOX(playerContainer), powerUpLabel, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(statusBar), playerContainer, TRUE, TRUE, 0);
    }

    gtk_widget_show_all(statusBar);
}

void View::drawBullet(cairo_t *cr) const {
    if (bullet) {
        auto [row, column] = bullet->getPosition();
        const GdkPixbuf* pixbuf = assets.at("bullet");
        gdk_cairo_set_source_pixbuf(cr, pixbuf, column * CELL_SIZE + CELL_SIZE / 4, row * CELL_SIZE + CELL_SIZE / 4);
        cairo_paint(cr);
    }
}

void View::drawBulletTrace(cairo_t *cr) const {
    if (bullet && bulletTrace) {
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        const int current = traceDistance - bullet->getDistance();
        for (int i = 0; i < current; i++) {
            const auto [row, column] = bulletTrace[i];
            const double x = column * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
            const double y = row * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
            cairo_rectangle(cr, x, y, TRACE_SIZE, TRACE_SIZE);
            cairo_fill(cr);
        }
    }
}


gboolean View::onClick(GtkWidget *widget, const GdkEventButton *event, gpointer data) {
    auto* view = static_cast<View*>(data);

    if (view->gameOver) {
        return FALSE;
    }

    const int row = event->y / CELL_SIZE;
    const int column = event->x / CELL_SIZE;
    const auto position = Position(row, column);

    if (event->button == 1) {
        if (Tank* clickedTank = view->getTankOnPosition(position)) {
            view->handleSelectTank(clickedTank);
            g_print("Tank selected\n");
        } else if (cellClicked(position)) {
            if (Tank* selectedTank = view->getSelectedTank()) {
                view->handleMoveTank(selectedTank, position);
            }
        }

    } else if (event->button == 3) {
        if (Tank* selectedTank = view->getSelectedTank()) {
            if (selectedTank->isDestroyed()) {
                return TRUE;
            }
            view->handleFireBullet(Position(selectedTank->getRow(), selectedTank->getColumn()), position);
            selectedTank->setSelected(false);
            view->update();
        }
    }

    return TRUE;
}

void View::handleSelectTank(Tank* tank) const {
    if (tank->getPlayer()->getId() != currentPlayer || tank->isDestroyed()) {
        // No permitir seleccionar tanques del otro jugador o que estén destruidos
        return;
    }
    deselectAllTanks();
    tank->setSelected(true);
    update();
}

void View::handleFireBullet(const Position &origin, const Position &target) {
    const POWER_UP currentPowerUp = players[currentPlayer].getPowerUp();

    // Create bullet
    bullet = new Bullet(origin, target);

    if (currentPowerUp == ATTACK_POWER) {
        bullet->setMaxDamage(true);
    }

    // Calculate bullet path
    const Pathfinder pathfinder(*gridMap);
    Queue* path;

    if (currentPowerUp == ATTACK_PRECISION) {
        path = pathfinder.aStar(origin, target);
    } else {
        path = pathfinder.lineaVista(origin, target);
    }
    bullet->setPath(*path);

    traceDistance = bullet->getDistance() - 1;
    bulletTrace = new Position[traceDistance];
    g_timeout_add(100, handleMoveBullet, this);

    // Reproducir efecto de sonido de disparo
    soundManager.playSoundEffect("fire");

    // Decrementar acciones restantes
    actionsRemaining--;

    // Verificar si se debe cambiar de turno
    if (actionsRemaining <= 0) {
        endTurn();
    }

    update();
}

gboolean View::handleMoveBullet(gpointer data) {
    auto* view = static_cast<View*>(data);
    if (view->bullet) {

        if (view->gameOver) {
            return FALSE;
        }

        if (view->bullet->move()) {
            view->destroyBullet();
            view->destroyBulletTrace();
            view->update();
            return FALSE;
        }

        if (view->bulletHitTank(view->bullet)) {
            Tank* tankHit = view->getTankOnPosition(view->bullet->getPosition());
            tankHit->applyDamage(view->bullet->getMaxDamage());

            // Reproducir efecto de sonido de impacto
            view->soundManager.playSoundEffect("impact");

            if (tankHit->getHealth() <= 0 && !tankHit->isDestroyed()) {
                tankHit->destroy(); // Marcar el tanque como destruido
                view->gridMap->removeTank(tankHit->getRow(), tankHit->getColumn()); // Remover del mapa

                // Iniciar la explosión en la posición del tanque
                Explosion explosion;
                int row = tankHit->getRow();
                int column = tankHit->getColumn();
                explosion.position = Position(row, column);
                explosion.currentFrame = 0;
                view->explosions.push_back(explosion);

                // Reproducir efecto de sonido de explosión
                view->soundManager.playSoundEffect("explosion");

                // Iniciar el temporizador para animar la explosión
                g_timeout_add(100, View::animateExplosions, view);

                // Verificar si todos los tanques del jugador han sido destruidos
                // if (view->areAllTanksDestroyed(tankHit->getPlayer())) {
                //     // Finalizar el juego
                //     view->endGameDueToDestruction(tankHit->getPlayer());
                //     view->update();
                //     return FALSE;
                // }
            }

            view->destroyBullet();
            view->destroyBulletTrace();
            view->update();
            return FALSE;
        }

        if (view->BulletHitWall(view->bullet)) {
            handleBulletBounce(view->bullet);
        }

        view->addTrace();
        view->update();
        return TRUE;
    }

    return FALSE;
}

void View::handleBulletBounce(Bullet* bullet) {
    auto [x, y] = bullet->getDirection();
    bullet->setDirection(Direction(-x, -y));
}


Tank* View::getTankOnPosition(const Position position) const {
    for (int i = 0; i < 8; i++) {
        Tank* tank = &tanks[i];
        if (!tank->isDestroyed() &&
            position.row == tank->getRow() && position.column == tank->getColumn()) {
            return tank;
        }
    }

    return nullptr;
}

Tank* View::getSelectedTank() const {
    for (int i = 0; i < 8; i++) {
        if (Tank* tank = &tanks[i];
            tank->isSelected()) {
            return tank;
            }
    }

    return nullptr;
}

bool View::cellClicked(const Position position) {
    return position.row >= 0 && position.row < ROWS && position.column >= 0 && position.column < COLS;
}

bool View::bulletHitTank(const Bullet* bullet) const {
    if (auto [row, col] = bullet->getPosition();
    gridMap->isOccupied(row, col)) {
        return true;
    }

    return false;
}


void View::handleMoveTank(Tank* tank, const Position position)  {
    if (tank->isDestroyed()) {
        return;
    }
    Pathfinder pathfinder(*gridMap);
    int startId = gridMap->toIndex(tank->getRow(), tank->getColumn());
    int goalId = gridMap->toIndex(position.row, position.column);

    int color = tank->getColor();

    // Generar un número aleatorio entre 1 y 10
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);
    int randomNumber = dist(gen);
    std::vector<int> path;
    if (color == 0 || color == 1) {
        // Tanque seleccionado: Rojo o amarillo
        // 50% de probabilidad BFS o 50% movimiento aleatorio
        if (randomNumber <= 5) {
            // 50% de probabilidad BFS
            path = pathfinder.bfs(startId, goalId);
            std::cout << "Color:" << color << " Algoritmo usado: BFS" << std::endl;
        } else {
            // 50% de probabilidad movimiento aleatorio
            path = pathfinder.randomMovement(startId, goalId);
            std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
        }
    } else if (color == 2 || color == 3) {
        // Tanque seleccionado: Celeste o azul
        // 80% de probabilidad Dijkstra o 20% movimiento aleatorio
        if (randomNumber <= 8) {
            // 80% de probabilidad Dijkstra
            path = pathfinder.dijkstra(startId, goalId);
            std::cout << "Color:" << color << " Algoritmo usado: Dijkstra" << std::endl;
        } else {
            // 20% de probabilidad para Acción D
            path = pathfinder.randomMovement(startId, goalId);
            std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
        }
    }

    if (path.size() < 2) {
        tank->setSelected(false);
        update();
        return;
    }

    // Decrementar acciones restantes
    actionsRemaining--;

    // Verificar si se debe cambiar de turno
    if (actionsRemaining <= 0) {
        endTurn();
    }

    // Reproducir efecto de sonido de movimiento en bucle
    moveSoundChannel = soundManager.playSoundEffect("move", -1);

    // Crear la estructura de datos para el movimiento
    auto* moveData = new MoveData{const_cast<View*>(this), tank, path, 1};

    // Iniciar el temporizador para mover el tanque
    g_timeout_add(100, View::moveTankStep, moveData);
}

void View::MoveTank(Tank* tank, const Position position) const {
    if (tank->isSelected()) {
        if (!gridMap->isObstacle(position.row, position.column) && !gridMap->isOccupied(position.row, position.column)) {
            gridMap->removeTank(tank->getRow(), tank->getColumn());
            gridMap->placeTank(position.row, position.column);

            tank->setPosition(position);
            update();
        }
    }
}

bool View::BulletHitWall(const Bullet* bullet) const {
    if (auto [row, col] = bullet->getPosition();
    gridMap->isObstacle(row, col)) {
        return true;
    }
    return false;
}

gboolean View::moveTankStep(gpointer data) {
    auto* moveData = static_cast<MoveData*>(data);
    View* view = moveData->view;
    Tank* tank = moveData->tank;
    const std::vector<int>& path = moveData->path;
    std::size_t& currentStep = moveData->currentStep;

    if (currentStep >= path.size()) {
        // Movimiento completado
        tank->setSelected(false); // Deseleccionar el tanque al finalizar el movimiento
        view->update(); // Actualizar la interfaz gráfica

        // Detener el efecto de sonido de movimiento
        if (view->moveSoundChannel != -1) {
            view->soundManager.stopSoundEffect("move");
            view->moveSoundChannel = -1;
        }

        delete moveData;
        return FALSE; // Detener el temporizador
    }

    int id = path[currentStep];
    const int row = id / view->gridMap->getCols();
    const int column = id % view->gridMap->getCols();

    // Obtener la posición anterior
    int prevId;
    if (currentStep > 0) {
        prevId = path[currentStep - 1];
    } else {
        prevId = view->gridMap->toIndex(tank->getRow(), tank->getColumn());
    }
    const int prevRow = prevId / view->gridMap->getCols();
    const int prevColumn = prevId % view->gridMap->getCols();

    // Calcular la dirección del movimiento
    int deltaRow = row - prevRow;
    int deltaCol = column - prevColumn;

    // Actualizar el ángulo de rotación del tanque
    if (deltaRow == 0 && deltaCol == 1) {
        // Movimiento hacia la derecha
        tank->setRotationAngle(0.0); // Sin rotación
    } else if (deltaRow == 1 && deltaCol == 0) {
        // Movimiento hacia abajo
        tank->setRotationAngle(90.0); // Rotación de 90 grados en sentido horario
    } else if (deltaRow == 0 && deltaCol == -1) {
        // Movimiento hacia la izquierda
        tank->setRotationAngle(180.0); // Rotación de 180 grados
    } else if (deltaRow == -1 && deltaCol == 0) {
        // Movimiento hacia arriba
        tank->setRotationAngle(270.0); // Rotación de 270 grados (90 grados antihorario)
    }

    currentStep++;
    const auto NewPosition = Position(row, column);
    view->MoveTank(tank, NewPosition);

    return TRUE; // Continuar el temporizador
}

void View::deselectAllTanks() const {
    for (int i = 0; i < 8; i++) {
        tanks[i].setSelected(false);
    }
}

void View::destroyBullet() {
    if (bullet) {
        delete bullet;
        bullet = nullptr;
    }
}
void View::destroyBulletTrace(){
    if (bulletTrace) {
        delete[] bulletTrace;
        bulletTrace = nullptr;
        traceDistance = 0;
    }
}

// Implementación de startTimer()
void View::startTimer() {
    // Configurar una función que se llame cada segundo
    g_timeout_add_seconds(1, updateTimer, this);
    g_timeout_add_seconds(20, grantPowerUp, this);
}

gboolean View::grantPowerUp(gpointer data) {
    const auto* view = static_cast<View*>(data);
    for (int i = 0; i < 2; i++) {
        view->players[i].generatePowerUp();
    }
    view->update();

    return TRUE;
}

// Implementación de updateTimer()
gboolean View::updateTimer(gpointer data) {
    auto* view = static_cast<View*>(data);

    if (view->remaining_time > 0) {
        --(view->remaining_time);

        // Calcular minutos y segundos
        int minutes = view->remaining_time / 60;
        int seconds = view->remaining_time % 60;

        // Formatear el tiempo como MM:SS
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);

        // Actualizar la etiqueta del temporizador directamente
        gtk_label_set_text(GTK_LABEL(view->timerLabel), buffer);

        return TRUE; // Continuar el temporizador
    } else {
        // El temporizador ha llegado a cero, finalizar el juego
        view->endGameDueToTime();
        return FALSE; // Detener el temporizador
    }
}

void View::endTurn() {
    // Cambiar al siguiente jugador
    currentPlayer = (currentPlayer + 1) % 2;
    actionsRemaining = 1; // Restablecer acciones (o el valor que corresponda)
    updatePlayerLabels();
    update(); // Actualizar la interfaz gráfica
}

void View::setActionsPerTurn(int actions) {
    actionsRemaining = actions;
}

void View::endGameDueToTime() {
    if (gameOver) return; // Evitar llamadas múltiples
    gameOver = true;

    // Reproducir efecto de sonido de fin de juego
    soundManager.playSoundEffect("game_over");

    int winner = determineWinner();
    if (winner == -1) {
        showTieMessage();
    } else {
        showWinnerMessage(winner);
    }
}


bool View::areAllTanksDestroyed(int player) {
    for (int i = 0; i < 8; ++i) {
        Tank& tank = tanks[i];
        if (tank.getPlayer()->getId() == player && !tank.isDestroyed()) {
            // Aún queda al menos un tanque de este jugador
            return false;
        }
    }
    return true; // Todos los tanques de este jugador han sido destruidos
}

void View::endGameDueToDestruction(int losingPlayer) {
    if (gameOver) return; // Evitar llamadas múltiples
    gameOver = true;

    soundManager.stopBackgroundMusic();

    // Reproducir efecto de sonido de fin de juego
    soundManager.playSoundEffect("game_over");

    int winner = (losingPlayer == 0) ? 1 : 0;
    showWinnerMessage(winner);
}
void View::showWinnerMessage(int winner) {
    // Crear un mensaje con el ganador
    std::string message = "¡El jugador " + std::to_string(winner + 1) + " ha ganado!";

    // Mostrar un diálogo con el mensaje
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Cerrar la ventana o reiniciar el juego
    gtk_widget_destroy(window);

}

void View::showTieMessage() {
    // Mostrar un diálogo indicando que hay un empate
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "¡El juego ha terminado en empate!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Cerrar la ventana o reiniciar el juego
    gtk_widget_destroy(window);

}

int View::determineWinner() {
    int tanksPlayer0 = 0;
    int tanksPlayer1 = 0;

    for (int i = 0; i < 8; ++i) {
        Tank& tank = tanks[i];
        if (!tank.isDestroyed()) {
            if (tank.getPlayer() == 0) {
                tanksPlayer0++;
            } else if (tank.getPlayer()->getId() == 1) {
                tanksPlayer1++;
            }
        }
    }

    if (tanksPlayer0 > tanksPlayer1) {
        return 0; // Gana el jugador 1
    } else if (tanksPlayer1 > tanksPlayer0) {
        return 1; // Gana el jugador 2
    } else {
        // Empate
        return -1;
    }
}


gboolean View::animateExplosions(gpointer data) {
    auto* view = static_cast<View*>(data);

    bool anyActive = false;

    for (auto it = view->explosions.begin(); it != view->explosions.end(); ) {
        it->currentFrame++;

        if (it->currentFrame >= 7) {
            // Animación terminada, eliminar explosión
            it = view->explosions.erase(it);
        } else {
            anyActive = true;
            ++it;
        }
    }

    view->update();

    return anyActive ? TRUE : FALSE;
}

void View::drawExplosions(cairo_t *cr) {
    for (const auto& explosion : explosions) {
        std::string key = "explosion_" + std::to_string(explosion.currentFrame + 1);
        GdkPixbuf* pixbuf = assets[key];

        int row = explosion.position.row;
        int column = explosion.position.column;

        gdk_cairo_set_source_pixbuf(cr, pixbuf, column * CELL_SIZE, row * CELL_SIZE);
        cairo_paint(cr);
    }
}

void View::initSound() {
    // Reproducir música de fondo
    soundManager.playBackgroundMusic("../assets/sounds/background_music.mp3");

    // Cargar efectos de sonido
    soundManager.loadSoundEffect("fire", "../assets/sounds/fire.wav");
    soundManager.loadSoundEffect("impact", "../assets/sounds/impact.wav");
    soundManager.loadSoundEffect("explosion", "../assets/sounds/explosion.wav");
    soundManager.loadSoundEffect("move", "../assets/sounds/move.wav");
    soundManager.loadSoundEffect("game_over", "../assets/sounds/game_over.wav");
}



