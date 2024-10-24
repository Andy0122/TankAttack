#include "View.h"
#include "systems/Pathfinder.h"
#include <chrono>
#include <thread>
#include <random>
#include <cmath>
#include "systems/SoundManager.h"

using namespace std;
using namespace DATA_STRUCTURES;
//TODO: Refactored code, pass game logic to model

View::View(Controller* controller, GtkWidget *window)
: controller(controller), window(window) {
    GtkWidget* vbox = createVBox(window);

    createTimerLabel(vbox);
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
    gridMapGame = map;
}

void View::setTanks(Tank* tanks) {
    this->tanksList = tanks;
}

void View::setPlayers(Player* players) {
    this->playersList = players;
}


void View::update() const {
    gtk_widget_queue_draw(drawingArea);
}

void View::addTrace() const {
    if (bulletTrace) {
        bulletTrace->append(bulletGame->getPosition());
    }
}



GtkWidget* View::createVBox(GtkWidget *window) {
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_vexpand(vbox, TRUE);

    return vbox;
}

void View::createTimerLabel(GtkWidget *vbox) {
    timerLabel = gtk_label_new("05:00");

    gtk_widget_set_halign(timerLabel, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(timerLabel, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(vbox), timerLabel, FALSE, FALSE, 10);
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

    gtk_widget_set_can_focus(drawingArea, TRUE); // Ensure the widget can receive focus
    gtk_widget_add_events(drawingArea, GDK_KEY_PRESS_MASK); // Add key press events
}

void View::createStatusBar(GtkWidget *vbox) {
    statusBar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_pack_start(GTK_BOX(vbox), statusBar, TRUE, TRUE, 0);
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

    // Load explosion images
    for (int i = 1; i <= 7; ++i) {
        string key = "explosion_" + to_string(i);
        string filename = "../assets/explosion/explosion_" + to_string(i) + ".png";

        if (GdkPixbuf* explosionImage = gdk_pixbuf_new_from_file(filename.c_str(), nullptr)) {
            // Resize the image
            assets[key] = gdk_pixbuf_scale_simple(explosionImage, CELL_SIZE, CELL_SIZE, GDK_INTERP_BILINEAR);
            g_object_unref(explosionImage);
        }
    }
}

void View::connectSignals() {
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(onDraw), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(onClick), this);
    g_signal_connect(G_OBJECT(drawingArea), "key-press-event", G_CALLBACK(onKeyPress), this);
    gtk_widget_add_events(drawingArea, GDK_KEY_PRESS_MASK | GDK_BUTTON_PRESS_MASK);
}


gboolean View::onDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    const auto view = static_cast<View*>(data);

    view->drawMap(cr);
    view->drawTanks(cr);
    view->drawStatusBar();
    view->drawBulletTrace(cr);
    view->drawBullet(cr);
    // view->drawExplosions(cr);

    return FALSE;
}

void View::drawMap(cairo_t *cr) {
    const GridGraph* gridMap = controller->getMap();

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            const Node& node = gridMap->getNode(row, col);

            // Get the cell image
            const GdkPixbuf* pixbuf = selectCellImage(node);

            // Draw the cell
            gdk_cairo_set_source_pixbuf(cr, pixbuf, col * CELL_SIZE, row * CELL_SIZE);
            cairo_paint(cr);
        }
    }
}

GdkPixbuf* View::selectCellImage(const Node& node) {
    GdkPixbuf* pixbuf = nullptr;
    if (!node.obstacle) {
        pixbuf = assets["obstacle"];
    } else {
        pixbuf = assets["cell"];
    }

    return pixbuf;
}

void View::drawTanks(cairo_t *cr) {
    const Tank* tanks = controller->getTanks();

    for (int i = 0; i < 8; i++) {
        const Tank* tank = &tanks[i];

        if (tank->isDestroyed()) {
            continue;
        }

        // Get the tank image
        const GdkPixbuf* tankImage = selectTankImage(tank->getColor());

        // Rotate the tank image
        GdkPixbuf* rotatedTankImage = rotateTankImage(tankImage, tank->getRotationAngle());

        // Draw the tank
        gdk_cairo_set_source_pixbuf(cr, rotatedTankImage, tank->getColumn() * CELL_SIZE, tank->getRow() * CELL_SIZE);
        cairo_paint(cr);

        // Free the GdkPixbuf
        g_object_unref(rotatedTankImage);

        // Draw the selected marker
        if (tank->isSelected()) {
            drawSelectedMarker(cr, *tank);
        }
    }
}

