#include <iostream>
#include <thread>
#include <chrono>
#include "Boss.h"
#include "utils/ObserverTemplate.h"

namespace Company {
    Boss::Boss() {
        std::cout << "[Boss] is hired!!\n";
    }

    void Boss::work() {
        int workhour = 0;
        setIsWorking(true);
        std::cout << "[Boss] Starts workday!!\n";
        static const std::string ORDER = "GET COFFEE";

        while (workhour < WORKDAY) {
            std::cout << "[Boss] Gives order: " << ORDER << "!!\n";
            notify(ORDER);
            std::this_thread::sleep_for(std::chrono::seconds(1));            
            workhour++;
        }

        notify("Going home!!");

        std::cout << "[Boss] Ends workday!!\n";
        setIsWorking(false);
    }
}