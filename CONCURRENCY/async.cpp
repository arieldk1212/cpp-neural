#include <iostream>
#include <chrono>
#include <thread>
#include <future> // holds the std::async

/* FUTURE AND ASYNC */

// to sum up, its a different thread that runs async..
// std::async -> without sync..
// future and async go together.

bool bufferedfileloader() {
  size_t bytes_loaded = 0;
  while (bytes_loaded < 2000) {
    std::cout << "thread: loading file.." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    bytes_loaded += 1000;
  }
  return true;
}

int get_number(int x) { return x; }

int main() {
  // <bool> is what the future expects to get (what the desired function returns)
  std::future<bool> backgroundthread = std::async(std::launch::async, &bufferedfileloader);
  std::future_status status;

  std::future<int> get_number_thread = std::async(std::launch::async, &get_number, 5);
  // we are blocked at the get until our result has computed.
  int result = get_number_thread.get();

  while (true) {
    std::cout << "the number is: " << result << std::endl;
    std::cout << "main thread is running.." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // like real time applications
    status = backgroundthread.wait_for(std::chrono::milliseconds(1)); // waits for the result from the function for the time given, if not it fails.
    if (status == std::future_status::ready) {
      std::cout << "data is ready.." << std::endl;
      break;
    }
  }
  std::cout << "program is complete" << std::endl;  

  return 0;
}
