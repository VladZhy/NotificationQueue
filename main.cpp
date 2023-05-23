#include "NotificationQueue.h"
#include "NotificationQueueManipulator.h"
#include <iostream>
#include <thread>

int main() {
    NotificationQueue<std::string> notificationQueue {10};
    NotificationQueueManipulator<std::string> notificationQueueManipulator(notificationQueue);

    const auto firstAddNotificationsFunction = NotificationQueueManipulator<std::string>::getAddNotificationsFunction
                                               (notificationQueueManipulator, "Notification from the first thread.");
    const auto secondAddNotificationsFunction = NotificationQueueManipulator<std::string>::getAddNotificationsFunction
                                               (notificationQueueManipulator, "Notification from the second thread.");
    const auto thirdAddNotificationsFunction = NotificationQueueManipulator<std::string>::getAddNotificationsFunction
                                               (notificationQueueManipulator, "Notification from the third thread.");
    const auto eraseExpiredNotificationsFunction = NotificationQueueManipulator<std::string>::getEraseExpiredNotificationsFunction
                                                   (notificationQueueManipulator);
    const auto analyzeQueueFunction = NotificationQueueManipulator<std::string>::getAnalyzeQueueFunction(notificationQueueManipulator);

    notificationQueueManipulator.enable();

    std::thread addNotificationsThread_1(firstAddNotificationsFunction);
    std::thread addNotificationsThread_2(secondAddNotificationsFunction);
    std::thread addNotificationsThread_3(thirdAddNotificationsFunction);
    std::thread eraseExpiredNotificationsThread(eraseExpiredNotificationsFunction);
    std::thread analyzeQueueThread(analyzeQueueFunction);

    std::this_thread::sleep_for(std::chrono::minutes(5));

    notificationQueueManipulator.disable();

    addNotificationsThread_1.join();
    addNotificationsThread_2.join();
    addNotificationsThread_3.join();
    eraseExpiredNotificationsThread.join();
    analyzeQueueThread.join();

    std::cout << "Notification Queue Analyzer was called "
              << NotificationQueueAnalyzer<std::string>::getAnalyzeQueueRequestsCount()
              << " times."
              << std::endl;

    return 0;
}