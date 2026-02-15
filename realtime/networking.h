#ifndef NETWORKING_H_
#define NETWORKING_H_

/**
 * @brief Networking for high performance, latency-sensitive applications in
 * linux environment
 * With the networking components that we will create, are going to be used for
 * the HFT and interacting with the trading networking api.
 * we will use the unix socket api to:
 1. Module of helper functions for constructing and configuring low latency Unix
sockets.
 2. TCP socket wrapper class.
 3. Low latency TCP server for handling numerous TCP connections.
 4. UDP multicast socket wrapper class.

 * Every kind of unix socket (network..) use the same api.
 https://github.com/stacygaudreau/nitek/blob/e0e7d30fbc9d806c998d4c7fec62728ba480aae8/source/llbase/sockets.h

 * most of the next code will be psuedo code
 */

/**
 * @brief by default sockets are blocking, meaning they can wait for a peer for
 * a long time, they return the result like a simple function, the socket calls
 * to their communication methods (send, recv), they execute on the same thread
 * that they are on and return when they are done, but it still involves
 * latencies and multithreading isnt always the most optimized solution, a GOOD
 * solution is to use polling to multiplex all the conncetions on the server,
 * offering us a non-blocking solution (on windows its select, on linux its
 * epoll), epoll is better the poll because poll has extra overhead by doing
 * kernal call each time its called and its gets slower with each socket its
 * polled.
 */

#endif