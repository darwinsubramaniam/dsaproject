#include "reminder_queue.h"

#include <iostream>

// --- State queries ---

bool ReminderQueue::isEmpty() const { return count == 0; }

bool ReminderQueue::isFull() const { return count == kMaxItems; }

int ReminderQueue::size() const { return count; }

// --- Core queue operations ---

bool ReminderQueue::enqueue(const std::string& title) {
    if (isFull()) {
        return false;
    }

    items[rear] = title;             // place at the next free slot
    rear = (rear + 1) % kMaxItems;   // advance, wrapping around
    count++;
    return true;
}

std::string ReminderQueue::dequeue() {
    // Caller is expected to check isEmpty() first.
    std::string served = items[front];
    front = (front + 1) % kMaxItems;  // advance, wrapping around
    count--;
    return served;
}

const std::string& ReminderQueue::peek() const { return items[front]; }

// --- Domain helper ---

void ReminderQueue::clear(const std::string& title) {
    // Scan the count live items, starting at front and wrapping around.
    for (int k = 0; k < count; k++) {
        int idx = (front + k) % kMaxItems;

        if (items[idx] == title) {
            // Close the gap: shift every later item back one slot (mod cap).
            for (int m = k; m < count - 1; m++) {
                int dst = (front + m) % kMaxItems;
                int src = (front + m + 1) % kMaxItems;
                items[dst] = items[src];
            }

            rear = (rear - 1 + kMaxItems) % kMaxItems;  // step the free slot back
            count--;
            return;
        }
    }
}

// --- Output ---

void ReminderQueue::display() const {
    if (isEmpty()) {
        std::cout << "No reminders.\n";
        return;
    }

    // Walk the count live items in FIFO order (oldest / next-served first).
    int no = 1;

    for (int k = 0; k < count; k++) {
        int idx = (front + k) % kMaxItems;
        std::cout << no << ". Reminder: " << items[idx] << "\n";
        no++;
    }
}
