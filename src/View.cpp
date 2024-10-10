#include "View.h"
#include "systems/Pathfinder.h"
#include <chrono>
#include <thread>
#include <random>
#include <cmath>

View::View(GtkWidget *window) {
    GtkWidget* vbox = createVBox(window);
    GtkWidget* hbox = createHBox(vbox);
    createDrawingArea(hbox);
    createStatusBar(vbox);
    loadAssets();
    connectSignals();
    gtk_widget_show_all(window);
}

void View::setGridMap(GridGraph *map) {
    gridMap = map;
}

void View::setTanks(Tank* tanks) {
    this->tanks = tanks;
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

GtkWidget* View::createPlayerLabel(const int player) {
    const std::string labelText = "Player " + std::to_string(player + 1);
    return gtk_label_new(labelText.c_str());
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
    view->updateStatusBar();
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
    const GdkPixbuf* pixbuf = nullptr;
    for (int i = 0; i < 8; i++) {
        const auto tank = &tanks[i];
        if (tank->getColor() == Yellow) {
            pixbuf = assets["yellow_tank"];
        } else if (tank->getColor() == Red) {
            pixbuf = assets["red_tank"];
        } else if (tank->getColor() == Cian) {
            pixbuf = assets["cian_tank"];
        } else if (tank->getColor() == Blue) {
            pixbuf = assets["blue_tank"];
        }
        gdk_cairo_set_source_pixbuf(cr, pixbuf, tank->getColumn() * CELL_SIZE, tank->getRow() * CELL_SIZE);
        cairo_paint(cr);
    }
}

void View::updateStatusBar() const {
    gtk_container_foreach(GTK_CONTAINER(statusBar), reinterpret_cast<GtkCallback>(gtk_widget_destroy), nullptr);

    for (int player = 0; player < 2; ++player) {
        GtkWidget* playerContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget* playerLabel = createPlayerLabel(player);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerLabel, FALSE, FALSE, 0);

        GtkWidget* playerBox = createPlayerBox(player);
        gtk_widget_set_halign(playerBox, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(playerBox, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerBox, TRUE, TRUE, 0);

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

    const int row = event->y / CELL_SIZE; // Y position
    const int column = event->x / CELL_SIZE; // X position
    const auto position = Position(row, column);

    if (event->button == 1) { // Left click
        if (Tank* clickedTank = view->getTankOnPosition(position)) { // Tank clicked
            view->handleSelectTank(clickedTank); // Select tank
            g_print("Tank selected\n"); // For debugging purposes

        } else if (cellClicked(position)) { // Cell clicked
            if (Tank* selectedTank = view->getSelectedTank()) { // There's a selected tank
                view->handleMoveTank(selectedTank, position); // Move tank
            }
        }

    } else if (event->button == 3) { // Right click
        if (Tank* selectedTank = view->getSelectedTank()) { // There's a selected tank
            view->handleFireBullet(Position(selectedTank->getRow(), selectedTank->getColumn()), position); // Fire bullet
            selectedTank->setSelected(false); // Deselect tank
        }
    }

    return TRUE;
}

void View::handleSelectTank(Tank* tank) const {
    deselectAllTanks();
    tank->setSelected(true);
}

void View::handleFireBullet(const Position &origin, const Position &target) {
    bullet = new Bullet(origin, target);
    traceDistance = bullet->getDistance() - 1;
    bulletTrace = new Position[traceDistance];
    g_timeout_add(30, handleMoveBullet, this);
    update();
}

gboolean View::handleMoveBullet(gpointer data) {
    if (const auto* view = static_cast<View*>(data); view->bullet) {
        if (view->bullet->move()) {
            // view->destroyBullet();
            // view->destroyBulletTrace();

            view->update();
            return FALSE;
        }

        if (view->bulletHitTank(view->bullet)) {
            Tank* TankHit = view->getTankOnPosition(view->bullet->getPosition());
            TankHit->applyDamage();
            // view->destroyBullet();
            // view->destroyBulletTrace();

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
        if (Tank* tank = &tanks[i];
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


void View::handleMoveTank(Tank* tank, const Position position) const {
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
        return;
    }

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
        tank->setSelected(false);
        delete moveData;
        return FALSE; // Detener el temporizador
    }

    int id = path[currentStep++];
    const int row = id / view->gridMap->getCols();
    const int column = id % view->gridMap->getCols();
    const auto NewPosition = Position(row, column);
    view->MoveTank(tank, NewPosition);

    // La interfaz gráfica se actualiza en MoveTank a través de update()
    // El temporizador continuará hasta que se devuelva FALSE

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