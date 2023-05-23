#ifndef NORMALIZED_DURATION_H
#define NORMALIZED_DURATION_H
#include <chrono>
#include <string>

class NormalizedDuration {
public:
    NormalizedDuration(const int& years, const int& months, const int& days, const int& hours, const int& minutes, const int& seconds);
    static NormalizedDuration getNormalizedDuration(std::chrono::seconds& duration);
    static std::string convertNormalizedDurationToString(const NormalizedDuration& normalizedDuration);
private:
    int years_;
    int months_;
    int days_;
    int hours_;
    int minutes_;
    int seconds_;
};

#endif