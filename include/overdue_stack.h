#ifndef OVERDUE_STACK_H
#define OVERDUE_STACK_H

#include <string>

#include "constants.h"

// ------------------------- Stack -------------------------------------
// Fixed-capacity LIFO stack of overdue alert titles.
// `top` is the index of the most recently pushed item (-1 when empty).
class OverdueStack {
private:
    std::string items[kMaxItems];
    int top = -1;

public:
    // --- State queries ---
    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    bool contains(const std::string& title) const;

    // --- Core stack operations ---
    bool push(const std::string& title);  // returns false if the stack is full
    std::string pop();                     // removes and returns the top item
    const std::string& peek() const;       // looks at the top item without removing

    // --- Output ---
    void display() const;  // prints alerts in LIFO order (newest first)
};

#endif  // OVERDUE_STACK_H
