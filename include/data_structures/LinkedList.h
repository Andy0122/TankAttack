#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <Position.h>

namespace DATA_STRUCTURES {

/**
 * @brief Represents a linked list data structure.
 */
class LinkedList {
public:
   /**
    * @brief Creates a new linked list.
    */
   LinkedList();

   /**
    * @brief Destroys the linked list.
    */
   ~LinkedList();

   /**
    * @brief Appends a new position to the linked list.
    *
    * @param data The position to append.
    */
   void append(Position data);

   /**
    * @brief Gets the position at the specified index.
    *
    * @param index The index of the position to get.
    * @return The position at the specified index.
    */
   [[nodiscard]] Position at(int index) const;

   /**
    * @brief Gets the size of the linked list.
    *
    * @return The size of the linked list.
    */
   [[nodiscard]] int size() const;

   /**
    * @brief Checks if the linked list is empty.
    *
    * @return True if the linked list is empty, false otherwise.
    */
   [[nodiscard]] bool empty() const;


private:
    /**
     * @brief Represents a node in the linked list.
     */
    struct Node {
     Position data;
     Node* next;
    };

    Node* head; ///< The head of the linked list.
    Node* tail; ///< The tail of the linked list.
    int length; ///< The length of the linked list.

}; // LinkedList


}

#endif //LINKEDLIST_H
