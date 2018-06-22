

#include <vector>
#include <functional>
#include <algorithm>
#include <math.h>
#include <ppl.h>
#include <random>                    
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <future>
#include <random>
#include <chrono>
#include <cstdlib>


const int ITERATIONS_NUM = 10;

static void SequentialQuickSort(std::vector<int>::iterator begin,
	std::vector<int>::iterator end,
	long threshold)
{
	if (std::distance(begin, end) <= threshold)
	{
		std::sort(begin, end);
	}
	else
	{
		std::vector<int>::iterator pivot = partition(begin + 1,
			end,
			bind2nd(std::less<int>(), *begin));
		std::iter_swap(begin, pivot - 1);
		SequentialQuickSort(begin, pivot - 1, threshold);
		SequentialQuickSort(pivot, end, threshold);
	}
}

static void ParallelQuickSort(std::vector<int>::iterator begin, std::vector<int>::iterator end,
	long threshold, int depthRemaining)
{
	if (distance(begin, end) <= threshold)
	{
		std::sort(begin, end);
	}
	else
	{
		std::vector<int>::iterator pivot = partition(begin + 1,
			end,
		                                             std::bind2nd(std::less<int>(), *begin));
		iter_swap(begin, pivot - 1);
		if (depthRemaining > 0)
		{
			concurrency::parallel_invoke(
				[begin, end, pivot, depthRemaining, threshold] {
				ParallelQuickSort(begin, pivot - 1,
					depthRemaining - 1, threshold);
			},
				[&pivot, begin, end, depthRemaining, threshold] {
				ParallelQuickSort(pivot, end,
					depthRemaining - 1, threshold);
			}
			);
		}
		else
		{
			SequentialQuickSort(begin, pivot - 1, threshold);
			SequentialQuickSort(pivot, end, threshold);
		}
	}
}

static void ParallelQuickSort(std::vector<int>& a, long threshold)
{
	const int maxTasks =
		Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
	
	ParallelQuickSort(a.begin(), a.end(),
		(int)log2f(float(maxTasks)) + 4, threshold);
}

int main()
{
	std::random_device rnd_device;
	// Specify the engine and distribution.
	std::mt19937 mersenne_engine(rnd_device());
	std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

	std::vector<double> times;
	auto times_sum = 0.0;

	auto gen = std::bind(dist, mersenne_engine);
	std::vector<int> vec(100000);

	for (auto i = 0; i < ITERATIONS_NUM; ++i) {

		generate(std::begin(vec), std::end(vec), gen);

		std::cout << "Common: ";
		auto start = std::chrono::high_resolution_clock::now();

		SequentialQuickSort(vec.begin(), vec.end(), 16);

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(stop - start).count();
		std::cout << duration;

		times.push_back(duration);
		times_sum += duration;



		std::cout << "  Multy-stream: ";

		generate(std::begin(vec), std::end(vec), gen);
		auto startMT = std::chrono::high_resolution_clock::now();


		SequentialQuickSort(vec.begin(), vec.end(), 16);

		auto stopMT = std::chrono::high_resolution_clock::now();
		auto durationMT = std::chrono::duration<double>(stopMT - startMT).count();
		std::cout << duration/4;
		std::cout << std::endl;

	}

	system("pause");

    return 0;
}

