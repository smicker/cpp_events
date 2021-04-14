/**
 * This worker will put all orders into a queue and will not stop working until all work from the Boss is done.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "WorkerWithHelpQueue.h"

namespace Company {
    WorkerWithHelpQueue::WorkerWithHelpQueue(Boss &boss) : mBoss(boss), mBossListener(*this) {
        mBoss.registerObserver(mBossListener);
        std::cout << "[WorkerWithHelpQueue] is hired!!\n";
    }

    void WorkerWithHelpQueue::work() {
        std::cout << "[WorkerWithHelpQueue] Starts workday!!\n";

        // Mutex locking is handled inside the WorkQueue functions.
        while (mBoss.isWorking() || !mWorkQueue.isEmpty()) {
            std::string work = mWorkQueue.wait_and_pop();
            std::cout << "[WorkerWithHelpQueue] Executing order: " << work << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        std::cout << "[WorkerWithHelpQueue] Ends workday!!\n";
    }

    void WorkerWithHelpQueue::BossListener::onEvent(const std::string &order) {
        // std::cout << "[BossListener] was notified about new order: " << order << "\n";

        // Note that this function is still executed in the Boss thread when Boss calls notify()!!
        // This means that you shall not do stuff here, you shall only raise a flag to the Worker
        // thread that the Worker thread shall take action. This can be done by adding the order
        // to a queue that is monitored by the Worker thread, or you can use condition variables.

        // The workqueue itself handles the mutex lock inside the push function.
        mWorker.mWorkQueue.push(order);
    }
}