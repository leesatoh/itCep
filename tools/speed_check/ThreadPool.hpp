#pragma once
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <cassert>
#include <functional>
#include "InterFaces.h"
template <typename T> class List
{
public:
	List()
	{}
	bool put(T&& data) {
		list_.push_back(std::move(data));
		return true;
	}
	bool put(const T& data) {
		list_.push_back(data);
		return true;
	}
	bool get(T& data) {
		if (list_.empty()) {
			return false;
		}
		data = std::move(list_.front());
		list_.pop_front();
		return true;
	}
	bool empty() const {
		return list_.empty();
	}
private:
	std::list<T> list_;
};



class cThreadPool
{
public:
	cThreadPool(int threadCount)
		: isTerminationRequested_(false)
		, list_()
	{
		for (int n = 0; n < threadCount; n++) {
			threads_.emplace_back(std::thread(main_));
		}
	}
	~cThreadPool() {
		{
			std::unique_lock<std::mutex> ul(mutex_);
			isTerminationRequested_ = true;
		}
		cv_.notify_all();
		const int size = threads_.size();
		for (int n = 0; n < size; n++) {
			threads_.at(n).join();
		}
	}
	bool add(std::shared_ptr<IRunnable> &&runnable) {
		{
			std::unique_lock<std::mutex> ul(mutex_);
			if (!list_.put(std::move(runnable))) { return false; }
		}
		cv_.notify_all();
		return true;
	}
	bool add(const std::shared_ptr<IRunnable> &runnable) {
		{
			std::unique_lock<std::mutex> ul(mutex_);
			if (!list_.put(runnable)) { return false; }
		}
		cv_.notify_all();
		return true;
	}
private:
	std::function<void()> main_ = [this]()
	{
		while (1) {
			std::shared_ptr<IRunnable> runnable;
			{
				std::unique_lock<std::mutex> ul(mutex_);
				while (list_.empty()) {
					if (isTerminationRequested_) { return; }
					cv_.wait(ul);
				}
				const bool result = list_.get(runnable);
				assert(result);
			}
			runnable->run();
		}
	};
	bool isTerminationRequested_;
	List<std::shared_ptr<IRunnable>> list_;
	std::mutex mutex_;
	std::condition_variable cv_;
	std::vector<std::thread> threads_;
};
