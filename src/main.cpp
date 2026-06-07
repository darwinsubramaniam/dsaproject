// =====================================================================
// MECS0023 DATA STRUCTURE & ALGORITHM - MINI PROJECT
// E-Learning Dashboard using events.csv
//
// Entry point: loads events from events.csv and runs the menu loop.
// =====================================================================

#include <iostream>
#include <string>
#include <vector>

#include "dashboard.h"
#include "date.h"
#include "event.h"
#include "store.h"
#include "ui.h"

// Outcome of a cancellable prompt.
enum class Input { Ok, Cancel, Closed };

// Read a line of text. An empty line cancels; a closed stream (Ctrl+D) ends.
Input readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!std::getline(std::cin, out)) {
        return Input::Closed;
    }
    if (out.empty()) {
        return Input::Cancel;
    }
    return Input::Ok;
}

// Read an integer, re-prompting on non-numeric input. An empty line cancels;
// a closed stream ends. (Ctrl+C terminates the program directly.)
Input readInt(const std::string& prompt, int& value) {
    while (true) {
        std::string line;
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            return Input::Closed;
        }
        if (line.empty()) {
            return Input::Cancel;
        }
        try {
            std::size_t pos = 0;
            value = std::stoi(line, &pos);
            return Input::Ok;
        } catch (const std::exception&) {
            std::cout << "You entered wrong input. Please enter a number.\n";
        }
    }
}

// Prompt for a day/month/year and build a Date. Propagates cancel/close.
Input readDate(const std::string& label, Date& out) {
    int day = 0;
    int month = 0;
    int year = 0;

    Input r = readInt(label + " day (blank to cancel): ", day);
    if (r != Input::Ok) return r;
    r = readInt(label + " month: ", month);
    if (r != Input::Ok) return r;
    r = readInt(label + " year: ", year);
    if (r != Input::Ok) return r;

    out = Date(day, month, year);
    return Input::Ok;
}

// ----------------------------- main ----------------------------------
int main() {
    Dashboard dash;
    const Date today = Date::today();

    store::load(dash);

    const std::vector<std::string> menuOptions = {
        "View Timeline (sorted by due date)",
        "View Calendar (grouped by date)",
        "Search Event",
        "Add Event",
        "Amend Event (change due date)",
        "Delete Event",
        "Mark Event Complete",
        "View Reminders (queue)",
        "View Overdue Alerts (stack)",
        "Sort Events (date / subject / status)",
        "Progress Report (completed vs pending)",
        "Exit",
    };

    bool running = true;

    // Handle a cancellable prompt result: returns true if the current action
    // should be aborted (user pressed Enter on an empty line, or input closed).
    auto aborted = [&running](Input r) -> bool {
        if (r == Input::Closed) {
            running = false;
            return true;
        }
        if (r == Input::Cancel) {
            std::cout << "Cancelled.\n";
            return true;
        }
        return false;
    };

    while (running) {
        ui::clearScreen();  // keep a single menu in place instead of scrolling

        int picked = ui::menu("E-LEARNING DASHBOARD", menuOptions);
        if (picked < 0) {
            picked = 11;  // q/Esc cancels the menu -> treat as Exit
        }

        const int choice = picked + 1;  // cases below are numbered 1..12

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
            if (aborted(readLine("Enter title or subject (blank to cancel): ",
                                 keyword))) {
                break;
            }

            dash.searchEvent(keyword);
            break;
        }
        case 4: {
            std::string title;
            std::string subject;
            std::string type;

            if (aborted(readLine("Title (blank to cancel): ", title))) break;
            if (aborted(readLine("Subject: ", subject))) break;
            if (aborted(readLine("Type: ", type))) break;

            Date dueDate;
            if (aborted(readDate("Due", dueDate))) break;

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
            std::vector<Event> events = dash.events();
            if (events.empty()) {
                std::cout << "No events to amend.\n";
                break;
            }

            int idx = ui::selectEvent("Select an event to AMEND:", events, today);
            if (idx < 0) {
                std::cout << "Cancelled.\n";
                break;
            }

            const std::string title = events[idx].title;

            Date newDate;
            if (aborted(readDate("New due", newDate))) break;

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
            std::vector<Event> events = dash.events();
            if (events.empty()) {
                std::cout << "No events to delete.\n";
                break;
            }

            int idx = ui::selectEvent("Select an event to DELETE:", events, today);
            if (idx < 0) {
                std::cout << "Cancelled.\n";
                break;
            }

            dash.deleteEvent(events[idx].title);
            store::save(dash);
            std::cout << "Event deleted and events.csv updated.\n";
            break;
        }
        case 7: {
            std::vector<Event> events = dash.events();
            if (events.empty()) {
                std::cout << "No events to mark complete.\n";
                break;
            }

            int idx = ui::selectEvent("Select an event to MARK COMPLETE:", events, today);
            if (idx < 0) {
                std::cout << "Cancelled.\n";
                break;
            }

            dash.markComplete(events[idx].title);
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
            const std::vector<std::string> sortOptions = {
                "By Due Date", "By Subject", "By Status"};

            int sortPick = ui::menu("Sort events:", sortOptions);
            if (sortPick < 0) {
                std::cout << "Cancelled.\n";
                break;
            }

            dash.sortEvents(sortPick + 1, today);  // sortEvents expects 1/2/3
            break;
        }
        case 11: {
            std::cout << "\n===== PROGRESS REPORT =====\n";
            dash.showProgress(today);
            break;
        }
        case 12: {
            store::save(dash);
            std::cout << "Events saved to events.csv. Goodbye!\n";
            running = false;
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter 1 to 12.\n";
            break;
        }
        }

        // Let the user read the output before the menu redraws. (Only in an
        // interactive terminal; pipes/tests just continue.)
        if (running && ui::interactive()) {
            std::cout << "\nPress Enter to return to the menu...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }

    return 0;
}
