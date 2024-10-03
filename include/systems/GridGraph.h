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
    bool obstacle;   ///< Indica si el nodo es accesible o no.
    bool occupied = false;   ///< Indica si el nodo esta ocupado o no.

    /**
     * @brief Constructor del nodo.
     *
     * @param id Identificador único del nodo.
     * @param accessible Valor opcional que indica si el nodo es accesible (true por defecto).
     */
    Node(int id, bool accessible = true);
};

/**
 * @brief Clase que representa una cuadrícula de nodos (grafo) y sus conexiones.
 */
class GridGraph {
private:
    static const int rows = 11; ///< Número predefinido de filas en la cuadrícula.
    static const int cols = 21; ///< Número predefinido de columnas en la cuadrícula.
    std::vector<std::vector<Node>> grid;  ///< Matriz que contiene los nodos del grafo.
    std::vector<std::vector<int>> adjList; ///< Lista de adyacencia para representar conexiones entre nodos.

    /**
     * @brief Convierte coordenadas 2D en un índice 1D en la cuadrícula.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @return Índice 1D correspondiente.
     */
    int toIndex(int row, int col) const;

public:
    /**
     * @brief Constructor de la clase GridGraph. Inicializa la cuadrícula de nodos.
     */
    GridGraph();

    /**
     * @brief Conecta los nodos accesibles en la cuadrícula.
     */
    void connectNodes();

    /**
     * @brief Imprime la representación del grafo en la consola.
     */
    void printGraph() const;

    /**
     * @brief Establece la accesibilidad de un nodo específico.
     *
     * @param row Fila del nodo.
     * @param col Columna del nodo.
     * @param accessible Valor booleano que indica si el nodo es accesible.
     */
    void setNodeAccessibility(int row, int col, bool accessible);

    [[nodiscard]] bool isObstacle(int row, int col) const;

    [[nodiscard]] bool isOccupied(int row, int col) const;

    void placeTank(int row, int col);

    void removeTank(int row, int col);

    /**
     * @brief Imprime la accesibilidad de los nodos en la cuadrícula.
     */
    void printAccessibility() const;

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
     * @brief Genera obstáculos aleatorios en la cuadrícula, marcando nodos como inaccesibles.
     */
    void generateObstacles();

    /**
     * @brief Verifica si un nodo específico es seguro.
     *
     * @param nodeId ID del nodo.
     * @return true Si el nodo es seguro.
     * @return false Si el nodo no es seguro.
     */
    bool isSafeNode(int nodeId);

    [[nodiscard]] Position getRandomAccessiblePosition() const;
};

#endif
