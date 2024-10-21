#include "data_structures/LinkedList.h"

LinkedList::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

LinkedList::~LinkedList() {
    while (!empty()) {
        const auto temp = head;
        head = head->next;
        delete temp;
        length--;
    }
}

void LinkedList::append(const Position data) {
    const auto newNode = new Node{data, nullptr};

    if (empty()) {
        head = newNode;
    } else {
        tail->next = newNode;
    }

    tail = newNode;
    length++;
}

Position LinkedList::at(const int index) const {
    if (index < 0 || index >= length) {
        return Position{-1, -1};
    }

    auto current = head;
    for (auto i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

int LinkedList::size() const {
    return length;
}

bool LinkedList::empty() const {
    return length == 0;
}