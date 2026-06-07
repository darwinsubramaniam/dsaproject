#include "overdue_stack.h"

#include <iostream>

// --- State queries ---

bool OverdueStack::isEmpty() const { return top == -1; }

bool OverdueStack::isFull() const { return top == kMaxItems - 1; }

int OverdueStack::size() const { return top + 1; }

bool OverdueStack::contains(const std::string& title) const {
    for (int i = 0; i < size(); i++) {
        if (items[i] == title) {
            return true;
        }
    }

    return false;
}

// --- Core stack operations ---

bool OverdueStack::push(const std::string& title) {
    if (isFull()) {
        return false;
    }

    items[++top] = title;
    return true;
}

std::string OverdueStack::pop() {
    // Caller is expected to check isEmpty() first.
    return items[top--];
}

const std::string& OverdueStack::peek() const { return items[top]; }

// --- Output ---

void OverdueStack::display() const {
    if (isEmpty()) {
        std::cout << "No overdue alerts.\n";
        return;
    }

    // Walk a copy with pop() so the LIFO order is explicit:
    // the most recently pushed alert is shown first.
    OverdueStack remaining = *this;
    int no = 1;

    while (!remaining.isEmpty()) {
        std::cout << no << ". OVERDUE: " << remaining.pop() << "\n";
        no++;
    }
}
