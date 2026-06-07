#ifndef EVENT_H
#define EVENT_H

#include <string>

#include "date.h"

// ------------------------- class Event -------------------------------
class Event {
private:
    std::string title;
    std::string subject;
    std::string type;
    Date dueDate;
    bool completed = false;

public:
    Event() = default;
    Event(std::string t, std::string s, std::string ty, const Date& d);

    const std::string& getTitle() const;
    const std::string& getSubject() const;
    const std::string& getType() const;
    const Date& getDueDate() const;
    bool isCompleted() const;

    void setDueDate(const Date& d);
    void setCompleted(bool value);

    std::string getStatus(const Date& today) const;
    int statusRank(const Date& today) const;
};

#endif  // EVENT_H
