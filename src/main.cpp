// =====================================================================
// MECS0023 DATA STRUCTURE & ALGORITHM - MINI PROJECT
// E-Learning Dashboard using events.csv
//
// Entry point: loads events from events.csv and runs the menu loop.
// =====================================================================

#include <iostream>
#include <limits>
#include <string>

#include "dashboard.h"
#include "date.h"
#include "event.h"
#include "store.h"

// Read an integer, re-prompting on non-numeric input. Returns false only when
// the input stream is closed (Ctrl+D / end of a pipe); the caller should then
// exit. (Ctrl+C terminates the program directly.)
bool readInt(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }

        if (std::cin.eof()) {
            return false;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "You entered wrong input. Please enter a number.\n";
    }
}

// Prompt for a day/month/year and build a Date. Returns false on closed input.
bool readDate(const std::string& label, Date& out) {
    int day = 0;
    int month = 0;
    int year = 0;

    if (!readInt(label + " day: ", day)) return false;
    if (!readInt(label + " month: ", month)) return false;
    if (!readInt(label + " year: ", year)) return false;

    out = Date(day, month, year);
    return true;
}

// ----------------------------- main ----------------------------------
int main() {
    Dashboard dash;
    const Date today = Date::today();

    store::load(dash);

    bool running = true;

    while (running) {
        int choice = 0;

        std::cout << "\n==========================================\n";
        std::cout << "          E-LEARNING DASHBOARD\n";
        std::cout << "==========================================\n";
        std::cout << " -- Retrieve Event Information --\n";
        std::cout << " 1. View Timeline (sorted by due date)\n";
        std::cout << " 2. View Calendar (grouped by date)\n";
        std::cout << " 3. Search Event\n";
        std::cout << " -- Update Event Information --\n";
        std::cout << " 4. Add Event\n";
        std::cout << " 5. Amend Event (change due date)\n";
        std::cout << " 6. Delete Event\n";
        std::cout << " -- Manage Event Status --\n";
        std::cout << " 7. Mark Event Complete\n";
        std::cout << " 8. View Reminders (queue)\n";
        std::cout << " 9. View Overdue Alerts (stack)\n";
        std::cout << " -- Sort Event --\n";
        std::cout << "10. Sort Events (date / subject / status)\n";
        std::cout << "11. Exit\n";
        std::cout << "------------------------------------------\n";
        if (!readInt(" Enter your choice (1-11): ", choice)) {
            break;  // input stream closed (Ctrl+D)
        }

        switch (choice) {
        case 1: {
            std::cout << "\n===== TIMELINE =====\n";
            dash.displayTimeline(today);
            break;
        }
        case 2: {
            std::cout << "\n===== CALENDAR =====\n";
            dash.displayCalendar();
            break;
        }
        case 3: {
            std::string keyword;

            std::cout << "Enter title or subject: ";
            std::getline(std::cin, keyword);

            dash.searchEvent(keyword);
            break;
        }
        case 4: {
            std::string title;
            std::string subject;
            std::string type;

            std::cout << "Title: ";
            std::getline(std::cin, title);

            std::cout << "Subject: ";
            std::getline(std::cin, subject);

            std::cout << "Type: ";
            std::getline(std::cin, type);

            Date dueDate;
            if (!readDate("Due", dueDate)) {
                running = false;
                break;
            }

            if (!dueDate.isValid()) {
                std::cout << "Invalid date. Event not added.\n";
                break;
            }

            dash.addEvent(Event{title, subject, type, dueDate});
            store::save(dash);

            std::cout << "Event added and saved to events.csv.\n";
            break;
        }
        case 5: {
            std::string title;

            std::cout << "Enter event title to amend: ";
            std::getline(std::cin, title);

            Date newDate;
            if (!readDate("New due", newDate)) {
                running = false;
                break;
            }

            if (!newDate.isValid()) {
                std::cout << "Invalid date. Amendment cancelled.\n";
                break;
            }

            dash.amendEvent(title, newDate);
            store::save(dash);

            std::cout << "events.csv updated.\n";
            break;
        }
        case 6: {
            std::string title;

            std::cout << "Enter event title to delete: ";
            std::getline(std::cin, title);

            if (dash.deleteEvent(title)) {
                store::save(dash);
                std::cout << "Event deleted and events.csv updated.\n";
            } else {
                std::cout << "Event not found.\n";
            }
            break;
        }
        case 7: {
            std::string title;

            std::cout << "Enter event title to mark complete: ";
            std::getline(std::cin, title);

            dash.markComplete(title);
            store::save(dash);

            std::cout << "events.csv updated.\n";
            break;
        }
        case 8: {
            dash.showReminders();
            break;
        }
        case 9: {
            dash.checkDeadlines(today);
            dash.showOverdue();
            break;
        }
        case 10: {
            int sortChoice = 0;

            if (!readInt("Sort by 1. Date  2. Subject  3. Status: ", sortChoice)) {
                running = false;
                break;
            }

            dash.sortEvents(sortChoice, today);
            break;
        }
        case 11: {
            store::save(dash);
            std::cout << "Events saved to events.csv. Goodbye!\n";
            running = false;
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter 1 to 11.\n";
            break;
        }
        }

        // After each action, ask whether to stay in the dashboard.
        // Anything other than 'y' (including just Enter) exits.
        if (running) {
            std::cout << "\nContinue using the dashboard? (y/N): ";

            std::string answer;
            std::getline(std::cin, answer);

            if (answer.empty() || (answer[0] != 'y' && answer[0] != 'Y')) {
                store::save(dash);
                std::cout << "Events saved to events.csv. Goodbye!\n";
                running = false;
            }
        }
    }

    return 0;
}
