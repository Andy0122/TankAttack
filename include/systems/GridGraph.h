#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <vector>
#include <iostream>
#include "Position.h"

/**
 * @brief Clase que representa un nodo individual en la cuadrícula del grafo.
 */
class Node {
public:
    int id;            ///< Identificador único del nodo.
    bool obstacle;     ///< Indica si el nodo es un obstáculo (false si es accesible).
    bool occupied = false;   ///< Indica si el nodo está ocupado o no.

    /**
     * @brief Constructor del nodo.
     *
     * @param id Identificador único del nodo.
     * @param accessible Indica si el nodo es accesible (true por defecto).
     */
    Node(int id, bool accessible = true);
};

/**
 * @brief Clase que representa una cuadrícula de nodos (grafo) y sus conexiones.
 */
class GridGraph {
private:
    // Constantes
    static const int rows = 13; ///< Número predefinido de filas en la cuadrícula.
    static const int cols = 25; ///< Número predefinido de columnas en la cuadrícula.

    // Miembros privados
    std::vector<std::vector<Node>> grid;  ///< Matriz que contiene los nodos del grafo.
    std::vector<std::vector<int>> adjList; ///< Lista de adyacencia para representar conexiones entre nodos.
    std::vector<int> safeNodeIdsLeft; ///< Lista de IDs de nodos seguros de la zona izquierda.
    std::vector<int> safeNodeIdsRight; ///< Lista de IDs de nodos seguros de la zona derecha.

public:
    // Constructor
    /**
     * @brief Constructor de la clase GridGraph. Inicializa la cuadrícula de nodos.
     */
    GridGraph();

    // Métodos de conversión y acceso
    /**
     * @brief Convierte coordenadas 2D en un índice 1D en la cuadrícula.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @return Índice 1D correspondiente.
     */
    int toIndex(int row, int col) const;

    /**
     * @brief Devuelve el número de filas de la cuadrícula.
     *
     * @return Número de filas.
     */
    int getRows() const;

    /**
     * @brief Devuelve el número de columnas de la cuadrícula.
     *
     * @return Número de columnas.
     */
    int getCols() const;

    /**
     * @brief Devuelve una referencia constante a un nodo en la posición especificada.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @return Referencia constante al nodo.
     */
    const Node& getNode(int row, int col) const;

    /**
     * @brief Devuelve una referencia constante al nodo dado su ID.
     *
     * @param id Identificador único del nodo.
     * @return Referencia constante al nodo.
     */
    const Node& getNodeById(int id) const;

    /**
     * @brief Devuelve una referencia al nodo dado su ID.
     *
     * @param id Identificador único del nodo.
     * @return Referencia al nodo.
     */
    Node& getNodeById(int id);

    /**
     * @brief Obtiene una posición accesible aleatoria en la cuadrícula.
     *
     * @return Objeto Position con la fila y columna de la posición accesible.
     */
    Position getRandomAccessiblePosition() const;

    /**
     * @brief Metodo para obtener la lista de adyacencia del grafo.
     * @return Una referencia constante a la lista de adyacencia.
     */
    const std::vector<std::vector<int>>& getAdjList() const;

    // Métodos de modificación
    /**
     * @brief Conecta los nodos accesibles en la cuadrícula.
     */
    void connectNodes();

    /**
     * @brief Establece la accesibilidad de un nodo específico.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @param accessible Valor booleano que indica si el nodo es accesible.
     */
    void setNodeAccessibility(int row, int col, bool accessible);

    /**
     * @brief Genera obstáculos aleatorios en la cuadrícula, marcando nodos como inaccesibles.
     */
    void generateObstacles();

    /**
     * @brief Coloca un tanque en la posición especificada.
     *
     * @param row Fila donde colocar el tanque.
     * @param col Columna donde colocar el tanque.
     */
    void placeTank(int row, int col);

    /**
     * @brief Elimina un tanque de la posición especificada.
     *
     * @param row Fila de la que eliminar el tanque.
     * @param col Columna de la que eliminar el tanque.
     */
    void removeTank(int row, int col);

    // Métodos de verificación
    /**
     * @brief Verifica si un nodo específico es un obstáculo.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @return true Si el nodo es un obstáculo.
     * @return false Si el nodo es accesible.
     */
    bool isObstacle(int row, int col) const;

    /**
     * @brief Verifica si un nodo específico está ocupado.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @return true Si el nodo está ocupado.
     * @return false Si el nodo no está ocupado.
     */
    bool isOccupied(int row, int col) const;

    /**
     * @brief Verifica si un nodo específico es seguro.
     *
     * @param nodeId ID del nodo.
     * @return true Si el nodo es seguro.
     * @return false Si el nodo no es seguro.
     */
    bool isSafeNode(int nodeId) const;

    // Métodos de utilidad
    /**
     * @brief Imprime la representación del grafo en la consola.
     */
    void printGraph() const;

    /**
     * @brief Imprime la accesibilidad de los nodos en la cuadrícula.
     */
    void printAccessibility() const;

    // Métodos de aseguramiento
    /**
     * @brief Asegura que los nodos seguros estén conectados entre sí.
     */
    void ensureSafeNodesConnectivity();

    /**
     * @brief Asegura que no existan áreas aisladas en la cuadrícula.
     */
    void ensureNoIsolatedAreas();

    /**
     * @brief Asegura que exista al menos un obstáculo entre las zonas izquierda y derecha.
     */
    void ensureObstacleBetweenLeftAndRight();

    /**
     * @brief Asegura que cada línea tenga al menos un obstáculo.
     */
    void ensureObstaclesInLines();

    /**
     * @brief Rellena áreas abiertas grandes con obstáculos para evitar espacios vacíos.
     */
    void fillLargeOpenAreas();


};

#endif // GRIDGRAPH_H
