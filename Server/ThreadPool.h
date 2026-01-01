#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
	ThreadPool(size_t minThreads, size_t maxThreads);
	~ThreadPool();

	void enqueue(int clientSocket);
	void stop();

private:
	void workerLoop();
	std::vector<std::thread> workers;
	std::queue<int> tasks;

	std::mutex queueMutex;
	std::condition_variable  condition;

	bool running;
	size_t minThreads;
	size_t maxThreads;
};