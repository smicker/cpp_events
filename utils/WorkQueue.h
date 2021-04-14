
#ifndef DRONECONTROLLER_WORKQUEUETEMPLATE_H
#define DRONECONTROLLER_WORKQUEUETEMPLATE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>

namespace Utils {

    /**
     * @brief A thread safe queue that has a wait_and_pop function that will block the calling thread until there is something in the queue.
     *
     * @tparam T
     */
    template <typename T>
    class WorkQueue {
        std::condition_variable work_available;
        std::mutex work_mutex;
        std::deque<T> work;

    public:
        /**
         * @brief Returns true if the queue is empty.
         */
        bool isEmpty() {
            std::unique_lock<std::mutex> lock(work_mutex);
            return work.empty();
        }

        /**
         * @brief Pushes the item onto the last position in the queue.
         *
         * @param item
         */
        void push(T item) {
            std::unique_lock<std::mutex> lock(work_mutex);

            bool was_empty = work.empty();
            work.push_back(item);

            lock.unlock();

            if (was_empty) {
                work_available.notify_one();
            }
        }

        /**
         * @brief Pushes the item onto the last position in the queue, if it does not currently already exist in the queue.
         *
         * @param item
         */
        void push_if_not_exist(T item) {
            std::unique_lock<std::mutex> lock(work_mutex);

            if (std::find(work.begin(), work.end(), item) == work.end()) {
                // Note, we cannot just call the push function here since that will stall the thread due to the lock
                // and we want one lock for this whole function, so the code below unfortunately needs to be a copy of
                // the push code.
                bool was_empty = work.empty();
                work.push_back(item);

                lock.unlock();

                if (was_empty) {
                    work_available.notify_one();
                }
            }
        }

        /**
         * @brief Halts the caller thread until there is an object in the queue
         * and then returns the first object and removes it from the queue.
         *
         * @return T
         */
        T wait_and_pop() {
            std::unique_lock<std::mutex> lock(work_mutex);
            while (work.empty()) {
                work_available.wait(lock);
            }

            T tmp = work.front();
            work.pop_front();
            return tmp;
        }

        /**
         * @brief Removes the first element in the queue.
         * 
         */
        void pop() {
            std::unique_lock<std::mutex> lock(work_mutex);
            if (!work.empty()) {
                work.pop_front();
            }
        }

        /**
         * @brief Halts the caller thread until there is an object in the queue
         * and then returns the first object without removing it from the queue.
         * 
         * @return T 
         */
        T wait_and_read() {
            std::unique_lock<std::mutex> lock(work_mutex);
            while (work.empty()) {
                work_available.wait(lock);
            }
            
            return work.front();
        }

        /**
         * @brief Halts the caller thread until there is an object in the queue
         * and then returns the last object and removes all items from the queue.
         *
         * @return T
         */
        T wait_pop_and_reset() {
            std::unique_lock<std::mutex> lock(work_mutex);
            while (work.empty()) {
                work_available.wait(lock);
            }

            T tmp = work.back();
            work.clear();
            return tmp;
        }
    };
} //namespace

#endif //DRONECONTROLLER_WORKQUEUETEMPLATE_H
