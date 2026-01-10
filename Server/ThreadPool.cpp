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

// Default constructor for cases where initialization is deferred
ThreadPool::ThreadPool()
    : running(false),
    minThreads(0),
    maxThreads(0),
    server(nullptr)
{
}

void ThreadPool::initialize(Server* server, size_t minThreads, size_t maxThreads) {
    this->server = server;
    this->minThreads = minThreads;
    this->maxThreads = maxThreads;
    this->running = true;
    workerStats.resize(minThreads);
    for (size_t i = 0; i < minThreads; ++i) {
        workerStats[i].state = WorkerState::Idle;
        workerStats[i].lastChange = std::chrono::steady_clock::now();
        workers.emplace_back(&ThreadPool::workerLoop, this, i);
    }
}

//End of aditional configuration in case is only declared and not initialized

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::enqueue(SOCKET clientSocket) {
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

ThreadPoolSnapshot ThreadPool::getSnapshot() const {
	ThreadPoolSnapshot snapshot;

    snapshot.totalThreads = workers.size();

    {
		std::lock_guard<std::mutex> lock(statsMutex);
        for (const auto& ws: workerStats) {
            if(ws.state == WorkerState::Busy) {
                snapshot.busyThreads++;
            } else if (ws.state == WorkerState::Idle) {
                snapshot.idleThreads++;
			}
        }

        {
			std::lock_guard<std::mutex> lock(queueMutex);
			snapshot.queuedTasks = tasks.size();
        }
    }

    return snapshot;
}
