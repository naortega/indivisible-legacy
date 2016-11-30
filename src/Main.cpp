#include <iostream>
#include <omp.h>
#include <vector>
#include <csignal>
#include <cassert>

static bool run;

void leave(int signum);

int main(void) {
	std::cout << "Indivisible v0.1\n";
	run = true;

	signal(SIGINT, leave);

	std::vector<unsigned long long> primes;
	primes.push_back(2);
	unsigned long long num = 2;

	// Use for to accomodate for OpenMP
	#pragma omp parallel
	{
		do {
			unsigned long long myNum;
			#pragma omp critical
			{
				myNum = num;
				++num;
			}
			bool isPrime = true;
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

void leave(int signum) {
	std::cout << "Exiting (" << signum << ")\n";
	run = false;
}