GdkPixbuf* View::selectTankImage(const Color color) {
    GdkPixbuf* pixbuf = nullptr;
    switch (color) {
        case Yellow: pixbuf = assets["yellow_tank"]; break;
        case Red: pixbuf = assets["red_tank"]; break;
        case Cian: pixbuf = assets["cian_tank"]; break;
        case Blue: pixbuf = assets["blue_tank"]; break;
    }

    return pixbuf;
}

GdkPixbuf* View::rotateTankImage(const GdkPixbuf* image, const double rotationAngle) {
    GdkPixbufRotation rotation;

    switch (static_cast<int>(rotationAngle)) {
        case 0:
            rotation = GDK_PIXBUF_ROTATE_NONE;
            break;
        case 90:
            rotation = GDK_PIXBUF_ROTATE_CLOCKWISE;
            break;
        case 180:
            rotation = GDK_PIXBUF_ROTATE_UPSIDEDOWN;
            break;
        case 270:
            rotation = GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE;
            break;
        default:
            rotation = GDK_PIXBUF_ROTATE_NONE;
            break;
    }

    return gdk_pixbuf_rotate_simple(image, rotation);
}

void View::drawSelectedMarker(cairo_t *cr, const Tank& tank) {
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_rectangle(cr, tank.getColumn() * CELL_SIZE, tank.getRow() * CELL_SIZE, CELL_SIZE, CELL_SIZE);
    cairo_stroke(cr);
}

void View::drawStatusBar() {
    gtk_container_foreach(GTK_CONTAINER(statusBar), reinterpret_cast<GtkCallback>(gtk_widget_destroy), nullptr);

    for (int id = 0; id < 2; ++id) {
        GtkWidget* playerContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        GtkWidget* playerLabel = createPlayerLabel(id);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerLabel, FALSE, FALSE, 0);

        GtkWidget* playerBox = createPlayerBox(id);
        gtk_widget_set_halign(playerBox, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(playerBox, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(playerContainer), playerBox, TRUE, TRUE, 0);

        GtkWidget* powerUpLabel = createPowerUpLabel(id);
        gtk_box_pack_start(GTK_BOX(playerContainer), powerUpLabel, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(statusBar), playerContainer, TRUE, TRUE, 0);
    }

    gtk_widget_show_all(statusBar);
}

GtkWidget* View::createPlayerLabel(const int playerId) const {
    const string labelText = "Player " + to_string(playerId + 1);
    GtkWidget* label = gtk_label_new(labelText.c_str());

    GdkRGBA color;
    if (playerId == controller->getCurrentPlayer()->getId()) {
        gdk_rgba_parse(&color, "green");
    } else {
        gdk_rgba_parse(&color, "white");
    }
    gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &color);

    return label;
}

GtkWidget* View::createPlayerBox(const int playerId) {
    GtkWidget* playerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    for (int col = 0; col < 2; ++col) {
        GtkWidget* colBox = createTankBox(playerId, col);
        gtk_box_pack_start(GTK_BOX(playerBox), colBox, FALSE, FALSE, 0);

        if (col == 0) {
            GtkWidget* separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
            gtk_box_pack_start(GTK_BOX(playerBox), separator, FALSE, FALSE, 10);
        }
    }

    return playerBox;
}

GtkWidget* View::createTankBox(const int playerId, const int col) {
    GtkWidget* colBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    const Tank* tanks = controller->getTanks();
    for (int tankIndex = 0; tankIndex < 2; ++tankIndex) {
        const int tankId = playerId * 4 + col * 2 + tankIndex;
        const Tank* tank = &tanks[tankId];
        GtkWidget* hbox = createTankDisplay(tank);
        gtk_box_pack_start(GTK_BOX(colBox), hbox, FALSE, FALSE, 0);
    }

    return colBox;
}

