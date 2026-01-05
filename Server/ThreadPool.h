#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class Server;

enum class WorkerState {
    Idle,
    Busy,
    Stopped
};

struct WorkerStats {
    WorkerState state;
    std::chrono::steady_clock::time_point lastChange;
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

    void enqueue(int clientSocket);
    void stop();

    ThreadPoolSnapshot getSnapshot();

private:
    void workerLoop(size_t index);
    void setWorkerState(size_t index, WorkerState newState);

    std::vector<std::thread> workers;
    std::queue<int> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;

    bool running;
    size_t minThreads;
    size_t maxThreads;

    std::vector<WorkerStats> workerStats;
    std::mutex statsMutex;

    Server* server;
};
