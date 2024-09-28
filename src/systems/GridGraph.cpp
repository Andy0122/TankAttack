#include "../../include/systems/GridGraph.h"
#include <cstdlib>
#include <ctime>

/**
 * @brief Constructor de la clase Node.
 *
 * @param id Identificador único del nodo.
 * @param accessible Indica si el nodo es accesible (true por defecto).
 */
Node::Node(int id, bool accessible) : id(id), obstacle(accessible) {}

/**
 * @brief Constructor de la clase GridGraph.
 *
 * Inicializa la cuadrícula de nodos con un número predefinido de filas y columnas.
 * Los nodos son accesibles por defecto.
 */
GridGraph::GridGraph() {
    // Inicializar la cuadrícula de nodos con filas y columnas predefinidas
    int id = 0;
    for (int row = 0; row < rows; ++row) {
        std::vector<Node> rowNodes;
        for (int col = 0; col < cols; ++col) {
            rowNodes.emplace_back(id++, true); // Todos los nodos son accesibles por defecto
        }
        grid.push_back(rowNodes);
    }

    // Inicializar la lista de adyacencia
    adjList.resize(rows * cols);
}

/**
 * @brief Convierte las coordenadas 2D a un índice 1D en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @return Índice 1D correspondiente al nodo.
 */
int GridGraph::toIndex(int row, int col) const {
    return row * cols + col;
}

/**
 * @brief Conecta los nodos accesibles en la cuadrícula.
 *
 * Conecta cada nodo con sus vecinos (arriba, abajo, izquierda, derecha) si son accesibles.
 */
void GridGraph::connectNodes() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Node& currentNode = grid[row][col];
            if (!currentNode.obstacle) {
                continue; // Si el nodo no es accesible, no se conecta
            }

            int currentIndex = toIndex(row, col);

            // Conectar al vecino de arriba
            if (row > 0 && grid[row - 1][col].obstacle) {
                int upIndex = toIndex(row - 1, col);
                adjList[currentIndex].push_back(upIndex);
            }

            // Conectar al vecino de abajo
            if (row < rows - 1 && grid[row + 1][col].obstacle) {
                int downIndex = toIndex(row + 1, col);
                adjList[currentIndex].push_back(downIndex);
            }

            // Conectar al vecino de la izquierda
            if (col > 0 && grid[row][col - 1].obstacle) {
                int leftIndex = toIndex(row, col - 1);
                adjList[currentIndex].push_back(leftIndex);
            }

            // Conectar al vecino de la derecha
            if (col < cols - 1 && grid[row][col + 1].obstacle) {
                int rightIndex = toIndex(row, col + 1);
                adjList[currentIndex].push_back(rightIndex);
            }
        }
    }
}

/**
 * @brief Modifica la accesibilidad de un nodo en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @param accessible Valor booleano que indica si el nodo es accesible o no.
 */
void GridGraph::setNodeAccessibility(int row, int col, bool accessible) {
    grid[row][col].obstacle = accessible;
}

bool GridGraph::isObstacle(int row, int col) const {
    return !grid[row][col].obstacle;
}

bool GridGraph::isOccupied(int row, int col) const {
    return grid[row][col].occupied;
}

void GridGraph::placeTank(int row, int col) {
    grid[row][col].occupied = true;
}

void GridGraph::removeTank(int row, int col) {
    grid[row][col].occupied = false;
}

/**
 * @brief Imprime la representación del grafo, mostrando las conexiones entre nodos.
 */
void GridGraph::printGraph() const {
    for (int i = 0; i < adjList.size(); ++i) {
        std::cout << "Node " << i << " is connected to: ";
        for (int neighbor : adjList[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Imprime la accesibilidad de los nodos en la cuadrícula.
 *
 * Imprime un "1" si el nodo es accesible y "0" si no lo es.
 */
void GridGraph::printAccessibility() const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::cout << (grid[row][col].obstacle ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Devuelve el número de filas de la cuadrícula.
 *
 * @return Número de filas.
 */
int GridGraph::getRows() const {
    return rows;
}

/**
 * @brief Devuelve el número de columnas de la cuadrícula.
 *
 * @return Número de columnas.
 */
int GridGraph::getCols() const {
    return cols;
}

/**
 * @brief Devuelve una referencia constante a un nodo específico en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @return Referencia constante al nodo.
 */
const Node& GridGraph::getNode(int row, int col) const {
    return grid[row][col];
}

/**
 * @brief Verifica si un nodo es un nodo seguro.
 *
 * Un nodo seguro no puede ser inaccesible.
 *
 * @param nodeId ID del nodo a verificar.
 * @return true Si el nodo es seguro.
 * @return false Si el nodo no es seguro.
 */
bool GridGraph::isSafeNode(int nodeId) {
    std::vector<int> safeNodes = {63, 64, 86, 107, 128, 148, 147, 85, 84, 105, 106, 127, 126, 125, 124, 123, 102, 103, 104, 83, 82, 144, 145, 146, 167, 166};

    // Recorrer la lista de nodos seguros y verificar si nodeId está en ella
    for (int safeNode : safeNodes) {
        if (safeNode == nodeId) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Genera obstáculos aleatorios en la cuadrícula.
 *
 * Genera zonas inaccesibles aleatorias, respetando los nodos seguros.
 */
void GridGraph::generateObstacles() {
    // Inicializar random seed
    std::srand(std::time(0));

    // Número de obstáculos a generar (puedes ajustar este número según el tamaño de la cuadrícula)
    int numObstacles = 10 + std::rand() % 10;  // Generar entre 10 y 20 obstáculos

    for (int i = 0; i < numObstacles; ++i) {
        // Seleccionar una posición inicial aleatoria para el obstáculo
        int startRow = std::rand() % rows;
        int startCol = std::rand() % cols;

        // Tamaño aleatorio del obstáculo
        int obstacleWidth = 1 + std::rand() % 3;  // Ancho del obstáculo (entre 1 y 3)
        int obstacleHeight = 1 + std::rand() % 3;  // Alto del obstáculo (entre 1 y 3)

        // Generar un obstáculo de forma rectangular
        for (int row = startRow; row < startRow + obstacleHeight && row < rows; ++row) {
            for (int col = startCol; col < startCol + obstacleWidth && col < cols; ++col) {
                int nodeId = row * cols + col;

                // Usar el metodo isSafeNode para verificar si el nodo es seguro antes de hacerlo inaccesible
                if (!isSafeNode(nodeId)) {
                    grid[row][col].obstacle = false;  // Hacer inaccesible
                }
            }
        }
    }
}