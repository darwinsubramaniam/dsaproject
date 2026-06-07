#include "dashboard.h"

#include <fstream>
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

    if (head == nullptr || e.getDueDate().compareTo(head->data.getDueDate()) < 0) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;

        while (current->next != nullptr &&
               current->next->data.getDueDate().compareTo(e.getDueDate()) < 0) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    reminders.enqueue(e.getTitle());
}

bool Dashboard::deleteEvent(const std::string& title) {
    Node* target = nullptr;

    if (head != nullptr && head->data.getTitle() == title) {
        target = head;
        head = head->next;
    } else {
        Node* current = head;

        while (current != nullptr &&
               current->next != nullptr &&
               current->next->data.getTitle() != title) {
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
        if (current->data.getTitle() == title) {
            Event e = current->data;
            e.setDueDate(newDate);

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
        if (current->data.getStatus(today) == "Past Deadline" &&
            !alerts.contains(current->data.getTitle())) {
            alerts.push(current->data.getTitle());
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
        printPadded(e.getTitle(), 27);
        printPadded(e.getSubject(), 12);
        printPadded(e.getType(), 12);
        e.getDueDate().display();
        std::cout << "   " << e.getStatus(today) << "\n";

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

    while (current != nullptr) {
        long currentDate = current->data.getDueDate().toKey();

        if (currentDate != lastDate) {
            std::cout << "\n[ ";
            current->data.getDueDate().display();
            std::cout << " ]\n";
            lastDate = currentDate;
        }

        std::cout << "- " << current->data.getTitle()
                  << " [" << current->data.getType() << "] "
                  << current->data.getSubject() << "\n";

        current = current->next;
    }
}

void Dashboard::searchEvent(const std::string& keyword) const {
    Node* current = head;
    bool found = false;

    while (current != nullptr) {
        if (current->data.getTitle() == keyword ||
            current->data.getSubject() == keyword) {
            std::cout << "Found: " << current->data.getTitle()
                      << " | " << current->data.getSubject()
                      << " | ";
            current->data.getDueDate().display();
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
    if (deleteEvent(title)) {
        std::cout << "Event marked complete and removed.\n";
    } else {
        std::cout << "Event not found.\n";
    }
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
                choose = arr[j].getDueDate().compareTo(arr[selected].getDueDate()) < 0;
            } else if (choice == 2) {
                choose = arr[j].getSubject() < arr[selected].getSubject();
            } else if (choice == 3) {
                choose = arr[j].statusRank(today) < arr[selected].statusRank(today);
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
        printPadded(arr[i].getTitle(), 27);
        printPadded(arr[i].getSubject(), 12);
        printPadded(arr[i].getType(), 12);
        arr[i].getDueDate().display();
        std::cout << "   " << arr[i].getStatus(today) << "\n";
    }
}

void Dashboard::saveEventsToFile() const {
    std::ofstream fout("events.txt");

    if (!fout) {
        std::cout << "Error: Cannot save events.txt.\n";
        return;
    }

    Node* current = head;

    while (current != nullptr) {
        const Event& e = current->data;
        const Date& d = e.getDueDate();

        fout << e.getTitle() << ";"
             << e.getSubject() << ";"
             << e.getType() << ";"
             << d.getDay() << ";"
             << d.getMonth() << ";"
             << d.getYear() << "\n";

        current = current->next;
    }
}
