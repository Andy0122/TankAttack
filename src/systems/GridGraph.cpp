#include "../../include/systems/GridGraph.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <utility>
#include <algorithm>

/**
 * @brief Constructor de la clase Node.
 *
 * @param id Identificador único del nodo.
 * @param accessible Indica si el nodo es accesible (true por defecto).
 */
Node::Node(int id, bool accessible) : id(id), accessible(accessible) {}

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
 * @brief Verifica si un nodo es un nodo seguro.
 *
 * Un nodo seguro no puede ser inaccesible.
 *
 * @param nodeId ID del nodo a verificar.
 * @return true Si el nodo es seguro.
 * @return false Si el nodo no es seguro.
 */
bool GridGraph::isSafeNode(int nodeId) {
    std::vector<int> safeNodes = {75, 76, 98, 99, 100, 101, 102, 122, 123, 124, 125, 126, 127, 147, 148, 149, 150, 151,
152, 172, 173, 174, 175, 176, 177, 197, 198, 199, 200, 201, 202, 222, 223, 224, 225,
226, 248, 249};
    //std::vector<int> safeNodes = {63, 64, 82, 83, 84, 85, 86, 102, 103, 104, 105, 106, 107, 123, 124, 125, 126, 127, 128, 144, 145, 146, 147, 148, 166, 167};
    // Recorrer la lista de nodos seguros y verificar si nodeId está en ella
    for (int safeNode : safeNodes) {
        if (safeNode == nodeId) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<std::vector<int>>> getPredefinedPatterns() {
    return {
            {
                {0, 0, 0, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}
            },
            {
                {1,1,0},{1,1,0},{0,0,0}
            },
            {
                {0,1,0},{0,1,0},{0,1,0}
            },
            {
                {1, 1, 0, 1},{1, 0, 0, 1},{1, 0, 0, 1},{1, 0, 1, 1}
            },
            {
                {0, 0, 0, 0, 0, 0},{0, 0, 0, 1, 0, 0},{0, 1, 1, 1, 0, 0},{0, 0, 1, 1, 1, 0},
                {0, 0, 1, 0, 0, 0},{0, 0, 0, 0, 0, 0}
            },
            {
                {1, 1, 0, 0, 1}, {1, 1, 0, 1, 1},{0, 0, 0, 0, 1},{1, 1, 1, 1, 1},{0, 0, 0, 0, 0}
            },
            {
                {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {1, 0, 1, 1, 0, 1}, {1, 0, 0, 1, 0, 1},
                {0, 0, 0, 0, 0, 0}, {0, 0, 1, 1, 0, 0}
            }
    };
}

std::vector<std::vector<int>> rotatePattern(const std::vector<std::vector<int>>& pattern) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            rotated[col][rows - row - 1] = pattern[row][col];
        }
    }
    return rotated;
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
            if (!currentNode.accessible) {
                continue; // Si el nodo no es accesible, no se conecta
            }

            int currentIndex = toIndex(row, col);

            // Conectar al vecino de arriba
            if (row > 0 && grid[row - 1][col].accessible) {
                int upIndex = toIndex(row - 1, col);
                adjList[currentIndex].push_back(upIndex);
            }

            // Conectar al vecino de abajo
            if (row < rows - 1 && grid[row + 1][col].accessible) {
                int downIndex = toIndex(row + 1, col);
                adjList[currentIndex].push_back(downIndex);
            }

            // Conectar al vecino de la izquierda
            if (col > 0 && grid[row][col - 1].accessible) {
                int leftIndex = toIndex(row, col - 1);
                adjList[currentIndex].push_back(leftIndex);
            }

            // Conectar al vecino de la derecha
            if (col < cols - 1 && grid[row][col + 1].accessible) {
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
    grid[row][col].accessible = accessible;
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
            std::cout << (grid[row][col].accessible ? "1 " : "0 ");
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
 * @brief Genera obstáculos aleatorios en la cuadrícula.
 *
 * Genera zonas inaccesibles aleatorias, respetando los nodos seguros.
 */
void GridGraph::generateObstacles(int density) {
    if (density == 0) {
        return;  // No se generan obstáculos
    }

    std::srand(std::time(0));

    // Calcular el número máximo de obstáculos según la densidad (0-10)
    int maxObstacles = (rows * cols * density) / 60;
    int numObstacles = 0;

    // Lista para registrar los IDs de las posiciones donde ya se colocaron obstáculos
    std::vector<int> occupiedPositions;

    // Obtener los patrones predefinidos
    std::vector<std::vector<std::vector<int>>> patterns = getPredefinedPatterns();

    while (numObstacles < maxObstacles) {
        // Seleccionar un patrón aleatorio
        const std::vector<std::vector<int>>& pattern = patterns[std::rand() % patterns.size()];

        // Rotar el patrón aleatoriamente
        int rotations = std::rand() % 4;
        auto rotatedPattern = pattern;
        for (int i = 0; i < rotations; ++i) {
            rotatedPattern = rotatePattern(rotatedPattern);
        }

        // Seleccionar una posición aleatoria
        int startRow = std::rand() % (rows - rotatedPattern.size() + 1);
        int startCol = std::rand() % (cols - rotatedPattern[0].size() + 1);

        // Verificar solapamiento con la nueva función
        if (checkOverlap(rotatedPattern, startRow, startCol, occupiedPositions)) {
            continue;  // Si se solapa con un obstáculo o una zona segura, no se coloca el patrón
        }

        // Colocar el patrón en la cuadrícula
        for (int r = 0; r < rotatedPattern.size(); ++r) {
            for (int c = 0; c < rotatedPattern[0].size(); ++c) {
                int nodeId = toIndex(startRow + r, startCol + c);
                grid[startRow + r][startCol + c].accessible = (rotatedPattern[r][c] == 0) ? true : false;

                // Añadir el ID de la posición (ya sea 0 o 1) a occupiedPositions
                occupiedPositions.push_back(nodeId);
            }
        }

        numObstacles++;

        // Verificar que los nodos accesibles aún están conectados
        if (!areAccessibleNodesConnected()) {
            // Revertir si los nodos accesibles se desconectaron
            placePattern(rotatedPattern, startRow, startCol, true);  // Revertir el patrón
            if (std::rand() % 2 == 0) {
                numObstacles--;
            }
            // Eliminar los IDs recién agregados de la lista
            for (int r = 0; r < rotatedPattern.size(); ++r) {
                for (int c = 0; c < rotatedPattern[0].size(); ++c) {
                    occupiedPositions.pop_back();  // Quitar el último ID agregado
                }
            }
        }
    }
}


bool GridGraph::checkOverlap(const std::vector<std::vector<int>>& rotatedPattern, int startRow, int startCol, const std::vector<int>& occupiedPositions) {
    bool overlaps = false;

    for (int r = 0; r < rotatedPattern.size() && !overlaps; ++r) {
        for (int c = 0; c < rotatedPattern[0].size(); ++c) {
            int nodeId = toIndex(startRow + r, startCol + c);

            // Verificar si ya está ocupado o es una zona segura (aunque el valor sea 0)
            if (std::find(occupiedPositions.begin(), occupiedPositions.end(), nodeId) != occupiedPositions.end() ||
                (rotatedPattern[r][c] == 1 && isSafeNode(nodeId))) {
                overlaps = true;
                break;
                }
        }
    }

    return overlaps;
}


bool GridGraph::areAccessibleNodesConnected() {
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    int accessibleCount = 0;

    // Encontrar un nodo accesible para iniciar la búsqueda
    bool startFound = false;
    for (int row = 0; row < rows && !startFound; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col].accessible) {
                q.push({row, col});
                visited[row][col] = true;
                startFound = true;
                break;
            }
        }
    }

    // Si no hay nodos accesibles, consideramos que no hay problema de conectividad
    if (q.empty()) {
        return true;
    }

    // Contar la cantidad total de nodos accesibles
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col].accessible) {
                accessibleCount++;
            }
        }
    }

    // Realizar la búsqueda BFS
    int visitedCount = 0;
    while (!q.empty()) {
        auto [currentRow, currentCol] = q.front();
        q.pop();
        visitedCount++;

        // Obtener los vecinos (arriba, abajo, izquierda, derecha)
        std::vector<std::pair<int, int>> neighbors = {
            {currentRow - 1, currentCol}, {currentRow + 1, currentCol},
            {currentRow, currentCol - 1}, {currentRow, currentCol + 1}
        };

        for (const auto& [nRow, nCol] : neighbors) {
            if (nRow >= 0 && nRow < rows && nCol >= 0 && nCol < cols &&
                grid[nRow][nCol].accessible && !visited[nRow][nCol]) {
                visited[nRow][nCol] = true;
                q.push({nRow, nCol});
                }
        }
    }

    // Verificar si se visitaron todos los nodos accesibles
    return visitedCount == accessibleCount;
}


void GridGraph::placePattern(const std::vector<std::vector<int>>& pattern, int row, int col, bool revert = false) {
    for (int r = 0; r < pattern.size(); ++r) {
        for (int c = 0; c < pattern[0].size(); ++c) {
            if (pattern[r][c] == 1) {
                grid[row + r][col + c].accessible = revert ? true : false;
            }
        }
    }
}
