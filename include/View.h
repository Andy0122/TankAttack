#pragma once

#include <map>
#include <gtk/gtk.h>
#include <string>

#include "entities/Tank.h"
#include "systems/GridGraph.h"


/**
 * @brief View class
 */
class View {
public:
    /**
     * @brief View Constructor
     * @param window GtkWidget* Window to display the game
     */
    explicit View(GtkWidget* window);

    /**
     * @brief Sets the map of the game
     * @param map GridGraph* Map of the game
     */
    void setGridMap(GridGraph* map);

    void setTank(Tank* tank);

    /**
     * @brief Updates the game interface
     */
    void update() const;

private:
    GtkWidget* drawingArea; ///< Drawing area of the game
    Tank* tank; ///< Temp tank of the game
    GtkWidget* statusBar; ///< Status bar of the game
    GridGraph* gridMap = nullptr; ///< Map of the game
    std::map<std::string, GdkPixbuf*> assets; ///< Assets of the game

    /**
     * @brief Creates a vertical box
     * @param window GtkWidget* Window
     * @return GtkWidget* Vertical box
     */
    static GtkWidget* createVBox(GtkWidget* window);

    /**
     * @brief Creates a horizontal box
     * @param vbox GtkWidget* Vertical box
     * @return GtkWidget* Horizontal box
     */
    static GtkWidget* createHBox(GtkWidget* vbox);

    /**
     * @brief Creates the drawing area
     * @param hbox GtkWidget* Horizontal box
     */
    void createDrawingArea(GtkWidget* hbox);

    /**
     * @brief Creates the status bar
     * @param vbox GtkWidget* Vertical box
     */
    void createStatusBar(GtkWidget* vbox);

    /**
     * @brief Loads the assets of the game
     */
    void loadAssets();

    /**
     * @brief Connects the signals of the game
     */
    void connectSignals();

    /**
     * @brief Draws the map of the game
     * @param cr cairo_t* Cairo context
     */
    void drawMap(cairo_t* cr);

    void drawTank(cairo_t* cr);

    /**
     * @brief Draws the path the game interface
     * @param widget GtkWidget* Widget
     * @param cr cairo_t* Cairo context
     * @param data gpointer Data
     */
    static gboolean onDraw(GtkWidget* widget, cairo_t* cr, gpointer data);

    /**
     * @brief Handles the click event
     * @param widget GtkWidget* Widget
     * @param event GdkEventButton* Event
     * @param data gpointer Data
     */
    static gboolean onClick(GtkWidget* widget, const GdkEventButton* event, gpointer data);

    static bool cellClicked(int row, int column);

    [[nodiscard]] bool tankClicked(int row, int column) const;

    /**
     * @brief Handles the selection of a tank
     * @param tank Tank* Tank
     */
    static void handleSelectTank(Tank* tank);

    void handleMoveTank(Tank* tank, int row, int column) const;

    static constexpr int CELL_SIZE = 50; ///< Size of the cell
    static constexpr int ROWS = 11; ///< Number of rows
    static constexpr int COLS = 21; ///< Number of columns
    static constexpr int X_OFFSET = 30; ///< X offset
    static constexpr int Y_OFFSET = 30; ///< Y offset
};
