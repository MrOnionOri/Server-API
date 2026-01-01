#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadPool.h"
#include "ServerCore.h"

ThreadPool::ThreadPool(Server* server, size_t minThreads, size_t maxThreads)
    : running(true),
    minThreads(minThreads),
    maxThreads(maxThreads),
    server(server)
{
    workerStats.resize(minThreads);

    for (size_t i = 0; i < minThreads; ++i) {
        workerStats[i].state = WorkerState::Idle;
        workerStats[i].lastChange = std::chrono::steady_clock::now();

        workers.emplace_back(&ThreadPool::workerLoop, this, i);
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::enqueue(int clientSocket) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(clientSocket);
    }
    condition.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        running = false;
    }
    condition.notify_all();

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerLoop(size_t index) {
    while (true) {
        int clientSocket;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] {
                return !tasks.empty() || !running;
                });

            if (!running && tasks.empty()) {
                return;
            }

            clientSocket = tasks.front();
            tasks.pop();
        }
        setWorkerState(index, WorkerState::Busy);
        server->clientHandler(clientSocket);
        setWorkerState(index, WorkerState::Idle);
    }
}

void ThreadPool::setWorkerState(size_t index, WorkerState newState) {
    std::lock_guard<std::mutex> lock(statsMutex);

    auto now = std::chrono::steady_clock::now();
    auto& stats = workerStats[index];

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - stats.lastChange
    );

    if (stats.state == WorkerState::Idle)
        stats.totalIdle += delta;
    else if (stats.state == WorkerState::Busy)
        stats.totalBusy += delta;

    stats.state = newState;
    stats.lastChange = now;
}

ThreadPoolSnapshot ThreadPool::getSnapshot() {
    std::lock_guard<std::mutex> lock(statsMutex);

    ThreadPoolSnapshot snap;
    snap.totalThreads = workers.size();
    snap.queuedTasks = tasks.size();

    for (auto& w : workerStats) {
        if (w.state == WorkerState::Busy) snap.busyThreads++;
        if (w.state == WorkerState::Idle) snap.idleThreads++;
    }

    return snap;
}
