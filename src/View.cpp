#include "View.h"
#include "systems/Pathfinder.h"
#include <chrono>
#include <thread>
#include <random>
#include <cmath>

#include "data_structures/LinkedList.h"
#include "systems/SoundManager.h"

using namespace std;
using namespace DATA_STRUCTURES;

// TODO: Add boucing bullet

LinkedList<Position>* queueToLinkedList(Queue<Position>* queue) {
    auto* list = new LinkedList<Position>();

    while (!queue->empty()) {
        list->append(queue->front());
        queue->pop();
    }

    return list;
}

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

void View::update() const {
    gtk_widget_queue_draw(drawingArea);
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
    view->drawTankPath(cr);
    view->drawTanks(cr);
    view->drawStatusBar();
    view->drawBulletTrace(cr);
    view->drawBullet(cr);
    view->drawExplosions(cr);

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

void View::drawTankPath(cairo_t *cr) const {
    LinkedList<Position>* trace = controller->getTankPath();
    if (trace == nullptr) {
        return;
    }

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    for (int i = 0; i < trace->size(); i++) {
        const auto [row, col] = trace->at(i);

        const double x = col * CELL_SIZE + CELL_SIZE / 2;
        const double y = row * CELL_SIZE + CELL_SIZE / 2;
        cairo_rectangle(cr, x - 5, y - 5, 10, 10);
        cairo_fill(cr);
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
        GdkPixbuf* rotatedTankImage = rotateImage(tankImage, tank->getRotationAngle());

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

GdkPixbuf* View::rotateImage(const GdkPixbuf* image, const double rotationAngle) {
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

GtkWidget* View::createPowerUpLabel(const int playerId) const {
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
        if (const LinkedList<Position>* trace = controller->getBulletPath();
            trace != nullptr && !trace->empty()) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

            for (int i = 0; i < trace->size(); i++) {
                const auto [row, column] = trace->at(i);

                const double x = column * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
                const double y = row * CELL_SIZE + (CELL_SIZE - TRACE_SIZE) / 2;
                cairo_rectangle(cr, x, y, TRACE_SIZE, TRACE_SIZE);
                cairo_fill(cr);
            }
        }
    }
}

void View::drawBullet(cairo_t *cr) const { // Add the bullet orientation
    if (const Bullet* bullet = controller->getBullet()) {
        auto [row, column] = bullet->getPosition();

        // Get the bullet image
        const GdkPixbuf* bulletImage = assets.at("bullet");

        // Rotate the bullet image
        GdkPixbuf* rotatedBulletImage = rotateImage(bulletImage, bullet->getRotationAngle());

        // Draw the bullet
        gdk_cairo_set_source_pixbuf(cr, rotatedBulletImage, column * CELL_SIZE + CELL_SIZE / 4, row * CELL_SIZE + CELL_SIZE / 4);
        cairo_paint(cr);

        // Free the GdkPixbuf
        g_object_unref(rotatedBulletImage);
    }
}

void View::drawExplosions(cairo_t *cr) {
    for (const auto& explosion : explosions) {
        std::string key = "explosion_" + std::to_string(explosion.currentFrame + 1);
        const GdkPixbuf* pixbuf = assets[key];

        const int row = explosion.position.row;
        const int column = explosion.position.column;

        gdk_cairo_set_source_pixbuf(cr, pixbuf, column * CELL_SIZE, row * CELL_SIZE);
        cairo_paint(cr);
    }
}


gboolean View::onClick(GtkWidget* widget, const GdkEventButton* event, gpointer data) {
    auto* view = static_cast<View*>(data);
    const auto* controller = view->controller;

    if (controller->getGameOver()) {
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

                LinkedList<Position>* path = controller->getTankPath();
                auto* moveData = new MoveData{view, selectedTank, path};
                view->moveSoundChannel = view->soundManager.playSoundEffect("move", -1);
                g_timeout_add(100, moveTankStep, moveData);
            }
        }

    } else if (event->button == 3) {
        if (Tank* selectedTank = controller->getSelectedTank()) {
            if (selectedTank->isDestroyed()) {
                return TRUE;
            }
            controller->handleFireBullet(Position(selectedTank->getRow(), selectedTank->getColumn()), position);

            LinkedList<Position>* path = controller->getBulletPath();
            auto* moveData = new MoveData{view, selectedTank, path, 0};
            view->soundManager.playSoundEffect("fire");
            g_timeout_add(500, moveBulletStep, moveData);
            selectedTank->setSelected(false);
        }
    }
    view->update();

    return TRUE;
}

