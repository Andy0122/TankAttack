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

#include "data_structures/Stack.h"

using namespace  std;
using namespace DATA_STRUCTURES;

#define ROW 13
#define COL 25


/**
 * @brief Constructor del Pathfinder.
 * @param g Referencia a un GridGraph.
 */
Pathfinder::Pathfinder(GridGraph& g) : graph(g) {}

Queue<Position>* convertStackToQueue(Stack<Position>& stack) {
    auto* queue = new Queue<Position>();
    while (!stack.empty()) {
        queue->push(stack.top());
        stack.pop();
    }
    return queue;
}

/**
 * @brief Implementación de BFS para encontrar el camino más corto desde un nodo de inicio a un nodo de destino.
 * @param startId Identificador del nodo de inicio.
 * @param goalId Identificador del nodo objetivo.
 * @return Un vector con los nodos que forman el camino desde el inicio hasta el objetivo.
 */
Queue<Position>* Pathfinder::bfs(Position src, Position dest) {
    std::vector<bool> visited(graph.getRows() * graph.getCols(), false);
    std::vector<int> parent(graph.getRows() * graph.getCols(), -1); // Para reconstruir el camino.
    std::queue<int> q;

    int startId = graph.toIndex(src.row, src.column);
    int goalId = graph.toIndex(dest.row, dest.column);

    // Iniciar BFS desde el nodo inicial.
    visited[startId] = true;
    q.push(startId);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Si llegamos al nodo objetivo, reconstruir el camino.
        if (current == goalId) {
            auto* path = new Stack<Position>();
            // std::vector<int> path;
            for (int at = goalId; at != -1; at = parent[at]) {
                path->push(Position{at / graph.getCols(), at % graph.getCols()});
            }
            // std::reverse(path->begin(), path->end());
            return convertStackToQueue(*path);
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



Queue<Position>* Pathfinder::lineaVista(Position start, Position goal) const {
    auto [startRow, startCol] = start;
    auto [goalRow, goalCol] = goal;

    auto* path = new Queue<Position>();

    if (startRow == goalRow) { // Move horizontally
        const int colIncrement = goalCol > startCol ? 1 : -1;
        for (int col = startCol + colIncrement; col != goalCol + colIncrement; col += colIncrement) {
            if (graph.isObstacle(startRow, col)) {
                return nullptr;
            }
            path->push(Position{startRow, col});
        }
        return path;
    }
    if (startCol == goalCol) { // Move vertically
        const int rowIncrement = goalRow > startRow ? 1 : -1;
        for (int row = startRow + rowIncrement; row != goalRow + rowIncrement; row += rowIncrement) {
            if (graph.isObstacle(row, startCol)) {
                return nullptr;
            }
            path->push(Position{row, startCol});
        }
        return path;
    }

    return nullptr;
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
        Queue<Position>* lineaVistaPath = lineaVista(Position{currentId / graph.getCols()}, Position{goalId % graph.getCols()});
        if (!lineaVistaPath->empty()) {
            // Se encontró línea de vista directa
            // Evitar duplicar el nodo actual si ya está en totalPath
            if (!totalPath.empty() && totalPath.back() == graph.toIndex(lineaVistaPath->front().row, lineaVistaPath->front().column)) {
                lineaVistaPath->pop();
            }
            for (int i = 0; i < lineaVistaPath->size(); ++i) {
                totalPath.push_back(graph.toIndex(lineaVistaPath->front().row, lineaVistaPath->front().column));
                lineaVistaPath->pop();
            }
            // totalPath.insert(totalPath.end(), graph.toIndex(lineaVistaPath->front().row, lineaVistaPath->front().column),
            //     graph.toIndex(lineaVistaPath->back().row, lineaVistaPath->back().column));
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

bool isDestination(const int row, const int col, const Pair &dest) {
    return row == dest.first && col == dest.second;
}

double calculateHValue(const int row, const int col, const Position &dest) {
    return sqrt((row - dest.row) * (row - dest.row) + (col - dest.column) * (col - dest.column));
}

bool isDestination(int row, int col, Position dest)
{
    if (row == dest.row && col == dest.column)
        return (true);
    else
        return (false);
}

bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}

Stack<Position>* tracePath(cell cellDetails[][COL], const Position dest) {
    auto [row, col] = dest;

    auto* path = new Stack<Position>();

    while (!(cellDetails[row][col].parent_i == row
            && cellDetails[row][col].parent_j == col)) {
        path->push(Position{row, col});
        const int temp_row = cellDetails[row][col].parent_i;
        const int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    return path;
    while (!path->empty()) {
        Position p = path->top();
        path->pop();
        printf("-> (%d, %d)", p.row, p.column);
    }
}

//Finish A* algorithm
Stack<Position>* Pathfinder::aStar(const Position src, const Position dest) const {
    if (!GridGraph::isValid(src.row, src.column)) {
        printf("Start node is invalid\n");
        return nullptr;
    }

    if (!GridGraph::isValid(dest.row, dest.column)) {
        printf("Goal node is invalid\n");
        return nullptr;
    }

    if (graph.isObstacle(src.row, src.column)
        || graph.isObstacle(dest.row, dest.column)) {
        printf("Start node is an obstacle\n");
        return nullptr;
    }

    if (src == dest) {
        printf("Source node is the destination node\n");
        return nullptr;
    }

    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));

    // Declare a 2D array of structure to hold the details
    // of that cell
    cell cellDetails[ROW][COL];

    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.row, j = src.column;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair of
     pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /*
         Generating all the 8 successor of this cell

             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \
                /   |  \
             S.W    S   S.E

         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West           (i, j-1)
         N.E--> North-East  (i-1, j+1)
         N.W--> North-West  (i-1, j-1)
         S.E--> South-East  (i+1, j+1)
         S.W--> South-West  (i+1, j-1)*/

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (GridGraph::isValid(i - 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("The destination cell is found\n");
                foundDest = true;
                return tracePath(cellDetails, dest);;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j] == false
                     && !graph.isObstacle(i - 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j] == false
                     && !graph.isObstacle(i + 1, j)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j + 1] == false
                     && !graph.isObstacle(i, j + 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j - 1] == false
                     && !graph.isObstacle(i, j - 1)) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));

                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- 5th Successor (North-East)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j + 1] == false
                     && !graph.isObstacle(i - 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 6th Successor (North-West)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(i - 1, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j - 1] == false
                     && !graph.isObstacle(i - 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));
                    // Update the details of this cell
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- 7th Successor (South-East)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j + 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j + 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j + 1] == false
                     && !graph.isObstacle(i + 1, j + 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 8th Successor (South-West)
        //------------

        // Only process this cell if this is a valid one
        if (isValid(i + 1, j - 1) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i + 1, j - 1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("The destination cell is found\n");
                return tracePath(cellDetails, dest);
                foundDest = true;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j - 1] == false
                     && !graph.isObstacle(i + 1, j - 1)) {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return nullptr;
}
