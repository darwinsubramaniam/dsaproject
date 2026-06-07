#include "event.h"

std::string Event::getStatus(const Date& today) const {
    if (completed) {
        return "Completed";
    }

    long diff = dueDate.toDayNumber() - today.toDayNumber();

    if (diff < 0) {
        return "Past Deadline";
    } else if (diff == 0) {
        return "Upcoming Today";
    } else {
        return "Upcoming (" + std::to_string(diff) + " days)";
    }
}

// Ordering key for "sort by status": overdue (0) first, then upcoming (1),
// then completed (2) last.
int Event::statusRank(const Date& today) const {
    if (completed) {
        return 2;
    }

    long diff = dueDate.toDayNumber() - today.toDayNumber();

    if (diff < 0) {
        return 0;
    } else {
        return 1;
    }
}
