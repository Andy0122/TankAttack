#ifndef QUEUE_H
#define QUEUE_H

#include <Position.h>


class Queue {
public:
    Queue();
    ~Queue();
    void push(Position position);
    void pop();
    [[nodiscard]] Position front() const;
    [[nodiscard]] Position back() const;
    [[nodiscard]] int size() const;
    [[nodiscard]] bool empty() const;

private:
    struct Node {
        Position data;
        Node* next;
    };
    Node* head;
    Node* tail;
    int length;
};



#endif //QUEUE_H
