#ifndef STACK_H
#define STACK_H

#include <Position.h>

namespace DATA_STRUCTURES {

/**
 * @brief A stack data structure that stores Position objects.
 */
class Stack {
public:
    /**
     * @brief Default constructor.
     */
    Stack();

    /**
     * @brief Destructor.
     */
    ~Stack();

    /**
     * @brief Pushes a value into the top.
     *
     * @param value Value to push onto the stack.
     */
    void push(Position value);

    /**
     * @brief Pops the top value off the stack.
     */
    void pop();

    /**
     * @brief Returns the top value of the stack.
     *
     * @return Position The top value of the stack.
     */
    [[nodiscard]] Position top() const;

    /**
     * @brief Returns the size of the stack.
     *
     * @return int The size of the stack.
     */
    [[nodiscard]] int size() const;

    /**
     * @brief Returns whether the stack is empty.
     *
     * @return true If the stack is empty.
     * @return false If the stack is not empty.
     */
    [[nodiscard]] bool empty() const;

private:
    /**
     * @brief Node structure for the stack.
     */
    struct Node {
        Position value;
        Node* next;
    };

    Node* topNode; ///< Top node of the stack.
    int length; ///< Length of the stack.
};

}

#endif //STACK_H
