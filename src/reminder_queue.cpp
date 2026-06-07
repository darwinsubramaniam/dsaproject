#include "reminder_queue.h"

#include <iostream>

bool ReminderQueue::isEmpty() const { return front == rear; }

void ReminderQueue::enqueue(const std::string& title) {
    if (rear < kMaxItems) {
        items[rear] = title;
        rear++;
    }
}

void ReminderQueue::clear(const std::string& title) {
    int i = front;

    while (i < rear) {
        if (items[i] == title) {
            int j = i;

            while (j < rear - 1) {
                items[j] = items[j + 1];
                j++;
            }

            rear--;
            return;
        }

        i++;
    }
}

void ReminderQueue::display() const {
    if (isEmpty()) {
        std::cout << "No reminders.\n";
        return;
    }

    int i = front;
    int no = 1;

    while (i < rear) {
        std::cout << no << ". Reminder: " << items[i] << "\n";
        i++;
        no++;
    }
}