GtkWidget* View::createTankDisplay(const Tank* tank) {
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GdkPixbuf* tankImage = selectTankImage(tank->getColor());
    GtkWidget* image = gtk_image_new_from_pixbuf(tankImage);
    gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);

    const string healthText = "Health: " + to_string(tank->getHealth());
    GtkWidget* label = gtk_label_new(healthText.c_str());
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    return hbox;
}

GtkWidget* View::createPowerUpLabel(const int playerId) {
    const Player* player = &controller->getPlayers()[playerId];

    GtkWidget* label = gtk_label_new(player->getPowerUpName().c_str());

    GdkRGBA fontColor;
    player->getPowerUpActive() ? gdk_rgba_parse(&fontColor, "red") : gdk_rgba_parse(&fontColor, "white");

    gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &fontColor);

    return label;
}

void View::drawBulletTrace(cairo_t *cr) const {
    if (const Bullet* bullet = controller->getBullet();
        bullet != nullptr) {
        if (Queue<Position>* bulletTrace = bullet->getPath();
            !bulletTrace->empty()) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

            for (int i = 0; i < bulletTrace->size(); i++) {
                const auto [row, column] = bulletTrace->front();

                const double x = column * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
                const double y = row * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
                cairo_rectangle(cr, x, y, TRACE_SIZE, TRACE_SIZE);
                cairo_fill(cr);

                bulletTrace->pop();
            }
        }
    }
}

