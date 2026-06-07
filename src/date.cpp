#include "date.h"

#include <iostream>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay() const { return day; }

int Date::getMonth() const { return month; }

int Date::getYear() const { return year; }

long Date::toKey() const {
    return static_cast<long>(year) * 10000 + month * 100 + day;
}

// Exact count of days since the civil epoch (1970-01-01), accounting for
// leap years and varying month lengths. Based on Howard Hinnant's
// days_from_civil algorithm. Used for accurate date differences.
long Date::toDayNumber() const {
    int y = year - (month <= 2);
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);            // [0, 399]
    const unsigned doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5
                         + day - 1;                                       // [0, 365]
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;          // [0, 146096]
    return static_cast<long>(era) * 146097 + static_cast<long>(doe) - 719468;
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
