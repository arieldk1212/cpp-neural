OSI & FUNDAMENTALS

Q: Explain the OSI model. Which layers matter most for a cloud router?
A: The OSI model has 7 layers: Physical, Data Link, Network, Transport, Session, Presentation, Application. A cloud router mainly operates at Layer 3 (IP routing) and Layer 4 (transport behavior like TCP/UDP). Layer 3 decides packet forwarding, while Layer 4 affects flow reliability, congestion, and performance tuning. Understanding interactions between L3 routing and L4 transport behavior is critical for optimization.

---

Q: Difference between TCP and UDP?
A: TCP is connection-oriented, reliable, ordered, and uses congestion and flow control. UDP is connectionless, best-effort, and low overhead. TCP is better for reliability-sensitive applications; UDP is preferred when latency matters or reliability is implemented at the application layer. Routers care because TCP reacts to packet loss and delay, while UDP traffic does not.

---

Q: What happens during a TCP 3-way handshake?
A: Client sends SYN → server responds SYN-ACK → client replies ACK. This synchronizes sequence numbers and establishes state on both sides. It prevents half-open connections and confirms reachability.

────────────────────
TCP INTERNALS
────────────────────

Q: What’s the difference between flow control and congestion control?
A: Flow control prevents overwhelming the receiver using a sliding window. Congestion control prevents overloading the network using algorithms like slow start and congestion avoidance. One protects endpoints; the other protects the network.

---

Q: Why does TCP reduce its sending rate after packet loss?
A: Packet loss is treated as a congestion signal. TCP assumes network buffers are overloaded and reduces its congestion window to stabilize throughput and avoid collapse.

---

Q: What causes out-of-order packets?
A: Multi-path routing, queueing delays, retransmissions, or load balancing. TCP handles reordering via sequence numbers, but excessive disorder increases latency and CPU overhead.

────────────────────
SOCKETS & LINUX NETWORKING
────────────────────

Q: Walk through creating a TCP server socket.
A: socket() → bind() → listen() → accept(). socket creates the endpoint, bind assigns an address, listen marks it passive, accept establishes a client connection.

---

Q: Blocking vs non-blocking sockets?
A: Blocking sockets wait for operations to complete. Non-blocking sockets return immediately, enabling event-driven architectures. High-performance routers favor non-blocking I/O to scale concurrency.

---

Q: select vs poll vs epoll?
A: select has fd limits and copies sets each call. poll scales better but still scans descriptors. epoll is event-driven and O(1) for large connections — preferred for high-performance servers.

---

Q: How would you debug a stuck TCP connection?
A: Capture packets (tcpdump), inspect socket states (ss/netstat), check retransmissions, RTT spikes, window sizes, and kernel logs. Determine whether the bottleneck is network, kernel buffers, or application logic.

────────────────────
ROUTING & NETWORK BEHAVIOR
────────────────────

Q: What is routing?
A: Routing is selecting a path for packet forwarding based on destination IP and routing tables. Routers use longest-prefix matching to determine next hops.

---

Q: Static vs dynamic routing?
A: Static routing is manually configured and predictable. Dynamic routing adapts to topology changes automatically. Cloud systems rely on dynamic behavior for resilience.

---

Q: What is an overlay network?
A: A virtual network built on top of another network. It abstracts routing decisions, enabling traffic optimization, redundancy, and performance control without changing underlying infrastructure.

────────────────────
PERFORMANCE & SYSTEM DESIGN
────────────────────

Q: Why do packets get dropped?
A: Queue overflow, congestion, buffer limits, NIC saturation, or firewall filtering. Drops signal congestion and affect TCP throughput.

---

Q: What is latency vs throughput?
A: Latency is time per packet; throughput is volume per time. Optimizing routers requires balancing both.

---

Q: What is zero-copy networking?
A: Avoiding memory duplication between kernel and user space. Improves performance and reduces CPU overhead.

---

Q: How do you design a high-performance packet pipeline?
A: Use batching, lock-free queues, non-blocking I/O, CPU affinity, and minimize cache misses. Avoid unnecessary copies and context switches.

────────────────────
SCENARIO QUESTIONS
────────────────────

Q: Traffic suddenly slows down. What do you check?
A: Packet loss, retransmissions, congestion window shrinkage, CPU saturation, queue depth, NIC stats, routing anomalies.

---

Q: UDP packets are lost intermittently. Why?
A: Buffer overflow, network congestion, NIC limitations, or timing bursts. UDP has no retransmission, so application-level handling is required.

---

