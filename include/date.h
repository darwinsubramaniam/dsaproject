#ifndef DATE_H
#define DATE_H

#include <chrono>

// ------------------------- class Date --------------------------------
// Calendar date backed by std::chrono::year_month_day (C++20), so the
// standard library owns the calendar arithmetic, comparison, and formatting.
class Date {
private:
    std::chrono::year_month_day ymd{
        std::chrono::year{2026}, std::chrono::January, std::chrono::day{1}};

public:
    Date() = default;
    Date(int d, int m, int y);

    // The current (UTC) calendar date.
    static Date today();

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    long toKey() const;
    long toDayNumber() const;

    int compareTo(const Date& other) const;
    void display() const;

    // True if this is a real calendar date (e.g. rejects 31 Feb).
    bool isValid() const;
};

#endif  // DATE_H
