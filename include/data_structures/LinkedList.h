#ifndef LINKEDLIST_H
#define LINKEDLIST_H


namespace DATA_STRUCTURES {

/**
 * @brief Represents a linked list data structure.
 */
template <typename T>
class LinkedList {
public:
   /**
    * @brief Creates a new linked list.
    */
   LinkedList() : head(nullptr), tail(nullptr), length(0) {}

   /**
    * @brief Destroys the linked list.
    */
    ~LinkedList(){
          while (!empty()) {
              const auto temp = head;
              head = head->next;
              delete temp;
              length--;
          }
      }

   /**
    * @brief Appends a new position to the linked list.
    *
    * @param data The data to append.
    */
    void append(const T& data){
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
    * @brief Gets the position at the specified index.
    *
    * @param index The index of the position to get.
    * @return The data on the specified index.
    */
    [[nodiscard]] T at(const int index) const {
          if (index < 0 || index >= length) {
              return nullptr;
          }

          auto current = head;
          for (auto i = 0; i < index; i++) {
              current = current->next;
          }

          return current->data;
    }

   /**
    * @brief Gets the size of the linked list.
    *
    * @return The size of the linked list.
    */
    [[nodiscard]] int size() const{
       return length;
    }

   /**
    * @brief Checks if the linked list is empty.
    *
    * @return True if the linked list is empty, false otherwise.
    */
    [[nodiscard]] bool empty() const{
       return length == 0;
    }


private:
    /**
     * @brief Represents a node in the linked list.
     */
    struct Node {
     T data;
     Node* next;
    };

    Node* head; ///< The head of the linked list.
    Node* tail; ///< The tail of the linked list.
    int length; ///< The length of the linked list.

}; // LinkedList


}

#endif //LINKEDLIST_H
