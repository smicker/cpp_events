#include <iostream>
#include <thread>
#include "Boss.h"
#include "Worker.h"
#include "WorkerWithQueue.h"
#include "WorkerWithHelpQueue.h"

int main() {
    std::cout << "------------\n";
    std::cout << "Workday starts!" << '\n';
    std::cout << "------------\n";

    Company::Boss boss;
    Company::Worker worker(boss);
    Company::WorkerWithQueue worker2(boss);
    Company::WorkerWithHelpQueue worker3(boss);

    auto workerThread = std::thread([&worker](){
        worker.work();
    });

    auto worker2Thread = std::thread([&worker2](){
        worker2.work();
    });

    auto worker3Thread = std::thread([&worker3](){
        worker3.work();
    });

    auto bossThread = std::thread([&boss](){
        boss.work();
    });

    bossThread.join();
    workerThread.join();
    worker2Thread.join();
    worker3Thread.join();
    return 0;
}