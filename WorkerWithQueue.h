#ifndef EVENTDEMO_WORKERWITHQUEUE_H
#define EVENTDEMO_WORKERWITHQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Boss.h"

namespace Company {
    class WorkerWithQueue {
    public:
        WorkerWithQueue(Boss &boss);
        void work();

    private:
        class BossListener : public Utils::Observer<std::string> {
            public:
                explicit BossListener(WorkerWithQueue &x) : mWorker(x){;};

                /**
                 * @brief Will be called when Boss gives an order
                 *
                 * @param data The order.
                 */
                void onEvent(const std::string &order) override;

                WorkerWithQueue &mWorker;
            };

        void executeOrder();

        Boss &mBoss;
        BossListener mBossListener;

        std::mutex mMutex;
        std::condition_variable mOrderAvailable;

        std::queue<std::string> mWorkQueue;
    };
}


#endif //EVENTDEMO_WORKERWITHQUEUE_H
