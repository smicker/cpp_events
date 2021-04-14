/**
 * This worker will just execute the tasks he has time with and will stop working when Boss stops working.
 */


#include <iostream>
#include <thread>
#include <chrono>
#include "Worker.h"

namespace Company {
    Worker::Worker(Boss &boss) : mBoss(boss), mBossListener(*this) {
        mBoss.registerObserver(mBossListener);
        std::cout << "[Worker] is hired!!\n";
    }

    void Worker::work() {
        int ordersDone = 0;
        std::cout << "[Worker] Starts workday!!\n";

        while ((ordersDone < ORDER_PER_DAY) && mBoss.isWorking()) {
            std::unique_lock<std::mutex> lock(mMutex);
            mOrderAvailable.wait(lock);
            // Worker owns the mutex here so we can safely read shared variable "lastOrder".
            std::cout << "[Worker] Executing order: " << mLastOrder << "\n";

            // If this Worker thread is taking long time we need to unlock here since if the Boss thread
            // calls onEvent the Boss thread will halt on the lock_guard until the lock is released and
            // we don't want to let the Boss wait.
            lock.unlock();

            ordersDone++;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        std::cout << "[Worker] Ends workday!!\n";
    }

    void Worker::BossListener::onEvent(const std::string &order) {
        // std::cout << "[BossListener] was notified about new order: " << order << "\n";

        // Note that this function is still executed in the Boss thread when Boss calls notify()!!
        // This means that you shall not do stuff here, you shall only raise a flag to the Worker
        // thread that the Worker thread shall take action. This can be done by adding the order
        // to a queue that is monitored by the Worker thread, or you can use condition variables.

        // Always lock with a mutex before changing shared variable and condition_variable
        std::lock_guard<std::mutex> lock(mWorker.mMutex);
        mWorker.mLastOrder = order;
        mWorker.mOrderAvailable.notify_one();
    }
}