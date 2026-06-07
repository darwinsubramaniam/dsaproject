#ifndef EVENT_H
#define EVENT_H

#include <string>

#include "date.h"

// ------------------------- struct Event ------------------------------
// Plain data record describing one e-learning event. Fields are public;
// construct with aggregate initialization, e.g.
//     Event e{"Quiz 1", "Math", "Quiz", Date(1, 6, 2026)};
struct Event {
    // Enumerator order is the "sort by status" priority: overdue first,
    // upcoming next, completed last.
    enum class Status { Overdue, Upcoming, Completed };

    std::string title;
    std::string subject;
    std::string type;
    Date dueDate;
    bool completed = false;

    // Behaviour derived from the data.
    Status status(const Date& today) const;       // the state
    std::string statusLabel(const Date& today) const;  // human-readable text
};

#endif  // EVENT_H
