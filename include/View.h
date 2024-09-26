#pragma once

#include <map>
#include <gtk/gtk.h>
#include <string>

#include "systems/GridGraph.h"


class View {
public:
    explicit View(GtkWidget* window);
    void setGridMap(GridGraph* map);
    void update() const;

private:
    GtkWidget* drawingArea;
    GtkWidget* statusBar;
    GridGraph* gridMap = nullptr;
    std::map<std::string, GdkPixbuf*> assets;

    void loadAssets();
    void drawMap(cairo_t* cr);
    static gboolean onDraw(GtkWidget* widget, cairo_t* cr, gpointer data);
    static gboolean onClick(GtkWidget* widget, const GdkEventButton* event, gpointer data);

    static constexpr int CELL_SIZE = 50;
    static constexpr int ROWS = 11;
    static constexpr int COLS = 21;
    static constexpr int X_OFFSET = 30;
    static constexpr int Y_OFFSET = 30;
};
