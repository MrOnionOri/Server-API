#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Socket.h"

class Server;

enum class WorkerState {
    Idle,
    Busy,
    Stopped
};

struct WorkerStats {
    WorkerState state = WorkerState::Idle;
    std::chrono::steady_clock::time_point lastChange{ std::chrono::steady_clock::now() };
    std::chrono::milliseconds totalBusy{ 0 };
    std::chrono::milliseconds totalIdle{ 0 };
};

struct ThreadPoolSnapshot {
    size_t totalThreads = 0;
    size_t busyThreads = 0;
    size_t idleThreads = 0;
    size_t queuedTasks = 0;
};

class ThreadPool {
public:
    ThreadPool(Server* server, size_t minThreads, size_t maxThreads);

    ThreadPool();
	void initialize(Server* server, size_t minThreads, size_t maxThreads);

    ~ThreadPool();

    //void enqueue(int clientSocket);
	void enqueue(SOCKET clientSocket);
    void stop();

    ThreadPoolSnapshot getSnapshot() const;

private:
    void workerLoop(size_t index);
    void setWorkerState(size_t index, WorkerState newState);

    std::vector<std::thread> workers;
    std::queue<int> tasks;

    mutable std::mutex queueMutex;
    std::condition_variable condition;

    //bool running;
    std::atomic<bool> running{ false };
    size_t minThreads;
    size_t maxThreads;

    std::vector<WorkerStats> workerStats;
    mutable std::mutex statsMutex;

    Server* server;
};
