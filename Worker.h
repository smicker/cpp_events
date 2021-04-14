#ifndef EVENTDEMO_WORKER_H
#define EVENTDEMO_WORKER_H

#include <mutex>
#include <condition_variable>
#include "Boss.h"

namespace Company {
    class Worker {
    public:
        Worker(Boss &boss);
        void work();

    private:
        class BossListener : public Utils::Observer<std::string> {
            public:
                explicit BossListener(Worker &x) : mWorker(x){;};

                /**
                 * @brief Will be called when Boss gives an order
                 *
                 * @param data The order.
                 */
                void onEvent(const std::string &order) override;

                Worker &mWorker;
            };

        void executeOrder();

        static const int ORDER_PER_DAY = 4;
        Boss &mBoss;
        BossListener mBossListener;

        std::mutex mMutex;
        std::condition_variable mOrderAvailable;

        std::string mLastOrder = "";
    };
}


#endif //EVENTDEMO_WORKER_H
