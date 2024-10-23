#ifndef POSITION_H
#define POSITION_H


/**
 * @brief Struct that represents a position in a grid map.
 */
struct Position {
    int row;    ///< Row of the position.
    int column;    ///< Column of the position.

    bool operator==(const Position& other) const {
        return row == other.row && column == other.column;
    }
};

#endif //POSITION_H
