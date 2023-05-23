#include "NotificationQueueAnalyzer.h"
#include "RandomNumberGenerationService.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>

template<typename T>
class NotificationQueueManipulator {
public:
    NotificationQueueManipulator(const NotificationQueue<T>& queue) : queue_{queue} {}

    void addNotifications(const T& notificationMessage) {
        while (this->isEnabled_.load()) {
            const std::chrono::seconds randomDuration = std::chrono::seconds(RandomNumberGenerationService::getRandomNumberInRange(1, 10));
            Notification<T> randomNotification = this->generateRandomNotification(notificationMessage);

            std::unique_lock<std::mutex> lock(this->mutex_);

            this->conditionVariable_.wait_for(lock, randomDuration, [this]{return !this->isEnabled_.load();});

            if (!this->isEnabled_.load()) {
                break;
            }

            try {
                this->queue_.addNotification(randomNotification);

                std::cout << "Queue size: " << this->queue_.getSize()
                          << " | Max queue size: " << this->queue_.getMaxSize() << std::endl;

                if (this->queue_.getSize() == this->queue_.getMaxSize()) {
                    this->isAnalysisRequested.store(true);

                    this->conditionVariable_.notify_all();
                }
            } catch (const std::length_error& e) {
                std::cerr << "Caught length_error: " << e.what() << std::endl;
            }
        }
    }

    void eraseExpiredNotifications() {
        while (this->isEnabled_.load()) {
            std::unique_lock<std::mutex> lock(this->mutex_);

            this->conditionVariable_.wait_for(lock, std::chrono::seconds(1), [this]{return !this->isEnabled_.load();});

            if (!this->isEnabled_.load()) {
                break;
            }

            this->queue_.eraseExpiredNotifications();
        }
    }

    void analyzeQueue() {
        while (this->isEnabled_.load()) {
            std::unique_lock<std::mutex> lock(this->mutex_);

            this->conditionVariable_.wait_for(lock, std::chrono::minutes(1), [this]{return this->isAnalysisRequested.load()
                                                                                           || !this->isEnabled_.load();});

            if (!this->isEnabled_.load()) {
                break;
            }

            try {
                NotificationQueueAnalyzer<T>::template analyzeQueue(this->queue_);
            } catch (const std::out_of_range& e) {
                std::cerr << "Caught out_of_range_error: " << e.what() << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Caught runtime_error: " << e.what() << std::endl;
            }
            
            if (this->isAnalysisRequested.load()) {
                this->isAnalysisRequested.store(false);
            }
        }
    }

    void enable() {
        this->isEnabled_.store(true);
    }

    void disable() {
        std::lock_guard<std::mutex> lock(this->mutex_);
        
        this->isEnabled_.store(false);
        this->isAnalysisRequested.store(false);

        this->conditionVariable_.notify_all();
    }

    static auto getAddNotificationsFunction(NotificationQueueManipulator<T>& manipulator, const T& message) {
        return [&manipulator, message](){manipulator.addNotifications(message);};
    }

    static auto getEraseExpiredNotificationsFunction(NotificationQueueManipulator<T>& manipulator) {
        return [&manipulator](){manipulator.eraseExpiredNotifications();};
    }

    static auto getAnalyzeQueueFunction(NotificationQueueManipulator<T>& manipulator) {
        return [&manipulator](){manipulator.analyzeQueue();};
    }
private:
    Notification<T> generateRandomNotification(const T& notificationMessage) {
        const Priority priority = getRandomNotificationPriority();
        const int randomNumberOfSeconds = RandomNumberGenerationService::getRandomNumberInRange(1, 30);
        const DateTime expirationDate = DateTime::getFutureDateTime(0, 0, 0, 0, 0, randomNumberOfSeconds);
        const T message = notificationMessage;

        return Notification(priority, expirationDate, message);
    }

    Priority getRandomNotificationPriority() {
        const Priority notificationPriorityTypes[3] {Priority::High, Priority::Medium, Priority::Low};

        return notificationPriorityTypes[RandomNumberGenerationService::getRandomNumberInRange(0, 2)];
    }

    NotificationQueue<T> queue_;
    std::condition_variable conditionVariable_;
    std::mutex mutex_;
    std::atomic<bool> isEnabled_ {false};
    std::atomic<bool> isAnalysisRequested {false};
};