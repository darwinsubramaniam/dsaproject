// =====================================================================
// MECS0023 DATA STRUCTURE & ALGORITHM - MINI PROJECT
// E-Learning Dashboard using events.csv
//
// Entry point: loads events from events.csv and runs the menu loop.
// =====================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dashboard.h"
#include "date.h"
#include "event.h"
#include "rapidcsv.h"

// ---------------------- Load from events.csv --------------------------
// CSV columns: title,subject,type,day,month,year,completed
// The "completed" column is optional (defaults to not completed).
void loadEventsFromFile(Dashboard& dash) {
    std::ifstream probe("events.csv");

    if (!probe) {
        std::cout << "events.csv not found.\n";
        std::cout << "It will be created when you add or save events.\n";
        return;
    }
    probe.close();

    rapidcsv::Document doc("events.csv", rapidcsv::LabelParams(0, -1));

    std::vector<std::string> columns = doc.GetColumnNames();
    bool hasCompleted =
        std::find(columns.begin(), columns.end(), "completed") != columns.end();

    for (std::size_t i = 0; i < doc.GetRowCount(); i++) {
        try {
            Event e(doc.GetCell<std::string>("title", i),
                    doc.GetCell<std::string>("subject", i),
                    doc.GetCell<std::string>("type", i),
                    Date(doc.GetCell<int>("day", i),
                         doc.GetCell<int>("month", i),
                         doc.GetCell<int>("year", i)));

            if (hasCompleted) {
                e.setCompleted(doc.GetCell<int>("completed", i) == 1);
            }

            dash.addEvent(e);
        } catch (const std::exception&) {
            continue;  // skip malformed rows
        }
    }

    std::cout << "Events loaded from events.csv.\n";
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
        std::cout << " -- Manage Event Status --\n";
        std::cout << " 6. Mark Event Complete\n";
        std::cout << " 7. View Reminders (queue)\n";
        std::cout << " 8. View Overdue Alerts (stack)\n";
        std::cout << " -- Sort Event --\n";
        std::cout << " 9. Sort Events (date / subject / status)\n";
        std::cout << "10. Exit\n";
        std::cout << "------------------------------------------\n";
        std::cout << " Enter your choice (1-10): ";
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

            std::cout << "Event added and saved to events.csv.\n";
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

            std::cout << "events.csv updated.\n";
            break;
        }
        case 6: {
            std::string title;

            std::cout << "Enter event title to mark complete: ";
            std::getline(std::cin, title);

            dash.markComplete(title);
            dash.saveEventsToFile();

            std::cout << "events.csv updated.\n";
            break;
        }
        case 7: {
            dash.showReminders();
            break;
        }
        case 8: {
            dash.checkDeadlines(today);
            dash.showOverdue();
            break;
        }
        case 9: {
            int sortChoice = 0;

            std::cout << "Sort by 1. Date  2. Subject  3. Status: ";
            std::cin >> sortChoice;
            std::cin.ignore();

            dash.sortEvents(sortChoice, today);
            break;
        }
        case 10: {
            dash.saveEventsToFile();
            std::cout << "Events saved to events.csv. Goodbye!\n";
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter 1 to 10.\n";
            break;
        }
        }

    } while (choice != 10);

    return 0;
}
