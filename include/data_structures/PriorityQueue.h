#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

template<typename T>
class PriorityQueue {
private:
    DATA_STRUCTURES::DynamicArray<T> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap[index] < heap[parentIndex]) {
                std::swap(heap[index], heap[parentIndex]);
                index = parentIndex;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (index * 2 + 1 < size) {
            int smallest = index;
            int leftChild = index * 2 + 1;
            int rightChild = index * 2 + 2;

            if (leftChild < size && heap[leftChild] < heap[smallest]) {
                smallest = leftChild;
            }
            if (rightChild < size && heap[rightChild] < heap[smallest]) {
                smallest = rightChild;
            }
            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    PriorityQueue() {}

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (!heap.empty()) {
            heap[0] = heap[heap.size() - 1];
            heap.pop_back();
            heapifyDown(0);
        }
    }

    const T& top() const {
        return heap[0];
    }

    bool empty() const {
        return heap.empty();
    }
};
#endif // PRIORITY_QUEUE_H
