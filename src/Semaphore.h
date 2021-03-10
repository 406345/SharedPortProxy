#pragma once

#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore
{
public:
	Semaphore(long v) {
		this->value = v;
	};
	~Semaphore() {};

	void wait() {
		std::unique_lock<mutex> m_lck(this->lck);

		--value;

		while (value < 0) {
			signal.wait(m_lck);
		}
	}

	void release() {
		std::unique_lock<mutex> m_lck(this->lck);
		
		++value;

		if (value > 0) {
			signal.notify_one();
		}
	}
private:
	long value = 0;
	mutex lck;
	condition_variable signal;
};