Q: A router CPU spikes under load. Causes?
A: Excess interrupts, inefficient packet handling, lock contention, excessive copying, or poor batching.

────────────────────
BONUS / ADVANCED
────────────────────

Q: What is head-of-line blocking?
A: One delayed packet blocks subsequent packets in ordered transport like TCP, increasing latency.

---

Q: Kernel vs user-space networking tradeoffs?
A: Kernel networking is safer and integrated; user-space frameworks reduce overhead and increase throughput but add complexity.

---

Q: Why do cloud routers optimize path selection?
A: To reduce latency, avoid congestion, and improve reliability by dynamically choosing better routes.

────────────────────

END OF SHEET

C++ NETWORKING PITFALLS — INTERVIEW PREP SHEET

────────────────────

1. Assuming send()/recv() transfer everything
────────────────────

Pitfall:
Believing send() or recv() processes the full buffer in one call.

Why:
Sockets operate on kernel buffers and may partially transfer data.

Consequence:
Silent truncation, corrupted protocols, stuck state machines.

Correct approach:
Always loop until all bytes are sent/received. Track offsets manually.

---

1. Ignoring EINTR / EAGAIN
────────────────────

Pitfall:
Treating interrupted system calls as fatal errors.

Why:
Signals or non-blocking behavior interrupt syscalls.

Consequence:
Random failures under load or signal activity.

Correct approach:
Retry on EINTR. For EAGAIN, integrate with your event loop.

---

1. Blocking sockets in high-concurrency systems
────────────────────

Pitfall:
Using blocking I/O in a server handling many connections.

Why:
Threads stall waiting on kernel operations.

Consequence:
Thread exhaustion, latency spikes, scalability collapse.

Correct approach:
Use non-blocking sockets + epoll/kqueue or async frameworks.

---

1. Forgetting TCP is a byte stream
────────────────────

Pitfall:
Assuming message boundaries are preserved.

Why:
TCP delivers a continuous stream, not packets.

Consequence:
Protocol desynchronization.

Correct approach:
Implement framing (length prefix, delimiter, etc.).

---

1. Buffer lifetime bugs
────────────────────

Pitfall:
Sending pointers to temporary or freed memory.

Why:
Async or delayed transmission.

Consequence:
Undefined behavior, corrupted packets.

Correct approach:
Ensure ownership and lifetime guarantees until transmission completes.

---

1. Busy polling sockets
────────────────────

Pitfall:
Looping on recv() without event notification.

Why:
Poor event design.

Consequence:
100% CPU usage.

Correct approach:
Use epoll/select/kqueue or proper blocking strategy.

---

1. Ignoring backpressure
────────────────────

Pitfall:
Writing faster than the network can handle.

Why:
Unlimited application-level queues.

Consequence:
Memory explosion, latency amplification.

Correct approach:
Implement flow control and bounded buffers.

---

1. Nagle + delayed ACK surprises
────────────────────

Pitfall:
Small writes causing latency spikes.

Why:
TCP coalescing behavior.

Consequence:
Unexpected delays.

Correct approach:
Use TCP_NODELAY when latency matters.

---

1. Not handling connection teardown properly
────────────────────

Pitfall:
Ignoring half-close or FIN behavior.

Why:
TCP shutdown semantics misunderstood.

Consequence:
Zombie sockets or data loss.

Correct approach:
Handle shutdown(), FIN, and close states explicitly.

---

1. Copy-heavy packet handling
────────────────────

Pitfall:
Repeated memcpy between layers.

Why:
Naive buffer pipelines.

Consequence:
Throughput collapse under load.

Correct approach:
Use zero-copy or buffer pooling strategies.

---

1. Race conditions in multithreaded networking
────────────────────

Pitfall:
Sharing socket state unsafely.

Why:
Concurrent reads/writes.

Consequence:
Data corruption or crashes.

Correct approach:
Clear ownership model or lock-free queues.

---

1. Misinterpreting errno
────────────────────

Pitfall:
Generic error handling.

Why:
Different errno values mean different recovery paths.

Consequence:
False failures or silent bugs.

Correct approach:
Handle recoverable vs fatal errors distinctly.

---

1. Ignoring kernel socket limits
────────────────────

Pitfall:
Running out of file descriptors or buffers.

Why:
OS limits or improper cleanup.

Consequence:
Server stops accepting connections.

Correct approach:
Tune ulimit/sysctl and close sockets reliably.

---

1. No timeout strategy
────────────────────

Pitfall:
Waiting forever on stalled connections.

Why:
Default blocking behavior.

Consequence:
Resource leakage.

Correct approach:
Use timeouts or watchdog logic.

