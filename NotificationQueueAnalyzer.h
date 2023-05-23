#ifndef NOTIFICATION_QUEUE_ANALYZER_H
#define NOTIFICATION_QUEUE_ANALYZER_H
#include "NotificationQueue.h"
#include "FilesystemService.h"
#include "Notification.h"
#include <fstream>
#include <iomanip>
#include <map>

template<typename T>
class NotificationQueueAnalyzer {
public:
    static void analyzeQueue(const NotificationQueue<T>& queue) {
        analyzeQueueRequestsCount_++;

        if (!queue.getSize()) {
            throw std::out_of_range("The selected notification queue is empty.");
        }
        
        std::ofstream queueAnalysisResultFile(FilesystemService::generateFilePath("NotificationQueueAnalyzerResult", ".txt", "./NotificationQueueAnalyzerResults"));

        if (!queueAnalysisResultFile.is_open()) {
            throw std::runtime_error("Failed to create file for recording results.");
        }

        recordQueueAnalysisTimeToFile(queueAnalysisResultFile);
        recordQueueSizeToFile(queueAnalysisResultFile, queue);
        recordQueueSizeInKbToFile(queueAnalysisResultFile, queue);
        recordQueuePriorityRatioToFile(queueAnalysisResultFile, queue);
        recordMaxNotificationExpirationDateDifferenceToFile(queueAnalysisResultFile, queue);

        queueAnalysisResultFile.close();

        std::cout << "Queue analysis completed successfully." << std::endl;
    }

    static int getAnalyzeQueueRequestsCount() {
        return analyzeQueueRequestsCount_;
    }
private:
    static void recordQueueAnalysisTimeToFile(std::ofstream& file) {
        file << "Queue analysis time: " << DateTime::getCurrentDateTimeString() << "\n";
    }

    static void recordQueueSizeToFile(std::ofstream& file, const NotificationQueue<T>& queue) {
        file << "Size: " << std::to_string(queue.getSize()) << "\n";
    }

    static void recordQueueSizeInKbToFile(std::ofstream& file, const NotificationQueue<T>& queue) {
        file << "Size in KB: " << std::to_string(calculateQueueSizeInKb(queue)) << "\n";
    }

    static double calculateQueueSizeInKb(const NotificationQueue<T>& queue) {
        return sizeof(Notification<T>) * queue.getSize() / 1024.0;
    }

    static void recordQueuePriorityRatioToFile(std::ofstream& file, const NotificationQueue<T>& queue) {
        const std::string queuePriorityTypes[3] {"High", "Medium", "Low"};
        std::map<std::string, double> queuePriorityRatioMap = getQueuePriorityRatioMap(queue);

        file << "Priority ratio: " << "\n";

        for (const std::string& queuePriorityType : queuePriorityTypes) {
            file << " - "  << queuePriorityType << ": " << std::fixed << std::setprecision(2)
                 << queuePriorityRatioMap[queuePriorityType] << "%" << "\n";
        }
    }

    static std::map<std::string, double> getQueuePriorityRatioMap(const NotificationQueue<T>& queue) {
        std::map<std::string, size_t> queuePriorityCountMap = getQueuePriorityCountMap(queue);
        const size_t queueSize = queue.getSize();
        std::map<std::string, double> queuePriorityRatioMap {};
        queuePriorityRatioMap["High"] = calculateQueuePriorityRatio(queuePriorityCountMap["High"], queueSize);
        queuePriorityRatioMap["Medium"] = calculateQueuePriorityRatio(queuePriorityCountMap["Medium"], queueSize);
        queuePriorityRatioMap["Low"] = calculateQueuePriorityRatio(queuePriorityCountMap["Low"], queueSize);

        return queuePriorityRatioMap;
    }

    static std::map<std::string, size_t> getQueuePriorityCountMap(const NotificationQueue<T>& queue) {
        size_t highPriorityCount {};
        size_t mediumPriorityCount {};
        size_t lowPriorityCount {};

        for (const auto& notification : queue.container_) {
            switch (notification.getPriority()) {
                case Priority::High:
                    highPriorityCount++;
                    break;
                case Priority::Medium:
                    mediumPriorityCount++;
                    break;
                case Priority::Low:
                    lowPriorityCount++;
                    break;
            }
        }

        return std::map<std::string, size_t> {{"High", highPriorityCount}, {"Medium", mediumPriorityCount}, {"Low", lowPriorityCount}};
    }

    static double calculateQueuePriorityRatio(const size_t& priorityCount, const size_t& queueSize) {
        return priorityCount * 100.0 / queueSize;
    }

    static void recordMaxNotificationExpirationDateDifferenceToFile(std::ofstream& file, const NotificationQueue<T>& queue) {
        file << "Maximum difference between expiration dates: " << getMaxNotificationExpirationDateDifference(queue) << "\n";
    }

    static std::string getMaxNotificationExpirationDateDifference(const NotificationQueue<T>& queue) {
        const auto minMaxExpirationDateNotifications = std::minmax_element(queue.container_.begin(), queue.container_.end(),
                                                       [](const Notification<T>& notificationA, const Notification<T>& notificationB)
                                                       {return notificationA.getExpirationDate() < notificationB.getExpirationDate();});
                                                 
        const DateTime minExpirationDate = minMaxExpirationDateNotifications.first->getExpirationDate();
        const DateTime maxExpirationDate = minMaxExpirationDateNotifications.second->getExpirationDate();
        
        return DateTime::getDifferenceBetweenDateTimes(minExpirationDate, maxExpirationDate);
    }

    static int analyzeQueueRequestsCount_;
};

template<typename T>
int NotificationQueueAnalyzer<T>::analyzeQueueRequestsCount_ = 0;

#endif