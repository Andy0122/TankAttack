#include "../../include/systems/Pathfinder.h"
#include <queue>
#include <algorithm> // Para std::reverse
#include <functional>
#include <limits>

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
