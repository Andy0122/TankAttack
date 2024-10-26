#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

namespace DATA_STRUCTURES {

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    ~LinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++length;
    }

    void append(const T& value) {
        push_back(value);
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        head = tail = nullptr;
        length = 0;
    }

    T& operator[](int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Índice fuera de rango");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Índice fuera de rango");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T& at(int index) {
        return (*this)[index];
    }

    const T& at(int index) const {
        return (*this)[index];
    }

    int size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void remove(int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Índice fuera de rango");
        }

        Node* current = head;
        Node* previous = nullptr;

        for (int i = 0; i < index; ++i) {
            previous = current;
            current = current->next;
        }

        if (!previous) {
            head = current->next;
            if (!head) {
                tail = nullptr;
            }
        } else {
            previous->next = current->next;
            if (!current->next) {
                tail = previous;
            }
        }

        delete current;
        --length;
    }

    bool contains(const T& value) const {
        Node* current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

} // namespace DATA_STRUCTURES

#endif // LINKEDLIST_H
