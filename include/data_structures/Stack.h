#ifndef STACK_H
#define STACK_H

#include <Position.h>

namespace DATA_STRUCTURES {

class Stack {
public:
    Stack();

    ~Stack();

    void push(Position value);

    void pop();

    [[nodiscard]] Position top() const;

    [[nodiscard]] int size() const;

    [[nodiscard]] bool empty() const;

private:
    struct Node {
        Position value;
        Node* next;
    };

    Node* topNode;
    int length;
};

}

#endif //STACK_H
