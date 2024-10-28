#pragma once

#include <Controller.h>
#include <map>
#include <string>
#include <gtk/gtk.h>
#include "entities/Bullet.h"
#include "entities/Tank.h"
#include "systems/GridGraph.h"
#include "systems/SoundManager.h"
#include "data_structures/LinkedList.h"


struct Explosion;
enum class AssetType {
 Cell,
 Obstacle,
 YellowTank,
 RedTank,
 BlueTank,
 CianTank,
 Bullet,
 Explosion1,
 Explosion2,
 Explosion3,
 Explosion4,
 Explosion5,
 Explosion6,
 Explosion7,
 COUNT // Para obtener el número total de activos
};
/**
 * @brief Clase que maneja la vista del juego.
 */
class View {
public:
    // Constructor
    /**
     * @brief Constructor de la clase View.
     *
     * @param controller Controller of the game.
     * @param window GtkWidget* Ventana principal para mostrar el juego.
     */
    explicit View(Controller* controller, GtkWidget* window);

    /**
     * @brief Actualiza la interfaz gráfica del juego.
     */
    void update() const;

private:
    Controller* controller;     ///< Controller of the game

    // Elementos de la interfaz de usuario
    GtkWidget* window = nullptr;         ///< Ventana principal del juego
    GtkWidget* drawingArea = nullptr;    ///< Área de dibujo del juego
    GtkWidget* statusBar = nullptr;      ///< Barra de estado del juego
    GtkWidget* timerLabel = nullptr;     ///< Etiqueta para mostrar el temporizador
    GdkPixbuf* assets[static_cast<int>(AssetType::COUNT)] = {nullptr};  ///< Recursos gráficos del juego
    DATA_STRUCTURES::LinkedList<Explosion> explosions; ///< Lista de explosiones en curso

    // Constantes
    static constexpr int CELL_SIZE = 50;             ///< Tamaño de cada celda
    static constexpr int ROWS = 13;                  ///< Número de filas
    static constexpr int COLS = 25;                  ///< Número de columnas
    static constexpr int X_OFFSET = 30;              ///< Desplazamiento en X
    static constexpr int Y_OFFSET = 30;              ///< Desplazamiento en Y
    static constexpr float TRACE_SIZE = CELL_SIZE * 0.25f; ///< Tamaño del rastro de la bala

    // Métodos privados

    // Métodos de temporizador
    /**
     * @brief Inicia el temporizador del juego.
     */
    void startTimer();

    /**
     * @brief Actualiza el temporizador del juego.
     *
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE para continuar el temporizador.
     */
    static gboolean updateTimer(gpointer data);

    /**
     * @brief Finaliza el juego debido a que se agotó el tiempo.
     */
    void endGameDueToTime();

    /**
     * @brief Finaliza el juego debido a la destrucción de todos los tanques de un jugador.
     *
     * @param losingPlayer Índice del jugador que perdió.
     */
    void endGameDueToDestruction(const Player* losingPlayer);

    /**
     * @brief Muestra un mensaje indicando el ganador.
     *
     * @param winner Índice del jugador ganador.
     */
    void showWinnerMessage(int winner) const;

    /**
     * @brief Muestra un mensaje indicando un empate.
     */
    void showTieMessage() const;

    /**
     * @brief Crea una caja vertical.
     *
     * @param window GtkWidget* Ventana.
     * @return GtkWidget* Caja vertical.
     */
    static GtkWidget* createVBox(GtkWidget* window);

    void createTimerLabel(GtkWidget* vbox);

    /**
     * @brief Crea una caja horizontal.
     *
     * @param vbox GtkWidget* Caja vertical.
     * @return GtkWidget* Caja horizontal.
     */
    static GtkWidget* createHBox(GtkWidget* vbox);

    /**
     * @brief Crea el área de dibujo.
     *
     * @param hbox GtkWidget* Caja horizontal.
     */
    void createDrawingArea(GtkWidget* hbox);

    /**
     * @brief Crea la barra de estado.
     *
     * @param vbox GtkWidget* Caja vertical.
     */
    void createStatusBar(GtkWidget* vbox);

    /**
     * @brief Crea la etiqueta del jugador.
     *
     * @param player Índice del jugador.
     * @return GtkWidget* Etiqueta del jugador.
     */
    [[nodiscard]] GtkWidget* createPlayerLabel(int playerId) const;

    /**
     * @brief Crea la caja de información del jugador.
     *
     * @param playerId Índice del jugador.
     * @return GtkWidget* Caja del jugador.
     */
    [[nodiscard]] GtkWidget* createPlayerBox(int playerId);

    /**
     * @brief Crea la caja de tanques para un jugador.
     *
     * @param playerId Índice del jugador.
     * @param col Índice de la columna.
     * @return GtkWidget* Caja de tanques.
     */
    [[nodiscard]] GtkWidget* createTankBox(int playerId, int col);

    /**
     * @brief Crea el widget de visualización del tanque.
     *
     * @param tank Referencia al tanque.
     * @return GtkWidget* Visualización del tanque.
     */
    [[nodiscard]] GtkWidget* createTankDisplay(const Tank* tank);

