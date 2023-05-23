#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include "DateTime.h"

enum struct Priority {Low = 0, Medium = 50, High = 100};

template<typename T>
class Notification {
public:
    Notification(const Priority& priority, const DateTime& expirationDate, const T& message)
        : priority_{priority}, expirationDate_{expirationDate}, message_{message} {}

    Priority getPriority() const {
        return this->priority_;
    }

    DateTime getExpirationDate() const {
        return this->expirationDate_;
    }

    T getMessage() const {
        return this->message_;
    }

    bool isExpired() const {
        return this->expirationDate_.getDateTime() < std::chrono::system_clock::now();
    }
private:
    Priority priority_;
    DateTime expirationDate_;
    T message_;
};

#endif