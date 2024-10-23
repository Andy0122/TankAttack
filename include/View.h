#pragma once

#include <map>
#include <string>
#include <gtk/gtk.h>

#include "data_structures/LinkedList.h"
#include "entities/Bullet.h"
#include "entities/Tank.h"
#include "systems/GridGraph.h"
#include "systems/SoundManager.h"

struct Explosion;
/**
 * @brief Clase que maneja la vista del juego.
 */
class View {
public:
    // Constructor
    /**
     * @brief Constructor de la clase View.
     *
     * @param window GtkWidget* Ventana principal para mostrar el juego.
     */
    explicit View(GtkWidget* window);

    // Métodos públicos
    /**
     * @brief Establece el mapa del juego.
     *
     * @param map Puntero al mapa de tipo GridGraph.
     */
    void setGridMap(GridGraph* map);

    /**
     * @brief Establece los tanques del juego.
     *
     * @param tanks Arreglo de tanques.
     */
    void setTanks(Tank* tanks);

    void setPlayers(Player* players);

    /**
     * @brief Actualiza la interfaz gráfica del juego.
     */
    void update() const;

private:
    // Variables miembro
    // Estado del juego
    Player* players = nullptr;            ///< Arreglo de jugadores del juego
    Tank* tanks = nullptr;               ///< Arreglo de tanques del juego
    Bullet* bullet = nullptr;            ///< Bala actual en el juego
    DATA_STRUCTURES::LinkedList* bulletTrace = nullptr;     ///< Rastreo del movimiento de la bala
    GridGraph* gridMap = nullptr;        ///< Mapa del juego
    bool gameOver = false;               ///< Indicador de fin del juego

    // Elementos de la interfaz de usuario
    GtkWidget* window = nullptr;         ///< Ventana principal del juego
    GtkWidget* drawingArea = nullptr;    ///< Área de dibujo del juego
    GtkWidget* statusBar = nullptr;      ///< Barra de estado del juego
    GtkWidget* timerLabel = nullptr;     ///< Etiqueta para mostrar el temporizador
    GtkWidget* playerLabels[2];          ///< Arreglo de etiquetas para los jugadores
    GtkWidget* powerUpLabels[2];    ///< Etiqueta para mostrar el poder obtenido
    std::map<std::string, GdkPixbuf*> assets; ///< Recursos gráficos del juego
    std::vector<Explosion> explosions; ///< Lista de explosiones en curso

    // Variables de mecánicas del juego
    int remaining_time = 300;            ///< Tiempo restante en segundos (5 minutos)
    int currentPlayer = 0;               ///< Jugador actual (0 para Jugador 1, 1 para Jugador 2)
    int actionsRemaining = 1;            ///< Acciones restantes en el turno actual

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

    // Gestión de turnos
    /**
     * @brief Finaliza el turno del jugador actual.
     */
    void endTurn();

    /**
     * @brief Establece el número de acciones por turno.
     *
     * @param actions Número de acciones por turno.
     */
    void setActionsPerTurn(int actions);

    // Métodos para finalizar el juego
    /**
     * @brief Finaliza el juego debido a que se agotó el tiempo.
     */
    void endGameDueToTime();

    /**
     * @brief Verifica si todos los tanques de un jugador han sido destruidos.
     *
     * @param player Índice del jugador.
     * @return True si todos los tanques han sido destruidos.
     */
    bool areAllTanksDestroyed(int player);

    /**
     * @brief Finaliza el juego debido a la destrucción de todos los tanques de un jugador.
     *
     * @param losingPlayer Índice del jugador que perdió.
     */
    void endGameDueToDestruction(int losingPlayer);

    /**
     * @brief Muestra un mensaje indicando el ganador.
     *
     * @param winner Índice del jugador ganador.
     */
    void showWinnerMessage(int winner);

    /**
     * @brief Muestra un mensaje indicando un empate.
     */
    void showTieMessage();

    /**
     * @brief Determina el ganador basado en los tanques restantes.
     *
     * @return Índice del jugador ganador, o -1 en caso de empate.
     */
    int determineWinner();

    // Métodos de interfaz de usuario
    /**
     * @brief Añade la posición actual de la bala al rastro.
     */
    void addTrace() const;

    /**
     * @brief Crea una caja vertical.
     *
     * @param window GtkWidget* Ventana.
     * @return GtkWidget* Caja vertical.
     */
    static GtkWidget* createVBox(GtkWidget* window);

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
    [[nodiscard]] GtkWidget* createPlayerLabel(int player);

