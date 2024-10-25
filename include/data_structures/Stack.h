#ifndef STACK_H
#define STACK_H


namespace DATA_STRUCTURES {

/**
 * @brief A stack data structure that stores Position objects.
 */
template <typename T>
class Stack {
public:
    /**
     * @brief Default constructor.
     */
    Stack() : topNode(nullptr), length(0) {}

    /**
     * @brief Destructor.
     */
    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    /**
     * @brief Pushes a value into the top.
     *
     * @param value Value to push onto the stack.
     */
     void push(const T value) {
        auto* newNode = new Node{value, topNode};
        topNode = newNode;
        length++;
     }

    /**
     * @brief Pops the top value off the stack.
     */
     void pop(){
         if (!empty()) {
             const auto* temp = topNode;
             topNode = topNode->next;
             delete temp;
             length--;
         }
     }

    /**
     * @brief Returns the top value of the stack.
     *
     * @return Position The top value of the stack.
     */
    [[nodiscard]] T top() const {
        return topNode->value;
    }

    /**
     * @brief Returns the size of the stack.
     *
     * @return int The size of the stack.
     */
    [[nodiscard]] int size() const{
        return length;
    }

    /**
     * @brief Returns whether the stack is empty.
     *
     * @return true If the stack is empty.
     * @return false If the stack is not empty.
     */
    [[nodiscard]] bool empty() const{
        return topNode == nullptr;
    }

private:
    /**
     * @brief Node structure for the stack.
     */
    struct Node {
        T value;
        Node* next;
    };

    Node* topNode; ///< Top node of the stack.
    int length; ///< Length of the stack.
};

}

#endif //STACK_H
