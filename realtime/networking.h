#ifndef NETWORKING_H_
#define NETWORKING_H_

#include <fcntl.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string>
#include <sys/socket.h>

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

// Function to set non-blocking
inline auto set_non_blocking(int fd) -> bool {
  const auto flags = fcntl(fd, F_GETFL, 0);
  if (flags & O_NONBLOCK) {
    return true;
  }
  return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1);
}

/**
 * @brief Reducing TCP packet delay
 * by default network sockets come with Nagles Algorithm, it aims to reduce the
 * number of small tcp packets emitted from a socket at the expense of adding a
 * small delay, we simply delete that delay because they are by default as low
 * as possible regardless the packet size, therefore its best disabled.
 */

// Function to disable the delay
inline auto set_no_delay(int fd) -> bool {
  int one{1};
  return (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
                     reinterpret_cast<void *>(&one), sizeof(one)) != -1);
}

/**
 * @brief For debugging we are going to add timestamps to packets, good for
 * measurement.
 */

// Function to add timestamps to packets
inline auto set_software_timestamps(int fd) -> bool {
  int one{1};
  return (setsockopt(fd, SOL_SOCKET, SO_TIMESTAMP,
                     reinterpret_cast<void *>(&one), sizeof(one)) != -1);
}

/**
 * @brief The SFN8000 series is an example of an accelerated network card
 * designed especially for low latency systems. Hardware on the card supports
the
 * ef_vi API which allows ethernet frames to be sent to and from the card
directly
 * from user space, completely bypassing the kernel in the process. This
improves
 * latency and determinism of network applications.

 * Solarflare also develops a network API called Onload which uses ef_vi to
 * provide a TCP and UDP socket interface separate from native system calls.
 */

// Function to connect to a socket
int connect(const std::string &ip, int port, const std::string &iface,
            bool is_listening) {
  // follow the github link
  in_inaddr.sin_addr.s_addr = INADDR_ANY;
  in_inaddr.sin_port = htons(port);
  in_inaddr.sin_family = AF_INET;
}

/**
 * @brief to send data over tcp its a two step
 */

// Step One
void load_tx(const void *data, size_t len) noexcept {
  // simply copy the given data into the buffer
  memcpy(tx_buffer.data() + i_tx_next, data, len);
  i_tx_next += len;
  ASSERT(i_tx_next < TCP_BUFFER_SIZE,
         "<TCPSocket> tx buffer overflow! Have you called tx_and_rx()?");
}

/**
 * @brief Next, the tx_and_rx() method is called, which encapsulates the socket
functions needed to both send and receive data over the network. First, some
data structures are configured to pass to the recvmsg() call. You can read more
about this system call at the manpages but suffice to say in this case it
returns the number of bytes waiting to be read in the TCP buffer configured by
the iov{ } structure.
A non-blocking read is performed on any pending data and a software kernel
timestamp is transformed into a normalised form suitable for logging and
measurement. The socket receive callback is then executed.

 * @brief Performance Improvement: The socket receive callback rx_callback() is
a std::function instance. These work a lot like virtual functions, and present
some overhead when being called. The compiler is smart enough to preprocess
trivial single instance examples of them, but performance gains can potentially
be had by using a the Curiously recurring template pattern instead. Since this
callback is called within a path critical to the TCP socket (its tx_and_rx()
method) this could be an avenue worth exploring later.
 */

// Function for tx rx

bool tx_and_rx() noexcept {
  char ctrl[CMSG_SPACE(sizeof(struct timeval))];
  auto cmsg = reinterpret_cast<struct cmsghdr *>(&ctrl);

  iovec iov{rx_buffer.data() + i_rx_next, TCP_BUFFER_SIZE - i_rx_next};
  msghdr msg{&in_inaddr, sizeof(in_addr), &iov, 1, ctrl, sizeof(ctrl), 0};

  // non-blocking read of data
  const auto rx_size = recvmsg(fd, &msg, MSG_DONTWAIT);
  if (rx_size > 0) {
    i_rx_next += rx_size;
    Nanos t_kernel{};
    timeval kernel_timeval{};
    if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_TIMESTAMP &&
        cmsg->cmsg_len == CMSG_LEN(sizeof(kernel_timeval))) {
      memcpy(&kernel_timeval, CMSG_DATA(cmsg), sizeof(kernel_timeval));
      // timestamp converted to nanoseconds
      t_kernel = kernel_timeval.tv_sec * NANOS_TO_SECS +
                 kernel_timeval.tv_usec * NANOS_TO_MICROS;
    }
    const auto t_user = get_time_nanos();
    logger.logf("% <TCPSocket::%> RX at socket %, len: %,"
                " t_user: %, t_kernel: %, delta: %\n",
                LL::get_time_str(&t_str), __FUNCTION__, fd, i_rx_next, t_user,
                t_kernel, (t_user - t_kernel));
    // finally, the receive data callback is executed
    rx_callback(this, t_kernel);
  }

  // non-blocking write out of data in tx buffer
  if (i_tx_next > 0) {
    const auto n =
        send(fd, tx_buffer.data(), i_tx_next, MSG_DONTWAIT | MSG_NOSIGNAL);
    logger.logf("% <TCPSocket::%> TX at socket %, size: %\n",
                LL::get_time_str(&t_str), __FUNCTION__, fd, n);
  }
  i_tx_next = 0;
  return (rx_size > 0);
}

/**
 * @brief tcp server anatomy
 1. The server is instantiated with its listen() method, which binds it to a
 local interface and port.
 2. A single TCPSocket is listened on for new incoming connection requests
 (member listening_socket)
 3. New clients have a dedicated TCPSocket created and maintained for them in
 the rx_sockets vector, which tracks all sockets being received upon by the
 server.
 4. Clients which can be written back to are added to the tx_sockets vector, and
 disconnected sockets are similarly tracked in the dx_sockets vector.
 5. Similar to the TCPSocket module, a tx_and_rx() method is called periodically
 by the application which in turn calls the non-blocking read and write methods
 of each of the TCPSocket instances the server is managing.
 */

// Psuedo code for the loop
void poll() noexcept {
  // ...
  if (e.events & EPOLLIN) {
    // epoll input event triggered, we have a
    // -> receive data ...
  }
  if (e.events & EPOLLOUT) {
    // epoll output event triggered
    // -> transmit data ...
  }

  if (e.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
    // EPOLLERR or EPOLLHUP -> socket was disconnected
    //  (error or signal hang up)
    //  -> add to dx_sockets ...
  }
  // ...
}
#endif