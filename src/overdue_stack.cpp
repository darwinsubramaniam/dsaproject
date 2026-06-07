#include "overdue_stack.h"

#include <iostream>

bool OverdueStack::isEmpty() const { return top == -1; }

bool OverdueStack::contains(const std::string& title) const {
    int i = 0;

    while (i <= top) {
        if (items[i] == title) {
            return true;
        }

        i++;
    }

    return false;
}

void OverdueStack::push(const std::string& title) {
    if (top < kMaxItems - 1) {
        top++;
        items[top] = title;
    }
}

void OverdueStack::display() const {
    if (isEmpty()) {
        std::cout << "No overdue alerts.\n";
        return;
    }

    int i = top;
    int no = 1;

    while (i >= 0) {
        std::cout << no << ". OVERDUE: " << items[i] << "\n";
        i--;
        no++;
    }
}
