/*
 * main.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: mikejyg
 *
 * this code uses signal to a individual thread to interrupt a system block.
 *
 */

#include <thread>
#include <iostream>
#include <signal.h>
#include <unistd.h>

static void handler(int signum)
{
	std::cout << "handler() thread: " << std::this_thread::get_id() << std::endl;
//	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	std::cout << "main thread: " << std::this_thread::get_id() << std::endl;

	signal(SIGUSR1, handler);

	std::thread t1([&](){
		while (true) {
			std::cout << "hello from: " << std::this_thread::get_id() << std::endl;
//			auto k = sleep(1);
			struct timespec sleepTime{1};
			auto k = nanosleep(&sleepTime, nullptr);
			std::cout << "nanosleep() returns: " << k << std::endl;
			if (k!=0) {
				std::cout << "exiting..." << std::endl;
				break;
			}

		}
	});

	sleep(4);

	std::cout << "tell t1 to quit..." << std::endl;
	pthread_kill(t1.native_handle(), SIGUSR1);
	t1.join();

	std::cout << "t1 joined." << std::endl;
	sleep(1);

	std::cout << "all done." << std::endl;
	return 0;

}
