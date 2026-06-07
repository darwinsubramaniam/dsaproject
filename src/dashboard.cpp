#include "dashboard.h"

#include <iostream>

#include "constants.h"

void Dashboard::printPadded(const std::string& text, int width) const {
    std::cout << text;

    int i = static_cast<int>(text.length());

    while (i < width) {
        std::cout << " ";
        i++;
    }
}

Dashboard::~Dashboard() {
    Node* current = head;

    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

void Dashboard::addEvent(const Event& e) {
    Node* newNode = new Node(e);

    if (head == nullptr || e.dueDate.compareTo(head->data.dueDate) < 0) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;

        while (current->next != nullptr &&
               current->next->data.dueDate.compareTo(e.dueDate) < 0) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    reminders.enqueue(e.title);
}

bool Dashboard::deleteEvent(const std::string& title) {
    Node* target = nullptr;

    if (head != nullptr && head->data.title == title) {
        target = head;
        head = head->next;
    } else {
        Node* current = head;

        while (current != nullptr &&
               current->next != nullptr &&
               current->next->data.title != title) {
            current = current->next;
        }

        if (current != nullptr && current->next != nullptr) {
            target = current->next;
            current->next = target->next;
        }
    }

    if (target != nullptr) {
        delete target;
        reminders.clear(title);
        return true;
    }

    return false;
}

void Dashboard::amendEvent(const std::string& title, const Date& newDate) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.title == title) {
            Event e = current->data;
            e.dueDate = newDate;

            deleteEvent(title);
            addEvent(e);

            std::cout << "Event amended.\n";
            return;
        }

        current = current->next;
    }

    std::cout << "Event not found.\n";
}

void Dashboard::checkDeadlines(const Date& today) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.status(today) == Event::Status::Overdue &&
            !alerts.contains(current->data.title)) {
            alerts.push(current->data.title);
        }

        current = current->next;
    }
}

void Dashboard::displayTimeline(const Date& today) {
    checkDeadlines(today);

    if (head == nullptr) {
        std::cout << "No events found.\n";
        return;
    }

    std::cout << "No  Title                      Subject     Type        Due Date     Status\n";
    std::cout << "-------------------------------------------------------------------------------\n";

    Node* current = head;
    int no = 1;

    while (current != nullptr) {
        const Event& e = current->data;

        std::cout << no << "   ";
        printPadded(e.title, 27);
        printPadded(e.subject, 12);
        printPadded(e.type, 12);
        e.dueDate.display();
        std::cout << "   " << e.statusLabel(today) << "\n";

        current = current->next;
        no++;
    }
}

void Dashboard::displayCalendar() const {
    if (head == nullptr) {
        std::cout << "No events found.\n";
        return;
    }

    Node* current = head;
    long lastDate = -1;
    int itemNo = 0;

    while (current != nullptr) {
        const Event& e = current->data;
        long currentDate = e.dueDate.toKey();

        if (currentDate != lastDate) {
            std::cout << "\n" << e.dueDate.weekday() << ", ";
            e.dueDate.display();
            std::cout << "\n";
            lastDate = currentDate;
            itemNo = 0;
        }

        itemNo++;
        std::cout << "   " << itemNo << ". Title  : " << e.title << "\n"
                  << "      Subject: " << e.subject << "\n"
                  << "      Type   : " << e.type << "\n";

        current = current->next;
    }
}

void Dashboard::searchEvent(const std::string& keyword) const {
    Node* current = head;
    bool found = false;

    while (current != nullptr) {
        if (current->data.title == keyword ||
            current->data.subject == keyword) {
            std::cout << "Found: " << current->data.title
                      << " | " << current->data.subject
                      << " | ";
            current->data.dueDate.display();
            std::cout << "\n";

            found = true;
        }

        current = current->next;
    }

    if (!found) {
        std::cout << "Event not found.\n";
    }
}

void Dashboard::markComplete(const std::string& title) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.title == title) {
            if (current->data.completed) {
                std::cout << "Event is already completed.\n";
                return;
            }

            // Keep the event for traceability; just flag it and stop
            // reminding about it.
            current->data.completed = true;
            reminders.clear(title);
            std::cout << "Event marked complete.\n";
            return;
        }

        current = current->next;
    }

    std::cout << "Event not found.\n";
}

void Dashboard::showReminders() const {
    std::cout << "\nReminder Queue:\n";
    reminders.display();
}

void Dashboard::showOverdue() const {
    std::cout << "\nOverdue Stack:\n";
    alerts.display();
}

void Dashboard::sortEvents(int choice, const Date& today) const {
    if (head == nullptr) {
        std::cout << "No events found.\n";
        return;
    }

    Event arr[kMaxItems];
    int count = 0;

    Node* current = head;

    while (current != nullptr && count < kMaxItems) {
        arr[count] = current->data;
        count++;
        current = current->next;
    }

    for (int i = 0; i < count - 1; i++) {
        int selected = i;

        for (int j = i + 1; j < count; j++) {
            bool choose = false;

            if (choice == 1) {
                choose = arr[j].dueDate.compareTo(arr[selected].dueDate) < 0;
            } else if (choice == 2) {
                choose = arr[j].subject < arr[selected].subject;
            } else if (choice == 3) {
                choose = arr[j].status(today) < arr[selected].status(today);
            }

            if (choose) {
                selected = j;
            }
        }

        if (selected != i) {
            Event temp = arr[i];
            arr[i] = arr[selected];
            arr[selected] = temp;
        }
    }

    std::cout << "No  Title                      Subject     Type        Due Date     Status\n";
    std::cout << "-------------------------------------------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << "   ";
        printPadded(arr[i].title, 27);
        printPadded(arr[i].subject, 12);
        printPadded(arr[i].type, 12);
        arr[i].dueDate.display();
        std::cout << "   " << arr[i].statusLabel(today) << "\n";
    }
}

std::vector<Event> Dashboard::events() const {
    std::vector<Event> result;

    for (Node* current = head; current != nullptr; current = current->next) {
        result.push_back(current->data);
    }

    return result;
}
