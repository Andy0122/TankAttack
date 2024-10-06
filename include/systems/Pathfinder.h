#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include "GridGraph.h"

/**
 * @brief Clase encargada de los algoritmos de búsqueda de caminos en el GridGraph.
 */
class Pathfinder {
private:
    GridGraph& graph;  ///< Referencia al grafo sobre el cual se hacen las búsquedas.



public:
    /**
     * @brief Constructor del Pathfinder.
     * @param g Referencia a un GridGraph.
     */
    Pathfinder(GridGraph& g);

    /**
     * @brief Implementación de BFS para encontrar el camino más corto desde un nodo de inicio a un nodo de destino.
     * @param startId Identificador del nodo de inicio.
     * @param goalId Identificador del nodo objetivo.
     * @return Un vector con los nodos que forman el camino desde el inicio hasta el objetivo.
     */
    std::vector<int> bfs(int startId, int goalId);

	/**
	* @brief Implementación del algoritmo de Dijkstra para encontrar el camino más corto desde un nodo de inicio a un nodo de destino.
	* @param startId Identificador del nodo de inicio.
	* @param goalId Identificador del nodo objetivo.
	* @return Un vector con los nodos que forman el camino más corto desde el inicio hasta el objetivo.
	*/
	std::vector<int> dijkstra(int startId, int goalId);


};

#endif // PATHFINDER_H
