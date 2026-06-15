#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadsManager
{
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;

	std::condition_variable conditional_variable;
	std::mutex queue_mutex;

	std::atomic<bool> stop_flag{ false };
	std::atomic<size_t> active_tasks{ 0 };

	std::condition_variable wait_all_cv;

	void worker_thread()
	{
		while (true)
		{
			std::function<void()> task;

			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				
				conditional_variable.wait(lock, [this]() {
					return stop_flag || !tasks.empty();
					});	

				if (stop_flag || tasks.empty())
					return;

				task = tasks.front();
				tasks.pop();
			}

			task();

			if (--active_tasks == 0)
			{
				wait_all_cv.notify_all();
			}
		}
	}
public:
	ThreadsManager(size_t thread_count = 0)
	{
		if (thread_count == 0)
		{
			thread_count = std::thread::hardware_concurrency();

			if (thread_count == 0)
				thread_count = 2;
		}
		this->workers.reserve(thread_count);
		
		for (int i = 0; i < thread_count; ++i)
		{
			workers.emplace_back(&ThreadsManager::worker_thread, this);
		}
	}

	~ThreadsManager()
	{
		{
			std::lock_guard<std::mutex> lock(queue_mutex);

			stop_flag = true;
		}
		conditional_variable.notify_all();

		for (auto& thread : this->workers)
		{
			thread.join();
		}
	}

	void push_task(std::function<void()> task)
	{
		this->active_tasks++;

		{
			std::lock_guard<std::mutex> lock(queue_mutex);

			this->tasks.push(task);
			//this->tasks.push(std::move(task));
		}

		this->conditional_variable.notify_one();
	}

	void wait_all()
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		
		wait_all_cv.wait(lock, [this]() {
			return active_tasks == 0;
			});
	}

	size_t thread_count() const
	{
		return this->workers.size();
	}
};