#pragma once

#include<thread>

//Custom thread that will join the thread on destruction if joinable
class CustomThread
{
public:
	CustomThread(std::thread t) :m_thread(std::move(t)) {}
	~CustomThread()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

private:
	std::thread m_thread;
};