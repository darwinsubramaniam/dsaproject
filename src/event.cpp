#include "event.h"

#include <utility>

Event::Event(std::string t, std::string s, std::string ty, const Date& d)
    : title(std::move(t)), subject(std::move(s)), type(std::move(ty)), dueDate(d) {}

const std::string& Event::getTitle() const { return title; }

const std::string& Event::getSubject() const { return subject; }

const std::string& Event::getType() const { return type; }

const Date& Event::getDueDate() const { return dueDate; }

bool Event::isCompleted() const { return completed; }

void Event::setDueDate(const Date& d) { dueDate = d; }

void Event::setCompleted(bool value) { completed = value; }

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
