#ifndef QUEUE_H
#define QUEUE_H

#include <Position.h>

namespace DATA_STRUCTURES {

/**
 * @brief Queue data structure.
 */
class Queue {
public:
    /**
     * @brief Queue constructor.
     */
    Queue();

    /**
     * @brief Queue destructor.
     */
    ~Queue();

    /**
     * @brief Add a new element to the queue.
     * @param position Element to add.
     */
    void push(Position position);

    /**
     * @brief Remove the first element from the queue.
     */
    void pop();

    /**
     * @brief Get the first element from the queue.
     * @return The first element from the queue.
     */
    [[nodiscard]] Position front() const;

    /**
     * @brief Get the last element from the queue.
     * @return The last element from the queue.
     */
    [[nodiscard]] Position back() const;

    /**
     * @brief Get the size of the queue.
     * @return The size of the queue.
     */
    [[nodiscard]] int size() const;

    /**
     * @brief Check if the queue is empty.
     * @return True if the queue is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const;

private:
    /**
     * @brief Node structure for the queue.
     */
    struct Node {
     Position data;
     Node* next;
    };

    Node* head; ///< Pointer to the first element of the queue.
    Node* tail; ///< Pointer to the last element of the queue.
    int length; ///< Number of elements in the queue.

}; // Queue


}

#endif //QUEUE_H
