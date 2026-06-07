#ifndef EVENT_H
#define EVENT_H

#include <string>

#include "date.h"

// ------------------------- struct Event ------------------------------
// Plain data record describing one e-learning event. Fields are public;
// construct with aggregate initialization, e.g.
//     Event e{"Quiz 1", "Math", "Quiz", Date(1, 6, 2026)};
struct Event {
    std::string title;
    std::string subject;
    std::string type;
    Date dueDate;
    bool completed = false;

    // Behaviour derived from the data.
    std::string getStatus(const Date& today) const;
    int statusRank(const Date& today) const;
};

#endif  // EVENT_H