    [[nodiscard]] GtkWidget* createPowerUpLabel(int playerId) const;

    // Métodos de configuración
    /**
     * @brief Carga los recursos gráficos del juego.
     */
    void loadAssets();

    /**
     * @brief Conecta las señales del juego.
     */
    void connectSignals();

    // Métodos de dibujo
    /**
     * @brief Dibuja la interfaz del juego.
     *
     * @param widget GtkWidget* Widget.
     * @param cr cairo_t* Contexto de Cairo.
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE para continuar dibujando.
     */
    static gboolean onDraw(GtkWidget* widget, cairo_t* cr, gpointer data);

    /**
     * @brief Dibuja el mapa del juego.
     *
     * @param cr cairo_t* Contexto de Cairo.
     */
    void drawMap(cairo_t* cr);

    GdkPixbuf* selectCellImage(const Node& node);

    /**
     * @brief Dibuja los tanques del juego.
     *
     * @param cr cairo_t* Contexto de Cairo.
     */
    void drawTanks(cairo_t* cr);

    void drawTankPath(cairo_t* cr) const;

    GdkPixbuf* selectTankImage(Color color);

    static GdkPixbuf* rotateImage(const GdkPixbuf* image, double rotationAngle);

    static void drawSelectedMarker(cairo_t* cr, const Tank& tank) ;

    /**
     * @brief Actualiza la barra de estado.
     */
    void drawStatusBar();

    /**
     * @brief Dibuja la bala del juego.
     *
     * @param cr cairo_t* Contexto de Cairo.
     */
    void drawBullet(cairo_t* cr) const;

    /**
     * @brief Dibuja el rastro de la bala.
     *
     * @param cr cairo_t* Contexto de Cairo.
     */
    void drawBulletTrace(cairo_t* cr) const;

    // Métodos de eventos
    /**
     * @brief Maneja el evento de clic.
     *
     * @param widget GtkWidget* Widget.
     * @param event GdkEventButton* Evento.
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE si el evento fue manejado.
     */
    static gboolean onClick(GtkWidget* widget, const GdkEventButton* event, gpointer data);

    static gboolean onKeyPress(GtkWidget* widget, GdkEventKey* event, gpointer data);

    /**
     * @brief Maneja el movimiento de la bala.
     *
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE si la bala sigue en movimiento.
     */
    static gboolean handleMoveBullet(gpointer data);

    /**
     * @brief Maneja el rebote de la bala.
     *
     * @param bullet Puntero a la bala.
     */
    // static void handleBulletBounce(Bullet* bullet);

    /**
     * @brief Verifica si la posición clicada está dentro de la cuadrícula.
     *
     * @param position Posición donde ocurrió el clic.
     * @return True si la celda está dentro de los límites.
     */
    static bool cellClicked(Position position);

    /**
     * @brief Verifica si la bala ha golpeado un tanque.
     *
     * @param bullet Puntero a la bala.
     * @return True si la bala golpeó un tanque.
     */
    // bool bulletHitTank(const Bullet* bullet) const;

    /**
     * @brief Verifica si la bala ha golpeado una pared.
     *
     * @param bullet Puntero a la bala.
     * @return True si la bala golpeó una pared.
     */
    // bool BulletHitWall(const Bullet* bullet) const;

    // Métodos de movimiento
    /**
     * @brief Función de paso para mover el tanque a lo largo de una ruta.
     *
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE para continuar moviéndose.
     */
    static gboolean moveTankStep(gpointer data);

    static void setTankRotationAngle(Tank* tank, int destRow, int destCol);

    static gboolean moveBulletStep(gpointer data);

    static void setBulletRotationAngle(Bullet* bullet, int destRow, int destCol);

    static bool tankKilled(const Tank* tank);

    static void startExplosion(View* view, Position position);

    // Explociones
    /**
   * @brief Anima las explosiones en curso.
   *
   * @param data Puntero a la instancia de View.
   * @return gboolean TRUE si hay explosiones activas.
   */
    static gboolean animateExplosions(gpointer data);

    /**
   * @brief Dibuja las explosiones en curso.
   *
   * @param cr cairo_t* Contexto de Cairo.
   */
    void drawExplosions(cairo_t* cr);

    static gboolean grantPowerUps(gpointer data);

    //Sounds
    SoundManager soundManager;  ///< Administrador de sonidos
    int moveSoundChannel = -1;  ///< Canal de sonido para el movimiento
    void initSound();           ///< Inicializa los sonidos
};

// Estructura para los datos de movimiento del tanque
struct MoveData {
    View* view;                ///< Puntero a la vista
    Tank* tank;                ///< Puntero al tanque
    DATA_STRUCTURES::LinkedList<Position>* path;     ///< Ruta a seguir
    int currentStep = 0;   ///< Paso actual en la ruta
};

struct Explosion {
 Position position;  ///< Posición de la explosión
 int currentFrame;   ///< Cuadro actual de la animación
};
