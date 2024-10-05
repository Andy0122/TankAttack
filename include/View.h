#pragma once

#include <map>
#include <gtk/gtk.h>
#include <string>

#include "entities/Bullet.h"
#include "entities/Tank.h"
#include "systems/GridGraph.h"


/**
 * @brief View class
 */
class View {
public:
    /**
     * @brief View Constructor.
     *
     * @param window GtkWidget* Window to display the game.
     */
    explicit View(GtkWidget* window);

    /**
     * @brief Sets the map of the game.
     *
     * @param map GridGraph* Map of the game.
     */
    void setGridMap(GridGraph* map);

    /**
    * @brief Sets the tanks of the game.
    *
    * @param tanks The tank array.
    */
    void setTanks(Tank* tanks);

    /**
     * @brief Updates the game interface.
     */
    void update() const;

private:
    GtkWidget* drawingArea = nullptr; ///< Drawing area of the game
    Tank* tanks = nullptr; ///< Array of tanks of the game
    Bullet* bullet = nullptr; ///< Bullet of the game
    GtkWidget* statusBar = nullptr; ///< Status bar of the game
    GridGraph* gridMap = nullptr; ///< Map of the game
    std::map<std::string, GdkPixbuf*> assets; ///< Assets of the game

    /**
     * @brief Creates a vertical box.
     *
     * @param window GtkWidget* Window.
     * @return GtkWidget* Vertical box.
     */
    static GtkWidget* createVBox(GtkWidget* window);

    /**
     * @brief Creates a horizontal box.
     *
     * @param vbox GtkWidget* Vertical box.
     * @return GtkWidget* Horizontal box.
     */
    static GtkWidget* createHBox(GtkWidget* vbox);

    /**
     * @brief Creates the drawing area.
     *
     * @param hbox GtkWidget* Horizontal box.
     */
    void createDrawingArea(GtkWidget* hbox);

    /**
     * @brief Creates the status bar.
     *
     * @param vbox GtkWidget* Vertical box.
     */
    void createStatusBar(GtkWidget* vbox);

    /**
     * @brief Creates the player label.
     *
     * @param player int Player.
     * @return GtkWidget* Player label.
     */
    [[nodiscard]] static GtkWidget* createPlayerLabel(int player);

    /**
     * @brief Creates the player box.
     *
     * @param player int Player.
     * @return GtkWidget* Player box.
     */
    [[nodiscard]] GtkWidget* createPlayerBox(int player) const;

    /**
     * @brief Creates the tank box.
     *
     * @param player int Player.
     * @param col int Column.
     * @return GtkWidget* Tank box.
     */
    [[nodiscard]] GtkWidget* createTankBox(int player, int col) const;

    /**
     * @brief Creates the tank display.
     *
     * @param tank Tank Tank.
     * @return GtkWidget* Tank display.
     */
    [[nodiscard]] GtkWidget* createTankDisplay(const Tank& tank) const;

    /**
     * @brief Updates the status bar.
     */
    void updateStatusBar() const;

    /**
     * @brief Loads the assets of the game.
     */
    void loadAssets();

    /**
     * @brief Connects the signals of the game.
     */
    void connectSignals();

    /**
     * @brief Draws the map of the game.
     *
     * @param cr cairo_t* Cairo context.
     */
    void drawMap(cairo_t* cr);

    /**
     * @brief Draws the tanks of the game.
     *
     * @param cr cairo_t* Cairo context.
     */
    void drawTanks(cairo_t* cr);

    /**
     * @brief Draws the bullet of the game.
     *
     * @param cr cairo_t* Cairo context.
     */
    void drawBullet(cairo_t* cr) const;

    /**
     * @brief Draws the path the game interface.
     *
     * @param widget GtkWidget* Widget.
     * @param cr cairo_t* Cairo context.
     * @param data gpointer Data.
     */
    static gboolean onDraw(GtkWidget* widget, cairo_t* cr, gpointer data);

    /**
     * @brief Handles the click event.
     *
     * @param widget GtkWidget* Widget.
     * @param event GdkEventButton* Event.
     * @param data gpointer Data.
     */
    static gboolean onClick(GtkWidget* widget, const GdkEventButton* event, gpointer data);

    /**
     * @brief Handles the move of the bullet.
     *
     * @param data gpointer Data.
     * @return gboolean True if the bullet is moving.
     */
    static gboolean handleMoveBullet(gpointer data);

    /**
    * @brief Gets the position of the cell clicked.
    *
    * @param position Position where the click happened.
    */
    static bool cellClicked(Position position);

    /**
     * @brief Gets the clicked tank.
     *
     * @param position Position Position.
     * @return Tank* Clicked tank.
     */
    [[nodiscard]] Tank* getClickedTank(Position position) const;

    /**
     * @brief Gets the selected tank.
     *
     * @return Tank* Selected tank.
     */
    [[nodiscard]] Tank* getSelectedTank() const;

    /**
     * @brief Handles the selection of a tank.
     *
     * @param tank Tank* Tank.
     */
    void handleSelectTank(Tank* tank) const;

    /**
     * @brief Deselects all tanks.
     */
    void deselectAllTanks() const;

    /**
     * @brief Handles the movement of a tank.
     *
     * @param tank Tank* Tank.
     * @param position Position Position.
     */
    void handleMoveTank(Tank* tank, Position position) const;

    /**
     * @brief Handles the fire of a bullet.
     *
     * @param origin Position Origin.
     * @param target Position Target.
     */
    void handleFireBullet(const Position& origin, const Position& target);

    static constexpr int CELL_SIZE = 50; ///< Size of the cell
    static constexpr int ROWS = 11; ///< Number of rows
    static constexpr int COLS = 21; ///< Number of columns
    static constexpr int X_OFFSET = 30; ///< X offset
    static constexpr int Y_OFFSET = 30; ///< Y offset
};
