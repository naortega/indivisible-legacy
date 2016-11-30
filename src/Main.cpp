#include <iostream>
#include <vector>
#include <csignal>

static bool run;

void leave(int signum);

int main(void) {
	std::cout << "Indivisible v0.1\n";
	run = true;

	signal(SIGINT, leave);

	std::vector<unsigned long long> primes;
	primes.push_back(2);
	unsigned long long num = 2;

	while(run) {
		bool isPrime = true;
		for(auto i : primes) {
			if(i > num / 2) break;
			if(num % i == 0) isPrime = false;
		}
		if(isPrime) {
			primes.push_back(num);
			std::cout << num << std::endl;
		}
		++num;
	}

	return 0;
}

void leave(int signum) {
	std::cout << "Exiting (" << signum << ")\n";
	run = false;
}
