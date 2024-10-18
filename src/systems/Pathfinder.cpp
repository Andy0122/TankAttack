#include "../../include/systems/Pathfinder.h"
#include <queue>
#include <algorithm> // Para std::reverse
#include <functional>
#include <limits>
#include <cstdlib>    // Para rand() y srand()
#include <cstring>
#include <ctime>      // Para time()
#include <float.h>
#include <bits/stdc++.h>

using namespace  std;

/**
 * @brief Constructor del Pathfinder.
 * @param g Referencia a un GridGraph.
 */
Pathfinder::Pathfinder(GridGraph& g) : graph(g) {}



/**
 * @brief Implementación de BFS para encontrar el camino más corto desde un nodo de inicio a un nodo de destino.
 * @param startId Identificador del nodo de inicio.
 * @param goalId Identificador del nodo objetivo.
 * @return Un vector con los nodos que forman el camino desde el inicio hasta el objetivo.
 */
std::vector<int> Pathfinder::bfs(int startId, int goalId) {
    std::vector<bool> visited(graph.getRows() * graph.getCols(), false);
    std::vector<int> parent(graph.getRows() * graph.getCols(), -1); // Para reconstruir el camino.
    std::queue<int> q;

    // Iniciar BFS desde el nodo inicial.
    visited[startId] = true;
    q.push(startId);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Si llegamos al nodo objetivo, reconstruir el camino.
        if (current == goalId) {
            std::vector<int> path;
            for (int at = goalId; at != -1; at = parent[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explorar los nodos vecinos
        for (int neighbor : graph.getAdjList()[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // Si no se encuentra un camino, devolver un vector vacío.
    return {};
}

/**
 * @brief Implementación del algoritmo de Dijkstra para encontrar el camino más corto desde un nodo de inicio a un nodo de destino.
 * @param startId Identificador del nodo de inicio.
 * @param goalId Identificador del nodo objetivo.
 * @return Un vector con los nodos que forman el camino más corto desde el inicio hasta el objetivo.
 */
std::vector<int> Pathfinder::dijkstra(int startId, int goalId) {
    const int INF = std::numeric_limits<int>::max(); // Valor infinito para inicializar distancias.
    std::vector<int> dist(graph.getRows() * graph.getCols() , INF); // Distancia a cada nodo.
    std::vector<int> parent(graph.getRows() * graph.getCols() , -1); // Para reconstruir el camino.
    dist[startId] = 0;

    // Cola de prioridad que almacena pares (distancia, nodo).
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        // Si llegamos al nodo objetivo, reconstruir el camino.
        if (current == goalId) {
            std::vector<int> path;
            for (int at = goalId; at != -1; at = parent[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Si la distancia actual es mayor que la mejor conocida, omitir.
        if (currentDist > dist[current]) continue;

        // Explorar los nodos vecinos
        for (int neighbor : graph.getAdjList()[current]) {
            // Suponemos un peso de 1 para cada arista
            int newDist = dist[current] + 1;

            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                parent[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }

    // Si no se encuentra un camino, devolver un vector vacío.
    return {};
}


/**
 * @brief Verifica si hay línea de vista directa entre dos nodos (sin obstáculos en línea recta).
 * @param startId Identificador del nodo de inicio.
 * @param goalId Identificador del nodo objetivo.
 * @return Un vector con los nodos que forman el camino directo si existe, o vacío si no.
 */
std::vector<int> Pathfinder::lineaVista(int startId, int goalId) {
    int startRow = startId / graph.getCols();
    int startCol = startId % graph.getCols();
    int goalRow = goalId / graph.getCols();
    int goalCol = goalId % graph.getCols();

    std::vector<int> path;

    if (startRow == goalRow) {
        // Moverse horizontalmente
        int colIncrement = (goalCol > startCol) ? 1 : -1;
        for (int col = startCol; col != goalCol + colIncrement; col += colIncrement) {
            if (graph.isObstacle(startRow, col)) {
                return {}; // Hay un obstáculo en el camino
            }
            path.push_back(graph.toIndex(startRow, col));
        }
        return path;
    } else if (startCol == goalCol) {
        // Moverse verticalmente
        int rowIncrement = (goalRow > startRow) ? 1 : -1;
        for (int row = startRow; row != goalRow + rowIncrement; row += rowIncrement) {
            if (graph.isObstacle(row, startCol)) {
                return {}; // Hay un obstáculo en el camino
            }
            path.push_back(graph.toIndex(row, startCol));
        }
        return path;
    } else {
        // No están alineados en fila o columna
        return {};
    }
}

/**
 * @brief Metodo que intenta moverse al objetivo utilizando línea de vista y movimiento aleatorio si es necesario.
 * @param startId Identificador del nodo de inicio.
 * @param goalId Identificador del nodo objetivo.
 * @return Un vector con los nodos que forman el camino desde el inicio hasta el objetivo.
 */
std::vector<int> Pathfinder::randomMovement(int startId, int goalId) {
    int currentId = startId;
    std::vector<int> totalPath;
    int attempts = 0;

    while (attempts < 4) {
        // Intentar línea de vista
        std::vector<int> lineaVistaPath = lineaVista(currentId, goalId);
        if (!lineaVistaPath.empty()) {
            // Se encontró línea de vista directa
            // Evitar duplicar el nodo actual si ya está en totalPath
            if (!totalPath.empty() && totalPath.back() == lineaVistaPath.front()) {
                lineaVistaPath.erase(lineaVistaPath.begin());
            }
            totalPath.insert(totalPath.end(), lineaVistaPath.begin(), lineaVistaPath.end());
            return totalPath;
        }

        // No hay línea de vista, realizar movimiento aleatorio
        std::vector<int> randomPath;
        int steps = 3 + std::rand() % 5; // Número aleatorio entre 3 y 7

        for (int i = 0; i < steps; ++i) {
            // Obtener las direcciones posibles
            std::vector<int> directions;
            int row = currentId / graph.getCols();
            int col = currentId % graph.getCols();

            // Arriba
            if (row > 0 && !graph.isObstacle(row - 1, col)) {
                directions.push_back(graph.toIndex(row - 1, col));
            }
            // Abajo
            if (row < graph.getRows() - 1 && !graph.isObstacle(row + 1, col)) {
                directions.push_back(graph.toIndex(row + 1, col));
            }
            // Izquierda
            if (col > 0 && !graph.isObstacle(row, col - 1)) {
                directions.push_back(graph.toIndex(row, col - 1));
            }
            // Derecha
            if (col < graph.getCols() - 1 && !graph.isObstacle(row, col + 1)) {
                directions.push_back(graph.toIndex(row, col + 1));
            }

            if (directions.empty()) {
                // No hay direcciones disponibles, detenerse
                break;
            }

            // Elegir una dirección aleatoria
            int randIndex = std::rand() % directions.size();
            int nextId = directions[randIndex];
            int nextRow = nextId / graph.getCols();
            int nextCol = nextId % graph.getCols();

            // Verificar si ya visitamos este nodo para evitar ciclos
            if (!randomPath.empty() && randomPath.back() == nextId) {
                continue;
            }

            randomPath.push_back(nextId);
            currentId = nextId;

            // Si hay un obstáculo en el siguiente movimiento, detenerse (no debería ocurrir)
            if (graph.isObstacle(nextRow, nextCol)) {
                break;
            }
        }

        // Agregar el camino aleatorio al camino total
        // Evitar duplicar el nodo actual si ya está en totalPath
        if (!totalPath.empty() && totalPath.back() == randomPath.front()) {
            randomPath.erase(randomPath.begin());
        }
        totalPath.insert(totalPath.end(), randomPath.begin(), randomPath.end());

        // Intentar línea de vista desde la nueva posición
        // currentId ya está actualizado
        attempts++;
    }

    return totalPath;
}

typedef std::pair<int, int> Pair;
typedef std::pair<double, std::pair<int, int> > pPair;

bool isDestination(int row, int col, Pair dest) {
    return row == dest.first && col == dest.second;
}

double calculateHValue(int row, int col, Pair dest) {
    return ((double)sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second)));
}

std::vector<int> Pathfinder::aStar(int startId, int goalId) {
    if (graph.isValid(startId / graph.getCols(), startId % graph.getCols())) {
        printf("Start node is invalid\n");
        return {};
    }

    if (graph.isValid(goalId / graph.getCols(), goalId % graph.getCols())) {
        printf("Goal node is invalid\n");
        return {};
    }

    if (graph.isObstacle(startId / graph.getCols(), startId % graph.getCols())
        || graph.isObstacle(goalId / graph.getCols(), goalId % graph.getCols())) {
        printf("Start node is an obstacle\n");
        return {};
    }

    if (startId == goalId) {
        printf("Start node is the goal node\n");
        return {};
    }

    bool closedList[graph.getRows()][graph.getCols()];
    memset(closedList, false, sizeof(closedList));

    cell cellDetails[graph.getRows()][graph.getCols()];

    int i, j;

    for (i = 0; i < graph.getRows(); i++) {
        for (j = 0; j < graph.getCols(); j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = startId / graph.getCols();
    j = startId % graph.getCols();
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    set<pPair> openList;

    openList.insert(make_pair(0.0, make_pair(1, j)));

    bool foundDest = false;

    vector<int> path;
    while (!openList.empty()) {
        pPair p = *openList.begin();

        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        if (graph.isValid(i - 1, j)) {
            path.push_back(graph.toIndex(i - 1, j));
            if (isDestination(i - 1, j, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i - 1][j] && !graph.isObstacle(i - 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, {goalId / graph.getCols(), goalId % graph.getCols()});
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j)));

                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------
        if (graph.isValid(i + 1, j)) {
            path.push_back(graph.toIndex(i + 1, j));
            if (isDestination(i + 1, j, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i + 1][j] && !graph.isObstacle(i + 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));

                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------
        if (graph.isValid(i, j + 1)) {
            path.push_back(graph.toIndex(i, j + 1));
            if (isDestination(i, j + 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i][j + 1] && !graph.isObstacle(i, j + 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j + 1)));

                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------
        if (graph.isValid(i, j - 1)) {
            path.push_back(graph.toIndex(i, j - 1));
            if (isDestination(i, j - 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i][j - 1] && !graph.isObstacle(i, j - 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i, j - 1)));

                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- 5th Successor (North-East) ------------
        if (graph.isValid(i - 1, j + 1)) {
            path.push_back(graph.toIndex(i - 1, j + 1));
            if (isDestination(i - 1, j + 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i - 1][j + 1] && !graph.isObstacle(i - 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j + 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j + 1].f == FLT_MAX || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));

                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 6th Successor (North-West)
        if (graph.isValid(i - 1, j - 1)) {
            path.push_back(graph.toIndex(i - 1, j - 1));
            if (isDestination(i - 1, j - 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i - 1][j - 1] && !graph.isObstacle(i - 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j - 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j - 1].f == FLT_MAX || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));

                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- 7th Successor (South-East)
        if (graph.isValid(i + 1, j + 1)) {
            path.push_back(graph.toIndex(i + 1, j + 1));
            if (isDestination(i + 1, j + 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i + 1][j + 1] && !graph.isObstacle(i + 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j + 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j + 1].f == FLT_MAX || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));

                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 8th Successor (South-West)
        if (graph.isValid(i + 1, j - 1)) {
            path.push_back(graph.toIndex(i + 1, j - 1));
            if (isDestination(i + 1, j - 1, {goalId / graph.getCols(), goalId % graph.getCols()})) {
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return path;

            } else if (!closedList[i + 1][j - 1] && !graph.isObstacle(i + 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j - 1, Pair(goalId / graph.getCols(), goalId % graph.getCols()));
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j - 1].f == FLT_MAX || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j - 1)));

                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    if (foundDest == false) {
        printf("Failed to find the destination cell\n");
    }
    return {};
}
