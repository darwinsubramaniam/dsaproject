#ifndef DATE_H
#define DATE_H

// ------------------------- class Date --------------------------------
class Date {
private:
    int day = 1;
    int month = 1;
    int year = 2026;

public:
    Date() = default;
    Date(int d, int m, int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    long toKey() const;
    long toDayNumber() const;

    int compareTo(const Date& other) const;
    void display() const;
};

#endif  // DATE_H