void View::drawBullet(cairo_t *cr) const { // Add the bullet orientation
    if (const Bullet* bullet = controller->getBullet()) {
        auto [row, column] = bullet->getPosition();

        const GdkPixbuf* pixbuf = assets.at("bullet");
        gdk_cairo_set_source_pixbuf(cr, pixbuf, column * CELL_SIZE + CELL_SIZE / 4, row * CELL_SIZE + CELL_SIZE / 4);
        cairo_paint(cr);
    }
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


gboolean View::onClick(GtkWidget *widget, const GdkEventButton *event, gpointer data) {
    auto* view = static_cast<View*>(data);
    const auto* controller = view->controller;

    if (view->gameOver) {
        return FALSE;
    }

    const int row = event->y / CELL_SIZE;
    const int column = event->x / CELL_SIZE;
    const auto position = Position(row, column);

    if (event->button == 1) {
        if (Tank* clickedTank = controller->getTankOnPosition(position)) {
            controller->handleSelectTank(clickedTank);
        } else if (cellClicked(position)) {
            if (Tank* selectedTank = controller->getSelectedTank()) {
                controller->handleMoveTank(selectedTank, position);

                Queue<Position>* path = controller->getTankPath();
                auto* moveData = new MoveData{view, selectedTank, path};
                view->moveSoundChannel = view->soundManager.playSoundEffect("move", -1);
                g_timeout_add(100, moveTankStep, moveData);
            }
        }

    } else if (event->button == 3) {
        if (Tank* selectedTank = view->getSelectedTank()) {
            if (selectedTank->isDestroyed()) {
                return TRUE;
            }
            view->handleFireBullet(Position(selectedTank->getRow(), selectedTank->getColumn()), position);
            selectedTank->setSelected(false);
        }
    }
    view->update();

    return TRUE;
}

gboolean View::moveTankStep(gpointer data) {
    const auto* moveData = static_cast<MoveData*>(data);
    View* view = moveData->view;
    Tank* tank = moveData->tank;
    Queue<Position>* path = moveData->path;

    if (path->empty()) {
        tank->setSelected(false);

        // Stop Sound Effect
        if (view->moveSoundChannel != -1) {
            view->soundManager.stopSoundEffect("move");
            view->moveSoundChannel = -1;
        }

        view->update();

        delete moveData;
        return FALSE;
    }

    auto [row, col] = path->front();
    path->pop();

    setTankRotationAngle(tank, row, col);

    const auto NewPosition = Position(row, col);
    view->controller->moveTank(tank, NewPosition);
    view->update();

    return TRUE;
}

void View::setTankRotationAngle(Tank* tank, const int destRow, const int destCol) {
    const int currentRow = tank->getRow();
    const int currentCol = tank->getColumn();

    const int deltaRow = destRow - currentRow;
    const int deltaCol = destCol - currentCol;

    double rotationAngle = 0.0;
    if (deltaRow == 0 && deltaCol == 1) {
        rotationAngle = 0.0;
    } else if (deltaRow == 1 && deltaCol == 0) {
        rotationAngle = 90.0;
    } else if (deltaRow == 0 && deltaCol == -1) {
        rotationAngle = 180.0;
    } else if (deltaRow == -1 && deltaCol == 0) {
        rotationAngle = 270.0;
    }

    tank->setRotationAngle(rotationAngle);
}

gboolean View::onKeyPress(GtkWidget* widget, GdkEventKey* event, gpointer data) {
    auto* view = static_cast<View*>(data);
    if (event->keyval == GDK_KEY_Shift_L || event->keyval == GDK_KEY_Shift_R) {
        view->handlePowerUpActivation();
    }

    return FALSE;
}

void View::handlePowerUpActivation() {
    Player& player = playersList[currentPlayer];

    if (player.getPowerUp() == NONE) {
        return;
    }

    player.setPowerUpActive(true);

    actionsRemaining--;

    if (actionsRemaining <= 0) {
        endTurn();
    }
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
    POWER_UP currentPowerUp;
    if (playersList[currentPlayer].getPowerUpActive()) {
        currentPowerUp = playersList[currentPlayer].getPowerUp();
        playersList[currentPlayer].setPowerUpActive(false);
        playersList[currentPlayer].erasePowerUp();
    } else {
        currentPowerUp = NONE;
    }

    createBullet(origin, target, currentPowerUp);

    bulletTrace = new LinkedList<Position>();

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

void View::createBullet(const Position& origin, const Position& target, const POWER_UP powerUp) {
    bulletGame = new Bullet(origin, target);

    if (powerUp == ATTACK_POWER) {
        bulletGame->setMaxDamage(true);
    }

    // Calculate bullet path
    const Pathfinder pathfinder(*gridMapGame);
    Queue<Position>* path;

    if (powerUp == ATTACK_PRECISION) {
        bulletGame->setPath(*pathfinder.aStar(origin, target));
        playersList[currentPlayer].setPowerUpActive(false);
        playersList[currentPlayer].erasePowerUp();
    } else {
        bulletGame->setPath(*pathfinder.lineaVista(origin, target));
    }
}


gboolean View::handleMoveBullet(gpointer data) {
    auto* view = static_cast<View*>(data);
    if (view->bulletGame && view->bulletGame->getPath() != nullptr) {

        if (view->gameOver) {
            return FALSE;
        }

        if (view->bulletGame->move()) {
            view->destroyBullet();
            view->destroyBulletTrace();
            view->update();
            return FALSE;
        }

        if (view->bulletHitTank(view->bulletGame)) {
            Tank* tankHit = view->getTankOnPosition(view->bulletGame->getPosition());
            tankHit->applyDamage(view->bulletGame->getMaxDamage());

            // Reproducir efecto de sonido de impacto
            view->soundManager.playSoundEffect("impact");

            if (tankHit->getHealth() <= 0 && !tankHit->isDestroyed()) {
                tankHit->destroy(); // Marcar el tanque como destruido
                view->gridMapGame->removeTank(tankHit->getRow(), tankHit->getColumn()); // Remover del mapa

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

        // if (view->BulletHitWall(view->bullet)) {
        //     handleBulletBounce(view->bullet);
        // }

        if (!view->bulletGame->reachedTarget()) {
            view->addTrace();
        }

        view->update();
        return TRUE;
    }

    return FALSE;
}

// void View::handleBulletBounce(Bullet* bullet) {
//     auto [x, y] = bullet->getDirection();
//     bullet->setDirection(Direction(-x, -y));
// }


Tank* View::getTankOnPosition(const Position position) const {
    for (int i = 0; i < 8; i++) {
        Tank* tank = &tanksList[i];
        if (!tank->isDestroyed() &&
            position.row == tank->getRow() && position.column == tank->getColumn()) {
            return tank;
        }
    }

    return nullptr;
}

Tank* View::getSelectedTank() const {
    for (int i = 0; i < 8; i++) {
        if (Tank* tank = &tanksList[i];
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
    gridMapGame->isOccupied(row, col)) {
        return true;
    }

    return false;
}


// void View::handleMoveTank(Tank* tank, const Position position)  {
//     if (tank->isDestroyed()) {
//         return;
//     }
//     Pathfinder pathfinder(*gridMapGame);
//     int startId = gridMapGame->toIndex(tank->getRow(), tank->getColumn());
//     int goalId = gridMapGame->toIndex(position.row, position.column);
//
//     int color = tank->getColor();
//
//     // Generar un número aleatorio entre 1 y 10
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dist(1, 10);
//     int randomNumber = dist(gen);
//     std::vector<int> path;
//     if (playersList[currentPlayer].getPowerUpActive() && playersList[currentPlayer].getPowerUp() == MOVEMENT_PRECISION) {
//         if (color == 0 || color == 1) {
//             if (randomNumber <= 9) {
//                 path = pathfinder.bfs(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: BFS" << std::endl;
//             } else {
//                 path = pathfinder.randomMovement(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
//             }
//         } else if (color == 2 || color == 3) {
//             if (randomNumber <= 9) {
//                 path = pathfinder.dijkstra(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: Dijkstra" << std::endl;
//             } else {
//                 path = pathfinder.randomMovement(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
//             }
//         }
//
//         playersList[currentPlayer].setPowerUpActive(false);
//         playersList[currentPlayer].erasePowerUp();
//     } else {
//         if (color == 0 || color == 1) {
//             // Tanque seleccionado: Rojo o amarillo
//             // 50% de probabilidad BFS o 50% movimiento aleatorio
//             if (randomNumber <= 5) {
//                 // 50% de probabilidad BFS
//                 path = pathfinder.bfs(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: BFS" << std::endl;
//             } else {
//                 // 50% de probabilidad movimiento aleatorio
//                 path = pathfinder.randomMovement(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
//             }
//         } else if (color == 2 || color == 3) {
//             // Tanque seleccionado: Celeste o azul
//             // 80% de probabilidad Dijkstra o 20% movimiento aleatorio
//             if (randomNumber <= 8) {
//                 // 80% de probabilidad Dijkstra
//                 path = pathfinder.dijkstra(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: Dijkstra" << std::endl;
//             } else {
//                 // 20% de probabilidad para Acción D
//                 path = pathfinder.randomMovement(startId, goalId);
//                 std::cout << "Color:" << color << " Algoritmo usado: movimiento aleatorio" << std::endl;
//             }
//         }
//     }
//     if (path.size() < 2) {
//         tank->setSelected(false);
//         update();
//         return;
//     }
//
//     // Decrementar acciones restantes
//     actionsRemaining--;
//
//     // Verificar si se debe cambiar de turno
//     if (actionsRemaining <= 0) {
//         endTurn();
//     }
//
//     // Reproducir efecto de sonido de movimiento en bucle
//     moveSoundChannel = soundManager.playSoundEffect("move", -1);
//
//     // Crear la estructura de datos para el movimiento
//     auto* moveData = new MoveData{const_cast<View*>(this), tank, path, 1};
//
//     // Iniciar el temporizador para mover el tanque
//     g_timeout_add(100, View::moveTankStep, moveData);
// }

bool View::BulletHitWall(const Bullet* bullet) const {
    if (auto [row, col] = bullet->getPosition();
    gridMapGame->isObstacle(row, col)) {
        return true;
    }
    return false;
}

void View::deselectAllTanks() const {
    for (int i = 0; i < 8; i++) {
        tanksList[i].setSelected(false);
    }
}

void View::destroyBullet() {
    if (bulletGame) {
        delete bulletGame;
        bulletGame = nullptr;
    }
}

void View::destroyBulletTrace(){
    if (bulletTrace) {
        delete bulletTrace;
        bulletTrace = nullptr;
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
        view->playersList[i].generatePowerUp();
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

    if (const Player& player = playersList[currentPlayer];
        player.getPowerUpActive() && player.getPowerUp() == DOUBLE_TURN) {
        actionsRemaining = 2;
        playersList[currentPlayer].setPowerUpActive(false);
        playersList[currentPlayer].erasePowerUp();
    } else {
        actionsRemaining = 1;
    }

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
        Tank& tank = tanksList[i];
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
        Tank& tank = tanksList[i];
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



