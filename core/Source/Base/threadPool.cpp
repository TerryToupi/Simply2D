#include "ThreadPool.h"
#include <algorithm>

namespace Simply2D
{
    ThreadPool& ThreadPool::Instance()
    {
        static ThreadPool instance;
        return instance;
    }

    void ThreadPool::Initialize() { Instance().InitializeImpl(); }
    void ThreadPool::Shutdown() { Instance().ShutdownImpl(); }
    void ThreadPool::Execute(const Job& job) { Instance().ExecuteImpl(job); }
    void ThreadPool::Wait() { Instance().WaitImpl(); }
    bool ThreadPool::Busy() { return Instance().BusyImpl(); }

    bool ThreadPool::JobQueue::push_back(const Job& item)
    {
        std::scoped_lock lock(m_lock);
        m_queue.push_back(item);
        return true;
    }

    bool ThreadPool::JobQueue::pop_front(Job& out)
    {
        std::scoped_lock lock(m_lock);
        if (m_queue.empty())
            return false;

        out = std::move(m_queue.front());
        m_queue.pop_front();
        return true;
    }

    void ThreadPool::JobQueue::clear()
    {
        std::scoped_lock lock(m_lock);
        m_queue.clear();
    }

    bool ThreadPool::JobQueue::empty()
    {
        std::scoped_lock lock(m_lock);
        return m_queue.empty();
    }

    void ThreadPool::WorkerLoop()
    {
        Job job;

        while (true)
        {
            if (m_jobQueue.pop_front(job))
            {
                job();
                m_finishLabel.fetch_add(1);
                continue;
            }

            std::unique_lock<std::mutex> lock(m_wakeMutex);
            m_wakeCondition.wait(lock, [this]() {
                return !m_alive.load() || !m_jobQueue.empty();
                });

            if (!m_alive.load() && m_jobQueue.empty())
                break;
        }
    }

    void ThreadPool::InitializeImpl()
    {
        ShutdownImpl();

        m_finishLabel.store(0);
        m_currentLabel.store(0);

        m_numThreads = std::thread::hardware_concurrency();
        m_numThreads = std::max(1u, m_numThreads);

        m_threads.reserve(m_numThreads);
        m_alive.store(true);

        for (uint32_t i = 0; i < m_numThreads; ++i)
            m_threads.emplace_back([this] { WorkerLoop(); });
    }

    void ThreadPool::ShutdownImpl()
    {
        if (!m_alive.load())
            return;

        m_alive.store(false);
        m_wakeCondition.notify_all();

        for (auto& t : m_threads)
            if (t.joinable())
                t.join();

        m_threads.clear();
        m_jobQueue.clear();

        m_currentLabel.store(0);
        m_finishLabel.store(0);
        m_numThreads = 0;
    }

    void ThreadPool::ExecuteImpl(const Job& job)
    {
        m_currentLabel.fetch_add(1);
        m_jobQueue.push_back(job);
        m_wakeCondition.notify_one();
    }

    bool ThreadPool::BusyImpl()
    {
        return m_finishLabel.load() < m_currentLabel.load();
    }

    void ThreadPool::WaitImpl()
    {
        while (BusyImpl())
            std::this_thread::yield();
    }
}
