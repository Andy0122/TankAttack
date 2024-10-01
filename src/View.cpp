#include "View.h"

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
    statusBar = gtk_label_new("Status Bar");
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
}

void View::connectSignals() {
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(onDraw), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(onClick), this);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);
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

gboolean View::onDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    const auto view = static_cast<View*>(data);

    view->drawMap(cr);
    view->drawTanks(cr);

    return FALSE;
}

gboolean View::onClick(GtkWidget *widget, const GdkEventButton *event, gpointer data) {
    const auto* view = static_cast<View*>(data);

    const int row = event->y / CELL_SIZE; // Y position
    const int column = event->x / CELL_SIZE; // X position
    const auto position = Position(row, column);

    if (Tank* clickedTank = view->getClickedTank(position)) {
        handleSelectTank(clickedTank);
        g_print("Tank selected\n"); // For debugging purposes
    }

    else if (cellClicked(position)) {
        g_print("Cell (%d, %d) was clicked\n", row, column); // For debugging purposes
        g_print("Cell is %s\n\n", view->gridMap->isObstacle(row, column) ? "obstacle" : "accessible"); // For debugging purposes

        if (event->button == 1) {
            if (Tank* selectedTank = view->getSelectedTank()) {
                g_print("Tank was moved from (%d, %d) to (%d, %d)\n", selectedTank->getRow(), selectedTank->getColumn(), row, column); // For debugging purposes
                view->handleMoveTank(selectedTank, position);
            } else {
                g_print("Tank can't be moved because there's no selected tank\n"); // For debugging purposes
            }
        }
        //
        // if (event->button == 3 && view->tank->isSelected()) {
        //     // Attack
    }

    return TRUE;
}

bool View::cellClicked(const Position position) {
    return position.row >= 0 && position.row < ROWS && position.column >= 0 && position.column < COLS;
}

Tank* View::getClickedTank(const Position position) const {
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

void View::handleSelectTank(Tank* tank) {
    if (!tank->isSelected()) {
        tank->setSelected(true);
    }
}

void View::handleMoveTank(Tank* tank, const Position position) const {
    if (tank->isSelected()) {
        if (!gridMap->isObstacle(position.row, position.column)
            && !gridMap->isOccupied(position.row, position.column)) {
            gridMap->removeTank(tank->getRow(), tank->getColumn());
            tank->setPosition(position);
            gridMap->placeTank(position.row, position.column);
            tank->setSelected(false);
            update();
        }
    }
}
