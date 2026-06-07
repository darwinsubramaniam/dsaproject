#ifndef NODE_H
#define NODE_H

#include "event.h"

// ------------------------- class Node --------------------------------
// Singly-linked list node owned by Dashboard.
class Node {
public:
    Event data;
    Node* next = nullptr;

    explicit Node(const Event& e) : data(e) {}
};

#endif  // NODE_H
