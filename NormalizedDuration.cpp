#include "NormalizedDuration.h"
#include <sstream>
#include <cstdlib>

NormalizedDuration::NormalizedDuration(const int& years, const int& months, const int& days, const int& hours, const int& minutes, const int& seconds)
    : years_{years}, months_{months}, days_{days}, hours_{hours}, minutes_{minutes}, seconds_{seconds} {}

namespace {
    template <typename T>
    int extractDurationCountByType(std::chrono::seconds& duration) {
        if (duration.count()) {
            const T convertedDurationByType = std::chrono::duration_cast<T>(duration);
            duration -= convertedDurationByType;

            return static_cast<int>(abs(convertedDurationByType.count()));
        } else {
            return 0;
        } 
    }
}

NormalizedDuration NormalizedDuration::getNormalizedDuration(std::chrono::seconds& duration) {
    const int years = extractDurationCountByType<std::chrono::years>(duration);
    const int months = extractDurationCountByType<std::chrono::months>(duration);
    const int days = extractDurationCountByType<std::chrono::days>(duration);
    const int hours = extractDurationCountByType<std::chrono::hours>(duration);
    const int minutes = extractDurationCountByType<std::chrono::minutes>(duration);
    const int seconds = extractDurationCountByType<std::chrono::seconds>(duration);

    return NormalizedDuration {years, months, days, hours, minutes, seconds};
}

std::string NormalizedDuration::convertNormalizedDurationToString(const NormalizedDuration& normalizedDuration) {
    std::ostringstream normalizedDurationOStringStream;
    normalizedDurationOStringStream << normalizedDuration.years_ << " years, "
                                    << normalizedDuration.months_ << " months, "
                                    << normalizedDuration.days_ << " days, "
                                    << normalizedDuration.hours_ << " hours, "
                                    << normalizedDuration.minutes_ << " minutes, "
                                    << normalizedDuration.seconds_ << " seconds";
                                    
    return normalizedDurationOStringStream.str();
}