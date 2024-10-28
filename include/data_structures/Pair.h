// Definición de tu propia estructura Pair
template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    // Constructor por defecto
    Pair() : first(), second() {}

    // Constructor con parámetros
    Pair(const T1& a, const T2& b) : first(a), second(b) {}

    // Operadores de comparación
    bool operator<(const Pair& other) const {
        return first < other.first;
    }

    bool operator>(const Pair& other) const {
        return first > other.first;
    }

    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
};
