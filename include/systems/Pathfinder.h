#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include "GridGraph.h"
#include "data_structures/Queue.h"

struct cell {
	int parent_i, parent_j;
	double f, g, h;
};

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

	/**
	 * @brief Search for a direct path between two positions.
	 * @param start Start position.
	 * @param goal Goal position.
	 * @return A pointer to queue with the found path or nullptr if there is no linear path.
	 */
	[[nodiscard]] Queue* lineaVista(Position start, Position goal) const;

	/**
	 * @brief Metodo que intenta moverse al objetivo utilizando línea de vista y movimiento aleatorio si es necesario.
	 * @param startId Identificador del nodo de inicio.
	 * @param goalId Identificador del nodo objetivo.
	 * @return Un vector con los nodos que forman el camino desde el inicio hasta el objetivo.
	 */
	std::vector<int> randomMovement(int startId, int goalId);

	[[nodiscard]] Queue* aStar(Position start, Position goal) const;
};

#endif // PATHFINDER_H
