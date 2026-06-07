#ifndef OVERDUE_STACK_H
#define OVERDUE_STACK_H

#include <string>

#include "constants.h"

// ------------------------- Stack -------------------------------------
class OverdueStack {
private:
    std::string items[kMaxItems];
    int top = -1;

public:
    bool isEmpty() const;
    bool contains(const std::string& title) const;
    void push(const std::string& title);
    void display() const;
};

#endif  // OVERDUE_STACK_H
