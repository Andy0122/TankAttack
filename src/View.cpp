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
            if (!node.accessible) {
                pixbuf = assets["obstacle"];
            } else {
                pixbuf = assets["cell"];
            }

            gdk_cairo_set_source_pixbuf(cr, pixbuf, col * CELL_SIZE, row * CELL_SIZE);
            cairo_paint(cr);
        }
    }
}

gboolean View::onDraw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    const auto view = static_cast<View*>(data);

    view->drawMap(cr);

    return FALSE;
}

gboolean View::onClick(GtkWidget *widget, const GdkEventButton *event, gpointer data) {
    const auto* view = static_cast<View*>(data);

    const int col = event->x / CELL_SIZE;
    int row = event->y / CELL_SIZE;

    if (col >= 0 && col < COLS && row >= 0 && row < ROWS) {
        const int id = row * COLS + col;  // ID del cuadro clickeado
        g_print("Se hizo clic en el cuadro con ID: %d\n", id);
    }

    return TRUE;
}