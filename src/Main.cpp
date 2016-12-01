#include <iostream>
#include <omp.h>
#include <vector>
#include <csignal>
#include <cassert>

static bool run;

int main(void) {
	std::cout << "Indivisible v0.1\n";
	run = true;

	signal(SIGINT, [](int signum) {
			std::cout << "Exiting (" << signum << ")\n";
			run = false;
			});

	std::vector<unsigned long long> primes;
	primes.push_back(2);
	unsigned long long num = 3;

	// Use for to accomodate for OpenMP
	#pragma omp parallel
	{
		do {
			unsigned long long myNum;
			#pragma omp critical
			{
				myNum = num;
				num += 2;
			}
			bool isPrime = true;
			#pragma omp barrier
			for(auto i : primes) {
				if(i > myNum / 2) break;
				if(myNum % i == 0) {
					isPrime = false;
					break;
				}
			}
			if(isPrime) {
				#pragma omp critical
				{
					primes.push_back(num);
					std::cout << num << std::endl;
				}
			}
		} while(run);
	}

	return 0;
}
