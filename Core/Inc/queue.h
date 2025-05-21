#ifndef QUEUE_H
#define QUEUE_H

template<typename T>
struct Queue {
    T queue[255];
    int front = 0;
    int rear = 0;

    bool is_empty() {
        return front == rear;
    }

    bool is_full() {
        return (rear + 1) % 255 == front;
    }

    // adds item to queue
    void enqueue(T item) {
        if (!is_full()) {
            queue[rear] = item;
            rear = (rear + 1) % 255;
        }
    }

    // removes item from queue
    void dequeue() {
        if (!is_empty()) {
            front = (front + 1) % 255;
        }
    }

    // gets value of next item without removing
    T next() {
        return queue[front];
    }
};

#endif /* QUEUE_H */
