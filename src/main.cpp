// =====================================================================
// MECS0023 DATA STRUCTURE & ALGORITHM - MINI PROJECT
// E-Learning Dashboard using events.txt
//
// Entry point: loads events from events.txt and runs the menu loop.
// =====================================================================

#include <fstream>
#include <iostream>
#include <string>

#include "dashboard.h"
#include "date.h"
#include "event.h"

// ---------------------- Load from events.txt --------------------------
void loadEventsFromFile(Dashboard& dash) {
    std::ifstream fin("events.txt");

    if (!fin) {
        std::cout << "events.txt not found.\n";
        std::cout << "Please create events.txt in the same folder as main.cpp.\n";
        return;
    }

    std::string title;
    std::string subject;
    std::string type;
    std::string dayText;
    std::string monthText;
    std::string yearText;

    while (std::getline(fin, title, ';')) {
        std::getline(fin, subject, ';');
        std::getline(fin, type, ';');
        std::getline(fin, dayText, ';');
        std::getline(fin, monthText, ';');
        std::getline(fin, yearText);

        int day = std::stoi(dayText);
        int month = std::stoi(monthText);
        int year = std::stoi(yearText);

        dash.addEvent(Event(title, subject, type, Date(day, month, year)));
    }

    std::cout << "Events loaded from events.txt.\n";
}

// ----------------------------- main ----------------------------------
int main() {
    Dashboard dash;
    Date today(6, 6, 2026);

    loadEventsFromFile(dash);

    int choice = 0;

    do {
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
        std::cout << " Enter your choice (1-11): ";
        std::cin >> choice;
        std::cin.ignore();

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
            int day = 0;
            int month = 0;
            int year = 0;

            std::cout << "Title: ";
            std::getline(std::cin, title);

            std::cout << "Subject: ";
            std::getline(std::cin, subject);

            std::cout << "Type: ";
            std::getline(std::cin, type);

            std::cout << "Due day: ";
            std::cin >> day;

            std::cout << "Due month: ";
            std::cin >> month;

            std::cout << "Due year: ";
            std::cin >> year;
            std::cin.ignore();

            dash.addEvent(Event(title, subject, type, Date(day, month, year)));
            dash.saveEventsToFile();

            std::cout << "Event added and saved to events.txt.\n";
            break;
        }
        case 5: {
            std::string title;
            int day = 0;
            int month = 0;
            int year = 0;

            std::cout << "Enter event title to amend: ";
            std::getline(std::cin, title);

            std::cout << "New due day: ";
            std::cin >> day;

            std::cout << "New due month: ";
            std::cin >> month;

            std::cout << "New due year: ";
            std::cin >> year;
            std::cin.ignore();

            dash.amendEvent(title, Date(day, month, year));
            dash.saveEventsToFile();

            std::cout << "events.txt updated.\n";
            break;
        }
        case 6: {
            std::string title;

            std::cout << "Enter event title to delete: ";
            std::getline(std::cin, title);

            if (dash.deleteEvent(title)) {
                dash.saveEventsToFile();
                std::cout << "Event deleted and events.txt updated.\n";
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
            dash.saveEventsToFile();

            std::cout << "events.txt updated.\n";
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

            std::cout << "Sort by 1. Date  2. Subject  3. Status: ";
            std::cin >> sortChoice;
            std::cin.ignore();

            dash.sortEvents(sortChoice, today);
            break;
        }
        case 11: {
            dash.saveEventsToFile();
            std::cout << "Events saved to events.txt. Goodbye!\n";
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter 1 to 11.\n";
            break;
        }
        }

    } while (choice != 11);

    return 0;
}
