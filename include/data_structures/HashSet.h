#ifndef HASHSET_H
#define HASHSET_H

#include "LinkedList.h"
#include <functional>

namespace DATA_STRUCTURES {

template<typename T>
class HashSet {
private:
    static const int DEFAULT_CAPACITY = 16; ///< Capacidad inicial por defecto.
    static const float LOAD_FACTOR_THRESHOLD; ///< Factor de carga para rehashing.

    LinkedList<T>* table; ///< Arreglo de listas enlazadas para los buckets.
    int capacity;         ///< Capacidad actual de la tabla hash.
    int size;             ///< Número actual de elementos en el conjunto.

    /**
     * @brief Calcula el índice hash para un elemento.
     * @param key Elemento para el cual calcular el hash.
     * @return Índice en la tabla de hash.
     */
    int hash(const T& key) const {
        std::hash<T> hasher;
        return hasher(key) % capacity;
    }

    /**
     * @brief Redimensiona y rehash la tabla cuando se excede el factor de carga.
     */
    void rehash() {
        int oldCapacity = capacity;
        capacity *= 2;
        LinkedList<T>* oldTable = table;
        table = new LinkedList<T>[capacity];

        // Reinsertar los elementos en la nueva tabla
        for (int i = 0; i < oldCapacity; ++i) {
            LinkedList<T>& bucket = oldTable[i];
            for (int j = 0; j < bucket.size(); ++j) {
                const T& key = bucket[j];
                int index = hash(key);
                table[index].push_back(key);
            }
        }

        delete[] oldTable;
    }

public:
    /**
     * @brief Constructor por defecto de HashSet.
     */
    HashSet() : capacity(DEFAULT_CAPACITY), size(0) {
        table = new LinkedList<T>[capacity];
    }

    /**
     * @brief Destructor de HashSet.
     */
    ~HashSet() {
        delete[] table;
    }

    /**
     * @brief Inserta un elemento en el conjunto.
     * @param key Elemento a insertar.
     */
    void insert(const T& key) {
        if (contains(key)) {
            return; // El elemento ya está en el conjunto
        }

        int index = hash(key);
        table[index].push_back(key);
        ++size;

        // Verificar si se excede el factor de carga
        if (static_cast<float>(size) / capacity > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }
    }

    /**
     * @brief Verifica si un elemento está en el conjunto.
     * @param key Elemento a buscar.
     * @return true si el elemento está en el conjunto, false en caso contrario.
     */
    bool contains(const T& key) const {
        int index = hash(key);
        const LinkedList<T>& bucket = table[index];
        for (int i = 0; i < bucket.size(); ++i) {
            if (bucket[i] == key) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Elimina un elemento del conjunto.
     * @param key Elemento a eliminar.
     */
    void remove(const T& key) {
        int index = hash(key);
        LinkedList<T>& bucket = table[index];
        for (int i = 0; i < bucket.size(); ++i) {
            if (bucket[i] == key) {
                bucket.remove(i);
                --size;
                return;
            }
        }
    }

    /**
     * @brief Obtiene el número de elementos en el conjunto.
     * @return Número de elementos.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Verifica si el conjunto está vacío.
     * @return true si está vacío, false en caso contrario.
     */
    bool empty() const {
        return size == 0;
    }

    /**
     * @brief Elimina todos los elementos del conjunto.
     */
    void clear() {
        delete[] table;
        capacity = DEFAULT_CAPACITY;
        size = 0;
        table = new LinkedList<T>[capacity];
    }
};

// Inicialización del factor de carga
template<typename T>
const float HashSet<T>::LOAD_FACTOR_THRESHOLD = 0.75f;

} // namespace DATA_STRUCTURES

#endif // HASHSET_H
