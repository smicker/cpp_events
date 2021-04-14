#ifndef EVENTDEMO_WORKERWITHHELPQUEUE_H
#define EVENTDEMO_WORKERWITHHELPQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Boss.h"
#include "utils/WorkQueue.h"

namespace Company {
    class WorkerWithHelpQueue {
    public:
        WorkerWithHelpQueue(Boss &boss);
        void work();

    private:
        class BossListener : public Utils::Observer<std::string> {
            public:
                explicit BossListener(WorkerWithHelpQueue &x) : mWorker(x){;};

                /**
                 * @brief Will be called when Boss gives an order
                 *
                 * @param data The order.
                 */
                void onEvent(const std::string &order) override;

                WorkerWithHelpQueue &mWorker;
            };

        void executeOrder();

        Boss &mBoss;
        BossListener mBossListener;

        Utils::WorkQueue<std::string> mWorkQueue;
    };
}


#endif //EVENTDEMO_WORKERWITHHELPQUEUE_H
