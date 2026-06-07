#include "date.h"

#include <chrono>
#include <iostream>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay() const { return day; }

int Date::getMonth() const { return month; }

int Date::getYear() const { return year; }

long Date::toKey() const {
    return static_cast<long>(year) * 10000 + month * 100 + day;
}

// Exact count of days since the civil epoch (1970-01-01), via the C++20
// chrono calendar. Used for accurate date differences.
long Date::toDayNumber() const {
    using namespace std::chrono;
    const year_month_day ymd{std::chrono::year{year},
                             std::chrono::month{static_cast<unsigned>(month)},
                             std::chrono::day{static_cast<unsigned>(day)}};
    return sys_days{ymd}.time_since_epoch().count();
}

int Date::compareTo(const Date& other) const {
    if (toKey() < other.toKey()) {
        return -1;
    } else if (toKey() > other.toKey()) {
        return 1;
    } else {
        return 0;
    }
}

void Date::display() const {
    if (day < 10) {
        std::cout << "0";
    }

    std::cout << day << "/";

    if (month < 10) {
        std::cout << "0";
    }

    std::cout << month << "/" << year;
}
