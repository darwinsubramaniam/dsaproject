#include "dashboard.h"

#include <iostream>

#include "constants.h"
#include "rapidcsv.h"

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
        if (!current->data.isCompleted() &&
            current->data.getStatus(today) == "Past Deadline" &&
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
    Node* current = head;

    while (current != nullptr) {
        if (current->data.getTitle() == title) {
            if (current->data.isCompleted()) {
                std::cout << "Event is already completed.\n";
                return;
            }

            // Keep the event for traceability; just flag it and stop
            // reminding about it.
            current->data.setCompleted(true);
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
    // Build an in-memory CSV document (row 0 holds the column names) and let
    // rapidcsv handle quoting/escaping on write.
    rapidcsv::Document doc(std::string(), rapidcsv::LabelParams(0, -1));
    doc.SetColumnName(0, "title");
    doc.SetColumnName(1, "subject");
    doc.SetColumnName(2, "type");
    doc.SetColumnName(3, "day");
    doc.SetColumnName(4, "month");
    doc.SetColumnName(5, "year");
    doc.SetColumnName(6, "completed");

    int row = 0;

    for (Node* current = head; current != nullptr; current = current->next) {
        const Event& e = current->data;
        const Date& d = e.getDueDate();

        doc.SetCell<std::string>(0, row, e.getTitle());
        doc.SetCell<std::string>(1, row, e.getSubject());
        doc.SetCell<std::string>(2, row, e.getType());
        doc.SetCell<int>(3, row, d.getDay());
        doc.SetCell<int>(4, row, d.getMonth());
        doc.SetCell<int>(5, row, d.getYear());
        doc.SetCell<int>(6, row, e.isCompleted() ? 1 : 0);

        row++;
    }

    try {
        doc.Save("events.csv");
    } catch (const std::exception&) {
        std::cout << "Error: Cannot save events.csv.\n";
    }
}
