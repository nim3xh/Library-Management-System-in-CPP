#include "Date.h"

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

bool Date::isPast(const Date& other) const {
    if (year < other.getYear())
        return true;
    else if (year == other.getYear() && month < other.getMonth())
        return true;
    else if (year == other.getYear() && month == other.getMonth() && day < other.getDay())
        return true;
    return false;
}
