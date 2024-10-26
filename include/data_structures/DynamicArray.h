#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdexcept>

namespace DATA_STRUCTURES {

template<typename T>
class DynamicArray {
private:
    T* data;            ///< Puntero al arreglo de datos.
    int capacity;       ///< Capacidad actual del arreglo.
    int length;         ///< Número actual de elementos en el arreglo.

    /**
     * @brief Redimensiona el arreglo al doble de su capacidad actual.
     */
    void resize() {
        capacity *= 2;
        T* newData = static_cast<T*>(operator new[](sizeof(T) * capacity));

        for (int i = 0; i < length; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
    }

public:
    /**
     * @brief Constructor por defecto de DynamicArray.
     */
    DynamicArray() : data(new T[2]), capacity(2), length(0) {}

    /**
     * @brief Constructor con capacidad inicial.
     * @param initialCapacity Capacidad inicial del arreglo.
     */
    explicit DynamicArray(int initialCapacity) : data(new T[initialCapacity]), capacity(initialCapacity), length(0) {}

    /**
     * @brief Destructor de DynamicArray.
     */
    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (int i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            // Liberar la memoria actual
            delete[] data;

            // Copiar los datos del otro objeto
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (int i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }


    /**
     * @brief Agrega un elemento al final del arreglo.
     * @param value Valor a agregar.
     */
    void push_back(const T& value) {
        if (length == capacity) {
            resize();
        }
        data[length++] = value;
    }

    /**
     * @brief Elimina el último elemento del arreglo.
     */
    void pop_back() {
        if (length > 0) {
            --length;
        }
    }

    /**
     * @brief Accede al elemento en la posición indicada.
     * @param index Índice del elemento.
     * @return Referencia al elemento.
     */
    T& operator[](int index) {

        if (index < 0 || index >= length) {
            throw std::out_of_range("Índice fuera de rango");
        }
        return data[index];
    }

    /**
     * @brief Accede al elemento en la posición indicada (constante).
     * @param index Índice del elemento.
     * @return Referencia constante al elemento.
     */
    const T& operator[](int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Índice fuera de rango");
        }
        return data[index];
    }

    /**
     * @brief Obtiene el número de elementos en el arreglo.
     * @return Número de elementos.
     */
    int size() const {
        return length;
    }

    /**
     * @brief Verifica si el arreglo está vacío.
     * @return true si está vacío, false en caso contrario.
     */
    bool empty() const {
        return length == 0;
    }

    /**
     * @brief Elimina todos los elementos del arreglo.
     */
    void clear() {
        length = 0;
    }

    /**
     * @brief Redimensiona el arreglo a una nueva capacidad.
     * @param newCapacity Nueva capacidad deseada.
     */
    void resize(int newSize) {
        if (newSize > capacity) {
            // Necesitamos redimensionar y copiar los datos
            T* newData = new T[newSize];
            for (int i = 0; i < length; ++i) {
                newData[i] = data[i];
            }
            // Inicializar los nuevos elementos
            for (int i = length; i < newSize; ++i) {
                newData[i] = T();
            }
            delete[] data;
            data = newData;
            capacity = newSize;
        } else if (newSize > length) {
            // Solo necesitamos inicializar los nuevos elementos
            for (int i = length; i < newSize; ++i) {
                data[i] = T();
            }
        } else if (newSize < length) {
            // Si reducimos el tamaño, no hacemos nada con los datos excedentes
            // Opcionalmente, podrías destruir los objetos si T es un tipo que lo requiere
        }
        length = newSize;
    }


};

} // namespace DATA_STRUCTURES

#endif // DYNAMICARRAY_H