gboolean View::moveTankStep(gpointer data) {
    auto* moveData = static_cast<MoveData*>(data);
    View* view = moveData->view;
    Tank* tank = moveData->tank;
    const int currentStep = moveData->currentStep;
    const LinkedList<Position>* path = moveData->path;

    if (path->size() == currentStep) {
        tank->setSelected(false);

        // Stop Sound Effect
        if (view->moveSoundChannel != -1) {
            view->soundManager.stopSoundEffect("move");
            view->moveSoundChannel = -1;
        }

        view->controller->destroyTankPath();
        delete moveData;

        view->update();
        return FALSE;
    }

    auto [row, col] = path->at(currentStep);

    setTankRotationAngle(tank, row, col);

    const auto newPosition = Position(row, col);
    view->controller->moveTank(tank, newPosition);

    moveData->currentStep++;
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

gboolean View::moveBulletStep(gpointer data) {
    auto* moveData = static_cast<MoveData*>(data);
    auto* view = moveData->view;
    const int currentStep = moveData->currentStep;
    const auto* controller = view->controller;
    const LinkedList<Position>* path = moveData->path;
    Bullet* bullet = controller->getBullet();

    if (path == nullptr) { // No path
        return FALSE;
    }

    if (bullet == nullptr) { // No bullet
        return FALSE;
    }

    if (controller->getGameOver()) { // Game over
        return FALSE;
    }

    if (path->size() == currentStep) { // Movement finished
        controller->destroyBullet();

        view->update();
        return FALSE;
    }

    auto [row, col] = path->at(currentStep);

    setBulletRotationAngle(bullet, row, col);

    // Move the bullet
    const auto newPosition = Position(row, col);
    Controller::moveBullet(bullet, newPosition);

    // Check collision with tank
    if (controller->bulletHitTank()) {
        controller->handleBulletCollision();
        view->soundManager.playSoundEffect("impact");

        if (Tank* tankHit = controller->getTankOnPosition(newPosition);
            tankKilled(tankHit)) {
            controller->handleTankDestruction(tankHit);
            startExplosion(view, Position{tankHit->getRow(), tankHit->getColumn()});

            if (const Player* playerHit = tankHit->getPlayer();
                controller->allTanksDestroyed(playerHit)) {
                view->endGameDueToDestruction(playerHit);
            }
        }
    }

    moveData->currentStep++;
    view->update();

    return TRUE;
}

void View::setBulletRotationAngle(Bullet* bullet, const int destRow, const int destCol) {
    const int currentRow = bullet->getPosition().row;
    const int currentCol = bullet->getPosition().column;

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

    bullet->setRotationAngle(rotationAngle);
}

bool View::tankKilled(const Tank* tank) {
    return tank->getHealth() <= 0 && !tank->isDestroyed();
}

void View::startExplosion(View* view, const Position position) {
    const auto explosion = Explosion{position, 0};
    view->explosions.push_back(explosion);

    view->soundManager.playSoundEffect("explosion");

    g_timeout_add(100, animateExplosions, view);
}

gboolean View::animateExplosions(gpointer data) {
    auto* view = static_cast<View*>(data);
    bool anyActive = false;

    for (auto it = view->explosions.begin(); it != view->explosions.end();) {
        it->currentFrame++;

        if (it->currentFrame >= 7) {
            it = view->explosions.erase(it);
        } else {
            anyActive = true;
            ++it;
        }
    }
    view->update();

    return anyActive ? TRUE : FALSE;
}

gboolean View::onKeyPress(GtkWidget* widget, GdkEventKey* event, gpointer data) {
    const auto* view = static_cast<View*>(data);
    const auto* controller = view->controller;
    if (event->keyval == GDK_KEY_Shift_L || event->keyval == GDK_KEY_Shift_R) {
        controller->handlePowerUpActivation();
        view->update();
    }

    return FALSE;
}

// void View::handleBulletBounce(Bullet* bullet) {
//     auto [x, y] = bullet->getDirection();
//     bullet->setDirection(Direction(-x, -y));
// }

bool View::cellClicked(const Position position) {
    return position.row >= 0 && position.row < ROWS && position.column >= 0 && position.column < COLS;
}

// bool View::BulletHitWall(const Bullet* bullet) const {
//     if (auto [row, col] = bullet->getPosition();
//     gridMapGame->isObstacle(row, col)) {
//         return true;
//     }
//     return false;
// }

void View::startTimer() {
    g_timeout_add_seconds(1, updateTimer, this);
    g_timeout_add_seconds(20, grantPowerUps, this);
}

gboolean View::grantPowerUps(gpointer data) {
    const auto* view = static_cast<View*>(data);
    const auto* controller = view->controller;

    controller->generatePowerUps();

    view->update();

    return TRUE;
}

gboolean View::updateTimer(gpointer data) {
    auto* view = static_cast<View*>(data);
    const auto* controller = view->controller;

    if (const int remainingTime = controller->getRemainingTime();
        remainingTime > 0) {
        // Decrease the remaining time
        controller->decreaseTime();

        // Format the time
        const int minutes = remainingTime / 60;
        const int seconds = remainingTime % 60;
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);

        // Update the timer label
        gtk_label_set_text(GTK_LABEL(view->timerLabel), buffer);

        return TRUE;
    }
    view->endGameDueToTime();
    return FALSE;
}

