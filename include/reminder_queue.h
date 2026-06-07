#ifndef REMINDER_QUEUE_H
#define REMINDER_QUEUE_H

#include <string>

#include "constants.h"

// ------------------------- Queue -------------------------------------
class ReminderQueue {
private:
    std::string items[kMaxItems];
    int front = 0;
    int rear = 0;

public:
    bool isEmpty() const;
    void enqueue(const std::string& title);
    void clear(const std::string& title);
    void display() const;
};

#endif  // REMINDER_QUEUE_H
