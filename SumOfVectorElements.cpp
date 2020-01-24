#include <iostream>

#include <chrono>
#include <vector>
#include <thread>
#include <utility>
#include <mutex>
#include <atomic>

namespace Multithreading
{
	constexpr long long size = 1000000;

	//Every thread executes calculateSum with different portion of the data (vector<int> elements)
	void calculateSum(std::atomic<long long>& sum, const std::vector<int>& elements, long long begin, long long end)
	{
		long long threadSum = 0;
		for (long long it = begin; it < end; ++it)
		{
			threadSum += elements[it];
		}
		sum.fetch_add(threadSum);
	}
}

int main()
{
	std::vector<int> elements;
	elements.reserve(Multithreading::size);

	for (int i = 0; i < Multithreading::size; i++)
	{
		elements.push_back(i + 1);
	}

	const auto beginTime = std::chrono::steady_clock::now();
	std::atomic< long long> sum = 0;

	int threadNumber = std::thread::hardware_concurrency();
	double step = (double)Multithreading::size / threadNumber;
	std::vector<std::thread> allThreads;
	allThreads.reserve(threadNumber);

	for (auto i = 0; i < threadNumber; i++)
	{
		std::thread newThread(Multithreading::calculateSum, std::ref(sum), std::ref(elements), i * step, (i + 1) * step);
		allThreads.push_back(std::move(newThread));
	}

	for (std::thread& currentThread : allThreads)
	{
		currentThread.join();
	}

	const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - beginTime;

	std::cout << "Time:" << duration.count() << " seconds" << std::endl;
	std::cout << "Result: " << sum << std::endl;

	return 0;
}
