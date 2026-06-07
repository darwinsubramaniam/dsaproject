#include "event.h"

Event::Status Event::status(const Date& today) const {
    if (completed) {
        return Status::Completed;
    }

    long diff = dueDate.toDayNumber() - today.toDayNumber();
    return diff < 0 ? Status::Overdue : Status::Upcoming;
}

std::string Event::statusLabel(const Date& today) const {
    switch (status(today)) {
        case Status::Completed:
            return "Completed";
        case Status::Overdue:
            return "Past Deadline";
        case Status::Upcoming: {
            long diff = dueDate.toDayNumber() - today.toDayNumber();
            if (diff == 0) {
                return "Upcoming Today";
            }
            return "Upcoming (" + std::to_string(diff) + " days)";
        }
    }

    return "";  // unreachable; all enumerators handled above
}
