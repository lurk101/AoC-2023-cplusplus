#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ctpl {

namespace detail {
template <typename T>
class Queue {
   public:
    bool push(T const& value) {
        std::unique_lock<std::mutex> lock(mutex);
        q.push(value);
        return true;
    }
    // deletes the retrieved element, do not use for non integral types
    bool pop(T& v) {
        std::unique_lock<std::mutex> lock(mutex);
        if (q.empty()) return false;
        v = q.front();
        q.pop();
        return true;
    }
    bool empty() {
        std::unique_lock<std::mutex> lock(mutex);
        return q.empty();
    }

   private:
    std::queue<T> q;
    std::mutex mutex;
};
}  // namespace detail

class thread_pool {
   public:
    thread_pool() { init(); }
    thread_pool(int nThreads) {
        init();
        resize(nThreads);
    }

    // the destructor waits for all the functions in the queue to be finished
    ~thread_pool() { stop(true); }

    // get the number of running threads in the pool
    int size() { return static_cast<int>(threads.size()); }

    // number of idle threads
    int n_idle() { return nWaiting; }
    std::thread& get_thread(int i) { return *threads[i]; }

    // change the number of threads in the pool
    // should be called from one thread, otherwise be careful to not interleave,
    // also with this->stop() nThreads must be >= 0
    void resize(int nThreads) {
        if (!isStop && !isDone) {
            int oldNThreads = static_cast<int>(threads.size());
            if (oldNThreads <= nThreads) {  // if the number of threads is increased
                threads.resize(nThreads);
                flags.resize(nThreads);

                for (int i = oldNThreads; i < nThreads; ++i) {
                    flags[i] = std::make_shared<std::atomic<bool>>(false);
                    set_thread(i);
                }
            } else {  // the number of threads is decreased
                for (int i = oldNThreads - 1; i >= nThreads; --i) {
                    *flags[i] = true;  // this thread will finish
                    threads[i]->detach();
                }
                {
                    // stop the detached threads that were waiting
                    std::unique_lock<std::mutex> lock(mutex);
                    cv.notify_all();
                }
                threads.resize(nThreads);  // safe to delete because the
                                           // threads are detached
                flags.resize(nThreads);    // safe to delete because the threads have copies
                                           // of shared_ptr of the flags, not originals
            }
        }
    }

    // empty the queue
    void clear_queue() {
        std::function<void(int id)>* _f;
        while (q.pop(_f)) delete _f;  // empty the queue
    }

    // pops a functional wrapper to the original function
    std::function<void(int)> pop() {
        std::function<void(int id)>* _f = nullptr;
        q.pop(_f);
        std::unique_ptr<std::function<void(int id)>> func(
            _f);  // at return, delete the function even if an exception occurred
        std::function<void(int)> f;
        if (_f) f = *_f;
        return f;
    }

    // wait for all computing threads to finish and stop all threads
    // may be called asynchronously to not pause the calling thread while
    // waiting if isWait == true, all the functions in the queue are run,
    // otherwise the queue is cleared without running the functions
    void stop(bool isWait = false) {
        if (!isWait) {
            if (isStop) return;
            isStop = true;
            for (int i = 0, n = size(); i < n; ++i) {
                *flags[i] = true;  // command the threads to stop
            }
            clear_queue();  // empty the queue
        } else {
            if (isDone || isStop) return;
            isDone = true;  // give the waiting threads a command to finish
        }
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.notify_all();  // stop all waiting threads
        }
        for (int i = 0; i < static_cast<int>(threads.size());
             ++i) {  // wait for the computing threads to finish
            if (threads[i]->joinable()) threads[i]->join();
        }
        // if there were no threads in the pool but some functors in the queue,
        // the functors are not deleted by the threads therefore delete them
        // here
        clear_queue();
        threads.clear();
        flags.clear();
    }

    template <typename F, typename... Rest>
    auto push(F&& f, Rest&&... rest) -> std::future<decltype(f(0, rest...))> {
        auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(int)>>(
            std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...));
        auto _f = new std::function<void(int id)>([pck](int id) { (*pck)(id); });
        q.push(_f);
        std::unique_lock<std::mutex> lock(mutex);
        cv.notify_one();
        return pck->get_future();
    }

    // run the user's function that excepts argument int - id of the running
    // thread. returned value is templatized operator returns std::future, where
    // the user can get the result and rethrow the catched exceptins
    template <typename F>
    auto push(F&& f) -> std::future<decltype(f(0))> {
        auto pck = std::make_shared<std::packaged_task<decltype(f(0))(int)>>(std::forward<F>(f));
        auto _f = new std::function<void(int id)>([pck](int id) { (*pck)(id); });
        q.push(_f);
        std::unique_lock<std::mutex> lock(mutex);
        cv.notify_one();
        return pck->get_future();
    }

   private:
    // deleted
    thread_pool(const thread_pool&);             // = delete;
    thread_pool(thread_pool&&);                  // = delete;
    thread_pool& operator=(const thread_pool&);  // = delete;
    thread_pool& operator=(thread_pool&&);       // = delete;

    void set_thread(int i) {
        std::shared_ptr<std::atomic<bool>> flag(flags[i]);  // a copy of the shared ptr to the flag
        auto f = [this, i, flag /* a copy of the shared ptr to the flag */]() {
            std::atomic<bool>& _flag = *flag;
            std::function<void(int id)>* _f;
            bool isPop = q.pop(_f);
            while (true) {
                while (isPop) {  // if there is anything in the queue
                    std::unique_ptr<std::function<void(int id)>> func(
                        _f);  // at return, delete the function even if an
                              // exception occurred
                    (*_f)(i);
                    if (_flag)
                        return;  // the thread is wanted to stop, return even if
                                 // the queue is not empty yet
                    else
                        isPop = q.pop(_f);
                }
                // the queue is empty here, wait for the next command
                std::unique_lock<std::mutex> lock(mutex);
                ++nWaiting;
                cv.wait(lock, [this, &_f, &isPop, &_flag]() {
                    isPop = q.pop(_f);
                    return isPop || isDone || _flag;
                });
                --nWaiting;
                if (!isPop)
                    return;  // if the queue is empty and isDone == true or
                             // *flag then return
            }
        };
        threads[i].reset(new std::thread(f));  // compiler may not support std::make_unique()
    }

    void init() {
        nWaiting = 0;
        isStop = false;
        isDone = false;
    }

    std::vector<std::unique_ptr<std::thread>> threads;
    std::vector<std::shared_ptr<std::atomic<bool>>> flags;
    detail::Queue<std::function<void(int id)>*> q;
    std::atomic<bool> isDone;
    std::atomic<bool> isStop;
    std::atomic<int> nWaiting;  // how many threads are waiting

    std::mutex mutex;
    std::condition_variable cv;
};

}  // namespace ctpl
