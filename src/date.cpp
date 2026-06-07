#include "date.h"

#include <format>
#include <iostream>

Date::Date(int d, int m, int y)
    : ymd{std::chrono::year{y},
          std::chrono::month{static_cast<unsigned>(m)},
          std::chrono::day{static_cast<unsigned>(d)}} {}

Date Date::today() {
    using namespace std::chrono;
    const year_month_day ymd{floor<days>(system_clock::now())};
    return Date(static_cast<int>(static_cast<unsigned>(ymd.day())),
                static_cast<int>(static_cast<unsigned>(ymd.month())),
                static_cast<int>(ymd.year()));
}

int Date::getDay() const {
    return static_cast<int>(static_cast<unsigned>(ymd.day()));
}

int Date::getMonth() const {
    return static_cast<int>(static_cast<unsigned>(ymd.month()));
}

int Date::getYear() const {
    return static_cast<int>(ymd.year());
}

long Date::toKey() const {
    return getYear() * 10000L + getMonth() * 100 + getDay();
}

long Date::toDayNumber() const {
    return std::chrono::sys_days{ymd}.time_since_epoch().count();
}

int Date::compareTo(const Date& other) const {
    const auto a = std::chrono::sys_days{ymd};
    const auto b = std::chrono::sys_days{other.ymd};
    return (a > b) - (a < b);
}

void Date::display() const {
    std::cout << std::format("{:%d/%m/%Y}", ymd);
}

bool Date::isValid() const {
    return ymd.ok();
}

std::string Date::weekday() const {
    const std::chrono::weekday wd{std::chrono::sys_days{ymd}};
    return std::format("{:%A}", wd);
}