    /**
     * @brief Actualiza las etiquetas de los jugadores para reflejar el turno actual.
     */
    void updatePlayerLabels();

    /**
     * @brief Crea la caja de información del jugador.
     *
     * @param player Índice del jugador.
     * @return GtkWidget* Caja del jugador.
     */
    [[nodiscard]] GtkWidget* createPlayerBox(int player) const;

    /**
     * @brief Crea la caja de tanques para un jugador.
     *
     * @param player Índice del jugador.
     * @param col Índice de la columna.
     * @return GtkWidget* Caja de tanques.
     */
    [[nodiscard]] GtkWidget* createTankBox(int player, int col) const;

    /**
     * @brief Crea el widget de visualización del tanque.
     *
     * @param tank Referencia al tanque.
     * @return GtkWidget* Visualización del tanque.
     */
    [[nodiscard]] GtkWidget* createTankDisplay(const Tank& tank) const;

    [[nodiscard]] GtkWidget* createPowerUpLabel(int player);

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

    /**
     * @brief Dibuja los tanques del juego.
     *
     * @param cr cairo_t* Contexto de Cairo.
     */
    void drawTanks(cairo_t* cr);

    /**
     * @brief Actualiza la barra de estado.
     */
    void updateStatusBar();

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

    void handlePowerUpActivation();

    /**
     * @brief Maneja la selección de un tanque.
     *
     * @param tank Puntero al tanque seleccionado.
     */
    void handleSelectTank(Tank* tank) const;

    /**
     * @brief Maneja el movimiento de un tanque.
     *
     * @param tank Puntero al tanque.
     * @param position Posición a la que se moverá.
     */
    void handleMoveTank(Tank* tank, Position position);

    /**
     * @brief Maneja el disparo de una bala.
     *
     * @param origin Posición de origen.
     * @param target Posición objetivo.
     */
    void handleFireBullet(const Position& origin, const Position& target);

    void createBullet(const Position& origin, const Position& target, POWER_UP powerUp);

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
    static void handleBulletBounce(Bullet* bullet);

    // Métodos de datos del juego
    /**
     * @brief Obtiene el tanque en una posición dada.
     *
     * @param position Posición a verificar.
     * @return Puntero al tanque en esa posición, o nullptr si no hay ninguno.
     */
    [[nodiscard]] Tank* getTankOnPosition(Position position) const;

    /**
     * @brief Obtiene el tanque seleccionado.
     *
     * @return Puntero al tanque seleccionado, o nullptr si no hay ninguno.
     */
    [[nodiscard]] Tank* getSelectedTank() const;

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
    bool bulletHitTank(const Bullet* bullet) const;

    /**
     * @brief Verifica si la bala ha golpeado una pared.
     *
     * @param bullet Puntero a la bala.
     * @return True si la bala golpeó una pared.
     */
    bool BulletHitWall(const Bullet* bullet) const;

    // Métodos de movimiento
    /**
     * @brief Mueve el tanque a una nueva posición.
     *
     * @param tank Puntero al tanque.
     * @param position Nueva posición.
     */
    void MoveTank(Tank* tank, Position position) const;

    /**
     * @brief Función de paso para mover el tanque a lo largo de una ruta.
     *
     * @param data Puntero a datos adicionales.
     * @return gboolean TRUE para continuar moviéndose.
     */
    static gboolean moveTankStep(gpointer data);

    // Métodos de reinicio
    /**
     * @brief Deselecciona todos los tanques.
     */
    void deselectAllTanks() const;

    /**
     * @brief Destruye la bala actual.
     */
    void destroyBullet();

    /**
     * @brief Destruye el rastro de la bala.
     */
    void destroyBulletTrace();

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

    static gboolean grantPowerUp(gpointer data);

    //Sounds
    SoundManager soundManager;  ///< Administrador de sonidos
    int moveSoundChannel = -1;  ///< Canal de sonido para el movimiento
    void initSound();           ///< Inicializa los sonidos
};

// Estructura para los datos de movimiento del tanque
struct MoveData {
    View* view;                ///< Puntero a la vista
    Tank* tank;                ///< Puntero al tanque
    std::vector<int> path;     ///< Ruta a seguir
    std::size_t currentStep;   ///< Paso actual en la ruta
};

struct Explosion {
 Position position;  ///< Posición de la explosión
 int currentFrame;   ///< Cuadro actual de la animación
};
