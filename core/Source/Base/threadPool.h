#pragma once

#include <deque>
#include <mutex>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <thread>

namespace Simply2D
{
    using Job = std::function<void()>;

    class ThreadPool
    {
    public:
        // ---- Static Public API ----
        static void Initialize();
        static void Shutdown();
        static void Execute(const Job& job);
        static void Wait();
        static bool Busy();

    private:
        // ---- Singleton Access ----
        static ThreadPool& Instance();

        ThreadPool() = default;
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

        // ---- Internal Implementations ----
        void InitializeImpl();
        void ShutdownImpl();
        void ExecuteImpl(const Job& job);
        void WaitImpl();
        bool BusyImpl();

        void WorkerLoop();

        struct JobQueue
        {
            bool push_back(const Job& item);
            bool pop_front(Job& out);
            void clear();
            bool empty();

            std::deque<Job> m_queue;
            std::mutex      m_lock;
        };

        // ---- Internal State ----
        std::atomic<bool> m_alive{ false };
        uint32_t          m_numThreads = 0;
        std::vector<std::thread> m_threads;

        JobQueue          m_jobQueue;

        std::atomic<uint64_t> m_currentLabel{ 0 };
        std::atomic<uint64_t> m_finishLabel{ 0 };

        std::condition_variable m_wakeCondition;
        std::mutex              m_wakeMutex;
    };
}

