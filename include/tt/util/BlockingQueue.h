#include <condition_variable>
#include <mutex>
#include <queue>

#pragma once

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

namespace tt {
    // Source - https://stackoverflow.com/a/52128086
    // Posted by Serge Rogatch
    // Retrieved 2026-09-17, License - CC BY-SA 4.0
    template <typename T> class BlockingQueue {
        std::condition_variable _cvCanPop;
        std::mutex _sync;
        std::queue<T> _qu;
        bool _bShutdown = false;

        public:
            void Push(T item)
            {
                {
                std::unique_lock<std::mutex> lock(_sync);
                _qu.push(std::move(item));
                }
                _cvCanPop.notify_one();
            }

            void RequestShutdown() {
                {
                std::unique_lock<std::mutex> lock(_sync);
                _bShutdown = true;
                }
                _cvCanPop.notify_all();
            }

            bool Pop(T &item) {
                std::unique_lock<std::mutex> lock(_sync);
                for (;;) {
                if (_qu.empty()) {
                    if (_bShutdown) {
                    return false;
                    }
                }
                else {
                    break;
                }
                _cvCanPop.wait(lock);
                }
                item = std::move(_qu.front());
                _qu.pop();
                return true;
            }

            void Clear() {
                while(!_qu.empty()) {
                    T i;
                    Pop(i);
                }
            }
        };
    }


#endif