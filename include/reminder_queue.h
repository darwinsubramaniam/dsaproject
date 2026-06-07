#ifndef REMINDER_QUEUE_H
#define REMINDER_QUEUE_H

#include <string>

#include "constants.h"

// ------------------------- Queue -------------------------------------
// Fixed-capacity circular FIFO queue of reminder titles.
// `front` indexes the next item to be served; `rear` indexes the next free
// slot. Both wrap around the array modulo kMaxItems, so freed slots at the
// head are reused. `count` tracks how many items are stored, which removes
// the empty-vs-full ambiguity of front == rear and lets us use every slot.
class ReminderQueue {
private:
    std::string items[kMaxItems];
    int front = 0;
    int rear = 0;
    int count = 0;

public:
    // --- State queries ---
    bool isEmpty() const;
    bool isFull() const;
    int size() const;

    // --- Core queue operations ---
    bool enqueue(const std::string& title);  // add to the rear; false if full
    std::string dequeue();                    // remove and return the front item
    const std::string& peek() const;          // look at the front item

    // --- Domain helper ---
    void clear(const std::string& title);  // remove a specific reminder by title

    // --- Output ---
    void display() const;  // prints reminders in FIFO order (oldest first)
};

#endif  // REMINDER_QUEUE_H
