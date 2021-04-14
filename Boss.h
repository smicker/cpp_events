#ifndef EVENTDEMO_BOSS_H
#define EVENTDEMO_BOSS_H

#include <mutex>
#include "utils/ObserverTemplate.h"


namespace Company {
    class Boss : public Utils::Observable<std::string> {
    public:
        Boss();
        void work();
        bool isWorking() {
            std::unique_lock<std::mutex> lock(mMutex);
            return mIsWorking;
        }
    private:
        void setIsWorking(bool isWorking) {
            std::unique_lock<std::mutex> lock(mMutex);
            mIsWorking = isWorking;
        }

        static const int WORKDAY = 4;
        bool mIsWorking = true;
        std::mutex mMutex;
    };
}


#endif //EVENTDEMO_BOSS_H