---

1. Incorrect endian handling
────────────────────

Pitfall:
Assuming host byte order.

Why:
Network uses big-endian.

Consequence:
Protocol corruption across architectures.

Correct approach:
Use htons/ntohl consistently.

────────────────────

END OF SHEET

C++ ASYNC NETWORKING PATTERNS — INTERVIEW PREP SHEET

────────────────────

1. Reactor Pattern (Event Loop Model)
────────────────────

Idea:
An event loop waits for socket readiness and dispatches handlers.

Flow:
epoll/kqueue/select → event ready → callback executes.

Why it exists:
Avoid blocking threads per connection.

Strengths:

- Scales to many connections
- Minimal thread overhead
- Predictable latency

Weaknesses:

- Callback complexity
- Harder state management

Interview insight:
You should understand that sockets are non-blocking and readiness-based, not completion-based.

Used when:
High-connection servers, routers, event-driven systems.

---

1. Proactor Pattern (Completion Model)
────────────────────

Idea:
Submit async operations; OS signals completion.

Flow:
Submit async read/write → OS completes → handler executes.

Why it exists:
Separate I/O initiation from completion.

Strengths:

- Cleaner async semantics
- Good OS-level optimization

Weaknesses:

- Platform dependent
- More complex implementation

Interview insight:
Know the difference between readiness (reactor) vs completion (proactor).

Used when:
Windows IOCP, advanced async frameworks.

---

1. Thread Pool + Event Loop Hybrid
────────────────────

Idea:
Event loop handles I/O; worker threads process heavy tasks.

Flow:
I/O event → enqueue work → worker thread processes → result returned.

Why it exists:
Prevent CPU-heavy work from blocking event loop.

Strengths:

- Good CPU utilization
- Maintains responsiveness

Weaknesses:

- Requires synchronization
- Backpressure complexity

Interview insight:
Shows understanding of separating I/O latency from compute latency.

Used when:
Routers performing packet inspection, encryption, routing decisions.

---

1. Coroutine-Based Async (Modern C++)
────────────────────

Idea:
Async code written like synchronous logic using coroutines.

Flow:
await I/O → suspend → resume when ready.

Why it exists:
Eliminate callback hell.

Strengths:

- Readable async logic
- Structured control flow

Weaknesses:

- Requires careful lifetime management
- Debugging complexity

Interview insight:
Coroutines don’t make code faster — they structure async flow.

Used when:
Modern high-performance frameworks.

---

1. Message-Passing / Actor Model
────────────────────

Idea:
Components communicate via queues instead of shared state.

Flow:
Message → queue → worker processes → response.

Why it exists:
Avoid locks and race conditions.

Strengths:

- Scalable concurrency
- Easier reasoning

Weaknesses:

- Queue latency
- Memory overhead

Interview insight:
Demonstrates understanding of isolation over locking.

Used when:
Distributed routers, pipeline architectures.

---

1. Lock-Free Event Pipeline
────────────────────

Idea:
Packets move through stages without blocking locks.

Flow:
NIC → queue → processing stage → forwarding stage.

Why it exists:
Minimize contention under high load.

Strengths:

- High throughput
- Low latency

Weaknesses:

- Complex correctness guarantees

Interview insight:
Important for packet routers or real-time systems.

---

1. Backpressure-Aware Async Design
────────────────────

Idea:
Control producer speed based on consumer capacity.

Flow:
Queue full → signal slow down → drop/throttle.

Why it exists:
Prevent memory and latency explosion.

Strengths:

- Stability under load

Weaknesses:

- Requires system-wide coordination

Interview insight:
Async systems fail without flow control.

---

1. Single-Writer Ownership Model
────────────────────

Idea:
Only one thread mutates connection state.

Why it exists:
Avoid locking complexity.

Strengths:

- Deterministic behavior
- High performance

Weaknesses:

- Requires careful architecture

Interview insight:
Ownership clarity is critical in async networking.

---

1. Batch Processing Pattern
────────────────────

Idea:
Process packets/events in groups instead of individually.

Why it exists:
Reduce syscall and cache overhead.

Strengths:

- Higher throughput
- Better CPU efficiency

Weaknesses:

- Slight latency tradeoff

Interview insight:
Common in routers and VPP-style pipelines.

────────────────────

INTERVIEW META TAKEAWAY
────────────────────

Async networking is about:

- Avoiding blocking
- Managing ownership safely
- Separating I/O from compute
- Handling overload gracefully
- Designing predictable pipelines

A strong candidate understands tradeoffs, not just APIs.

END OF SHEET
