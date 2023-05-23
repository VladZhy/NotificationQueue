#include "DateTime.h"
#include "NormalizedDuration.h"
#include <ctime>
#include <sstream>
#include <iomanip>

DateTime::DateTime(const int& year, const int& month, const int& day, const int& hours, const int& minutes, const int& seconds)
    : year_{year}, month_{month}, day_{day}, hours_{hours}, minutes_{minutes}, seconds_{seconds}, dateTime_{} {
        this->setDateTime();
}

bool DateTime::operator<(const DateTime& rhsDateTime) const {
    return this->dateTime_ < rhsDateTime.dateTime_;
}

bool DateTime::operator>(const DateTime& rhsDateTime) const {
    return this->dateTime_ > rhsDateTime.dateTime_;
}

std::chrono::seconds DateTime::operator-(const DateTime& rhsDateTime) const {
    return std::chrono::duration_cast<std::chrono::seconds>(this->dateTime_ - rhsDateTime.dateTime_);
}

void DateTime::setDateTime() {
    std::tm dateTimeStruct = {this->seconds_, this->minutes_, this->hours_, this->day_, this->month_ - 1, this->year_ - 1900, 0, 0, -1};
    this->dateTime_ = std::chrono::system_clock::from_time_t(std::mktime(&dateTimeStruct));
}

std::chrono::system_clock::time_point DateTime::getDateTime() const {
    return this->dateTime_;
}

namespace {
    void assignLocalTimeToDateTimeStruct(std::tm& dateTimeStruct, const std::time_t& time) {
        #ifdef _WIN32
            localtime_s(&dateTimeStruct, &time);
        #else
            localtime_r(&time, &dateTimeStruct);
        #endif
    }

    std::string convertDurationToString(std::chrono::seconds& duration) {
        const NormalizedDuration normalizedDuration = NormalizedDuration::getNormalizedDuration(duration);
        
        return NormalizedDuration::convertNormalizedDurationToString(normalizedDuration);
    }
}

std::string DateTime::getCurrentDateTimeString() {
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm currentDateTimeStruct {};

    assignLocalTimeToDateTimeStruct(currentDateTimeStruct, currentTime);

    std::ostringstream currentDateTimeOStringStream;
    currentDateTimeOStringStream << std::put_time(&currentDateTimeStruct, "%F %T");

    return currentDateTimeOStringStream.str();
}

std::string DateTime::getDifferenceBetweenDateTimes(const DateTime& firstDateTime, const DateTime& secondDateTime) {
    std::chrono::seconds durationBetweenDateTimes = firstDateTime - secondDateTime;
    
    return convertDurationToString(durationBetweenDateTimes);
}

DateTime DateTime::getFutureDateTime(const int& years, const int& months, const int& days, const int& hours, const int& minutes, const int& seconds) {
    const std::chrono::duration<long long> futureDuration = std::chrono::years{years}
                                                            + std::chrono::months{months}
                                                            + std::chrono::days{days}
                                                            + std::chrono::hours{hours}
                                                            + std::chrono::minutes{minutes}
                                                            + std::chrono::seconds{seconds};                                              
    const std::time_t futureTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + futureDuration);
    std::tm futureDateTimeStruct{};

    assignLocalTimeToDateTimeStruct(futureDateTimeStruct, futureTime);

    return DateTime(futureDateTimeStruct.tm_year + 1900,
                    futureDateTimeStruct.tm_mon + 1,
                    futureDateTimeStruct.tm_mday,
                    futureDateTimeStruct.tm_hour,
                    futureDateTimeStruct.tm_min,
                    futureDateTimeStruct.tm_sec);
}