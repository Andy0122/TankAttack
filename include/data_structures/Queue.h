#ifndef QUEUE_H
#define QUEUE_H


namespace DATA_STRUCTURES {

/**
 * @brief Queue data structure.
 */
template <typename T>
class Queue {
public:
    /**
     * @brief Queue constructor.
     */
    Queue() : head(nullptr), tail(nullptr), length(0) {}

    /**
     * @brief Queue destructor.
     */
    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    /**
     * @brief Add a new element to the queue.
     * @param data Element to add.
     */
     void push(const T data) {
         const auto newNode = new Node{data, nullptr};

         if (empty()) {
             head = newNode;
         } else {
             tail->next = newNode;
         }

         tail = newNode;
         length++;
     }

    /**
     * @brief Remove the first element from the queue.
     */
     void pop() {
         if (empty()) {
             return;
         }

         auto temp = head;
         head = head->next;
         delete temp;
         length--;

         if (empty()) {
             tail = nullptr;
         }
    }

    /**
     * @brief Get the first element from the queue.
     * @return The first element from the queue.
     */
    [[nodiscard]] T front() const {
        return head->data;
    }

    /**
     * @brief Get the last element from the queue.
     * @return The last element from the queue.
     */
    [[nodiscard]] T back() const {
        return tail->data;
    }

    /**
     * @brief Get the size of the queue.
     * @return The size of the queue.
     */
    [[nodiscard]] int size() const {
        return length;
    }

    /**
     * @brief Check if the queue is empty.
     * @return True if the queue is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const{
        return length == 0;
    }

private:
    /**
     * @brief Node structure for the queue.
     */
    struct Node {
     T data;
     Node* next;
    };

    Node* head; ///< Pointer to the first element of the queue.
    Node* tail; ///< Pointer to the last element of the queue.
    int length; ///< Number of elements in the queue.

}; // Queue


}

#endif //QUEUE_H
