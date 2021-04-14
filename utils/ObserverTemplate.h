#ifndef EVENTDEMO_OBSERVERTEMPLATE_H
#define EVENTDEMO_OBSERVERTEMPLATE_H

#include <iostream>
#include <vector>
#include <functional>

namespace Utils{

    template <typename T>
    class Observer{
    public:
        virtual void onEvent(const T &data)=0;
    };

    template <typename T>
    class Observable{
    public:
        void registerObserver(Observer<T>& o){
            observers.emplace_back(o);
        }

        void notify(const T& data ){
            for(Observer<T>& o : observers){
                o.onEvent(data);
            }
        }

    private:
        std::vector<std::reference_wrapper<Observer<T>>> observers;
    };
} //namespace


#endif //EVENTDEMO_OBSERVERTEMPLATE_H
