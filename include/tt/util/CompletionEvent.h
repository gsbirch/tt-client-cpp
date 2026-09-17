#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace tt {
    class CompletionEvent {
        public:
            void signal()
            {
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    completed_ = true;
                }

                cv_.notify_all();
            }

            void wait()
            {
                std::unique_lock<std::mutex> lock(mutex_);

                cv_.wait(lock, [this] {
                    return completed_;
                });
            }

        private:
            bool completed_ = false;
            std::mutex mutex_;
            std::condition_variable cv_;
    };
}

