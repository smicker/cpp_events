/**
 * This worker will put all orders into a queue and will not stop working until all work from the Boss is done.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "WorkerWithQueue.h"

namespace Company {
    WorkerWithQueue::WorkerWithQueue(Boss &boss) : mBoss(boss), mBossListener(*this) {
        mBoss.registerObserver(mBossListener);
        std::cout << "[WorkerWithQueue] is hired!!\n";
    }

    void WorkerWithQueue::work() {
        int isDoneForToday = 0;
        std::cout << "[WorkerWithQueue] Starts workday!!\n";

        while (!isDoneForToday) {
            std::unique_lock<std::mutex> lock(mMutex);
            mOrderAvailable.wait(lock, [&]()
            {
                // Acquire the lock only if Boss stopped working or
                // the queue isn't empty
                return !mBoss.isWorking() || !mWorkQueue.empty();
            });

            // If this Worker thread is taking long time we need to unlock here since if the Boss thread
            // calls onEvent the Boss thread will halt on the lock_guard until the lock is released and
            // we don't want to let the Boss wait.
            lock.unlock();
            
            bool queueDone = false;
            while (!queueDone) {
                lock.lock();
                    // Worker owns the mutex here so we can safely read shared variable "workQueue".
                    std::string work = mWorkQueue.front();
                    mWorkQueue.pop();
                lock.unlock();
                std::cout << "[WorkerWithQueue] Executing order: " << work << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(5));

                lock.lock();
                    queueDone = mWorkQueue.empty();
                lock.unlock();
            }

            isDoneForToday = !mBoss.isWorking();
        }

        std::cout << "[WorkerWithQueue] Ends workday!!\n";
    }

    void WorkerWithQueue::BossListener::onEvent(const std::string &order) {
        // std::cout << "[BossListener] was notified about new order: " << order << "\n";

        // Note that this function is still executed in the Boss thread when Boss calls notify()!!
        // This means that you shall not do stuff here, you shall only raise a flag to the Worker
        // thread that the Worker thread shall take action. This can be done by adding the order
        // to a queue that is monitored by the Worker thread, or you can use condition variables.

        // Always lock with a mutex before changing shared variable and condition_variable
        std::lock_guard<std::mutex> lock(mWorker.mMutex);
        mWorker.mWorkQueue.push(order);
        mWorker.mOrderAvailable.notify_one();
    }
}