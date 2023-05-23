#ifndef DATE_TIME_H
#define DATE_TIME_H
#include <chrono>
#include <string>

class DateTime {
public:
    DateTime(const int& year, const int& month, const int& day, const int& hours, const int& minutes, const int& seconds);
    bool operator<(const DateTime& rhsDateTime) const;
    bool operator>(const DateTime& rhsDateTime) const;
    std::chrono::seconds operator-(const DateTime& rhsDateTime) const;
    void setDateTime();
    std::chrono::system_clock::time_point getDateTime() const;
    static std::string getCurrentDateTimeString();
    static std::string getDifferenceBetweenDateTimes(const DateTime& firstDateTime, const DateTime& secondDateTime);
    static DateTime getFutureDateTime(const int& years = 0, const int& months = 0, const int& days = 0, const int& hours = 0, const int& minutes = 0, const int& seconds = 0);
private:
    int year_;
    int month_;
    int day_;
    int hours_;
    int minutes_;
    int seconds_;
    std::chrono::system_clock::time_point dateTime_;
};

#endif