#ifndef NOTIFICATION_QUEUE_H
#define NOTIFICATION_QUEUE_H
#include "Notification.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <deque>

template<typename T>
class NotificationQueue {
public:
    NotificationQueue(const size_t& maxSize) : maxSize_{maxSize}, container_{} {}

    void addNotification(const Notification<T>& newNotification) {
        if (this->container_.size() < this->maxSize_) {
            this->insertNotification(newNotification);
        } else {
            if(this->eraseExpiredNotifications()) {
                this->addNotification(newNotification);
            } else {
                throw std::length_error("Not enough space in the selected notification queue.");
            }
        }
    }

    Notification<T> getNotification() {
        if (this->container_.empty()) {
            throw std::out_of_range("The selected notification queue is empty.");
        }

        if (!this->container_.front().isExpired()) {
            Notification<T> notificationToReturn = this->container_.front();
            
            this->container_.pop_front();

            return notificationToReturn;
        } else {
            this->eraseExpiredNotifications();

            return this->getNotification();
        } 
    }

    size_t getSize() const {
        return this->container_.size();
    }

    size_t getMaxSize() const {
        return this->maxSize_;
    }
    
    bool eraseExpiredNotifications() {
        const auto notificationsToErase = std::remove_if(this->container_.begin(), this->container_.end(),
                                          [](const Notification<T>& notification) { return notification.isExpired();});
        const int erasedNotificationsCount = static_cast<int>(std::distance(notificationsToErase, this->container_.end()));

        this->container_.erase(notificationsToErase, this->container_.end());

        if (erasedNotificationsCount == 1) {
            std::cout << "1 notification has been erased." << std::endl;
        } else {
            std::cout << erasedNotificationsCount << " notifications have been erased." << std::endl;
        }

        return erasedNotificationsCount > 0;
    }
private:
    void insertNotification(const Notification<T>& newNotification) {
        this->container_.push_back(newNotification);

        std::stable_sort(this->container_.begin(), this->container_.end(),
                         [](const Notification<T>& notificationA, const Notification<T>& notificationB)
                         {return notificationA.getPriority() > notificationB.getPriority();});

        std::cout << "Notification added. Message: " << newNotification.getMessage() << std::endl;
    }

    
    template<typename T>
    friend class NotificationQueueAnalyzer;

    size_t maxSize_;
    std::deque<Notification<T>> container_;
};

#endif