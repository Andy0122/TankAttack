#include "data_structures/Queue.h"

Queue::Queue() : head(nullptr), tail(nullptr), length(0) {}

Queue::~Queue() {
    while (!empty()) {
        pop();
    }
}

void Queue::push(const Position position) {
    const auto newNode = new Node{position, nullptr};

    if (empty()) {
        head = newNode;
    } else {
        tail->next = newNode;
    }

    tail = newNode;
    length++;
}

void Queue::pop() {
    if (empty()) {
        return;
    }

    const auto temp = head;
    head = head->next;
    delete temp;
    length--;

    if (empty()) {
        tail = nullptr;
    }
}

Position Queue::front() const {
    return head->data;
}

Position Queue::back() const {
    return tail->data;
}

int Queue::size() const {
    return length;
}

bool Queue::empty() const {
    return length == 0;
}
