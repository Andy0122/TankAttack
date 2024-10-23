#include "data_structures/Stack.h"

using namespace DATA_STRUCTURES;

Stack::Stack() : topNode(nullptr), length(0) {}

Stack::~Stack() {
    while (!empty()) {
        pop();
    }
}

void Stack::push(const Position value) {
    auto* newNode = new Node{value, topNode};
    topNode = newNode;
    length++;
}

void Stack::pop() {
    if (!empty()) {
        const auto* temp = topNode;
        topNode = topNode->next;
        delete temp;
        length--;
    }
}

Position Stack::top() const {
    return topNode->value;
}

int Stack::size() const {
    return length;
}

bool Stack::empty() const {
    return topNode == nullptr;
}