void View::endGameDueToTime() {
    if (controller->getGameOver()) return;
    controller->setGameOver(true);

    // Play sound effect
    soundManager.playSoundEffect("game_over");

    // Show a message dialog
    if (const Player* winner = controller->determineWinner();
        winner == nullptr) {
        showTieMessage();
    } else {
        showWinnerMessage(winner->getId());
    }
}

void View::endGameDueToDestruction(const Player* losingPlayer) {
    if (controller->getGameOver()) return;
    controller->setGameOver(true);

    // Play sound effect
    soundManager.stopBackgroundMusic();
    soundManager.playSoundEffect("game_over");

    // Show a message dialog
    const int winner = losingPlayer->getId() == 0 ? 1 : 0;
    showWinnerMessage(winner);
}

void View::showWinnerMessage(const int winner) const {
    const string message = "¡El jugador " + std::to_string(winner + 1) + " ha ganado!";

    // Show a dialog
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Close the window
    gtk_widget_destroy(window);

}

void View::showTieMessage() const {
    // Show a dialog
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "¡El juego ha terminado en empate!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Close the window
    gtk_widget_destroy(window);

}

void View::initSound() {
    // Play background music
    soundManager.playBackgroundMusic("../assets/sounds/background_music.mp3");

    // Load sound effects
    soundManager.loadSoundEffect("fire", "../assets/sounds/fire.wav");
    soundManager.loadSoundEffect("impact", "../assets/sounds/impact.wav");
    soundManager.loadSoundEffect("explosion", "../assets/sounds/explosion.wav");
    soundManager.loadSoundEffect("move", "../assets/sounds/move.wav");
    soundManager.loadSoundEffect("game_over", "../assets/sounds/game_over.wav");
}

