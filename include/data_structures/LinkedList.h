#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <Position.h>


class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    void append(Position data);
    [[nodiscard]] Position at(int index) const;
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



#endif //LINKEDLIST_H
