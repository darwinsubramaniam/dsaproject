#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <string>

#include "date.h"
#include "event.h"
#include "node.h"
#include "overdue_stack.h"
#include "reminder_queue.h"

// ------------------------- Dashboard ---------------------------------
// Owns a date-sorted singly-linked list of events, plus a reminder queue
// and an overdue-alert stack.
class Dashboard {
private:
    Node* head = nullptr;
    ReminderQueue reminders;
    OverdueStack alerts;

    void printPadded(const std::string& text, int width) const;

public:
    Dashboard() = default;
    ~Dashboard();

    // Non-copyable: the class owns a raw linked list.
    Dashboard(const Dashboard&) = delete;
    Dashboard& operator=(const Dashboard&) = delete;

    void addEvent(const Event& e);
    bool deleteEvent(const std::string& title);
    void amendEvent(const std::string& title, const Date& newDate);

    void checkDeadlines(const Date& today);
    void displayTimeline(const Date& today);
    void displayCalendar() const;
    void searchEvent(const std::string& keyword) const;
    void markComplete(const std::string& title);

    void showReminders() const;
    void showOverdue() const;

    void sortEvents(int choice, const Date& today) const;
    void saveEventsToFile() const;
};

#endif  // DASHBOARD_H
