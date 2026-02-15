#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <stdexcept>
#include <thread>

namespace Datavar {
static constexpr float MS = 0.001f;
static constexpr int DATA_SIZE{100};
static const unsigned int THREADS{std::thread::hardware_concurrency()};
}; // namespace Datavar

namespace RandomGenerator {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> AlgoDist(0, 4);
static std::uniform_int_distribution<> DataDist(1, 101);
}; // namespace RandomGenerator

class Algorithms {
public:
  Algorithms()
      : m_Algorithms{"Bubble Sort", "Selection Sort", "Insertion Sort",
                     "Merge Sort", "Heap Sort"} {
    m_ChosenAlgo =
        m_Algorithms[RandomGenerator::AlgoDist(RandomGenerator::gen)];
  }
  std::string GetChosenAlgo() const { return m_ChosenAlgo; }
  bool operator()(const std::string &Name, std::vector<unsigned int> &Data) {
    if (Data.size() <= 1)
      return true;

    if (Name == "Bubble Sort") {
      for (size_t i = 0; i + 1 < Data.size(); ++i) {
        for (size_t j = 0; j + 1 < Data.size() - i; ++j) {
          if (Data[j] > Data[j + 1])
            std::swap(Data[j], Data[j + 1]);
        }
      }
      return true;

    } else if (Name == "Selection Sort") {
      for (size_t i = 0; i < Data.size(); ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < Data.size(); ++j) {
          if (Data[j] < Data[min_idx])
            min_idx = j;
        }
        std::swap(Data[i], Data[min_idx]);
      }
      return true;

    } else if (Name == "Insertion Sort") {
      for (size_t i = 1; i < Data.size(); ++i) {
        unsigned int key = Data[i];
        // use signed index to avoid unsigned underflow
        for (std::ptrdiff_t p = static_cast<std::ptrdiff_t>(i) - 1;
             p >= 0 && key < Data[static_cast<size_t>(p)]; --p) {
          Data[static_cast<size_t>(p) + 1] = Data[static_cast<size_t>(p)];
          if (p == 0 || !(key < Data[static_cast<size_t>(p - 1)])) {
            Data[static_cast<size_t>(p)] = key;
            break;
          }
        }
        // handle the case where we shifted all the way to index 0
        if (key < Data[0])
          Data[0] = key;
      }
      return true;

    } else if (Name == "Merge Sort") {
      auto Merge = [](std::vector<unsigned int> &A, size_t l, size_t m,
                      size_t r) {
        const size_t n1 = m - l + 1;
        const size_t n2 = r - m;
        std::vector<unsigned int> L(n1), R(n2);
        for (size_t i = 0; i < n1; ++i)
          L[i] = A[l + i];
        for (size_t j = 0; j < n2; ++j)
          R[j] = A[m + 1 + j];
        size_t i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
          if (L[i] <= R[j])
            A[k++] = L[i++];
          else
            A[k++] = R[j++];
        }
        while (i < n1)
          A[k++] = L[i++];
        while (j < n2)
          A[k++] = R[j++];
      };
      auto MergeSort = [&](auto &&self, std::vector<unsigned int> &A,
                           size_t left, size_t right) -> void {
        if (left >= right)
          return;
        const size_t mid = left + (right - left) / 2;
        self(self, A, left, mid);
        self(self, A, mid + 1, right);
        Merge(A, left, mid, right);
      };
      MergeSort(MergeSort, Data, 0, Data.size() - 1);
      return true;

    } else if (Name == "Heap Sort") {
      auto Heapify = [&](auto &&self, std::vector<unsigned int> &A, size_t n,
                         size_t i) -> void {
        for (;;) {
          size_t largest = i;
          const size_t l = 2 * i + 1;
          const size_t r = 2 * i + 2;
          if (l < n && A[l] > A[largest])
            largest = l;
          if (r < n && A[r] > A[largest])
            largest = r;
          if (largest == i)
            break;
          std::swap(A[i], A[largest]);
          i = largest;
        }
      };
      const size_t n = Data.size();
      for (size_t i = n / 2; i-- > 0;)
        Heapify(Heapify, Data, n, i);
      for (size_t i = n; i-- > 1;) {
        std::swap(Data[0], Data[i - 1]);
        Heapify(Heapify, Data, i - 1, 0);
      }
      return true;
    }
    return false;
  }

private:
  std::string m_ChosenAlgo;
  std::array<std::string, 5> m_Algorithms;
};

class Job {
public:
  Job() : m_JobStatus{false}, m_ScrambledData(Datavar::DATA_SIZE) {
    for (int i = 0; i < Datavar::DATA_SIZE; ++i) {
      m_ScrambledData[(size_t)i] =
          (unsigned int)RandomGenerator::DataDist(RandomGenerator::gen);
    }
    m_SortedData = m_ScrambledData;
    std::sort(m_SortedData.begin(), m_SortedData.end());
  }

  bool JobStatus() const { return m_JobStatus; }

  Algorithms &GetAlgorithms() { return m_Algorithm; }

  std::pair<std::vector<unsigned int>, std::string> GetRequirements() {
    return {m_ScrambledData, m_Algorithm.GetChosenAlgo()};
  }

  void IsJobFinished(const std::vector<unsigned int> &Data) {
    m_JobStatus = SetCorrectedData(Data);
  }

private:
  bool SetCorrectedData(const std::vector<unsigned int> &Data) {
    if (Data.size() != m_ScrambledData.size()) {
      throw std::length_error("Sizes Are Different.");
    }
    return m_SortedData == Data;
  }

private:
  bool m_JobStatus;
  Algorithms m_Algorithm;
  std::vector<unsigned int> m_SortedData;
  std::vector<unsigned int> m_ScrambledData;
};

class WorkerBenchmark {
public:
  // in the ctor start the start, dtor stop the time.
  WorkerBenchmark() { m_StartTime = std::chrono::high_resolution_clock::now(); }
  ~WorkerBenchmark() { Stop(); }

  void Stop() {
    std::chrono::time_point<std::chrono::high_resolution_clock> StopTime =
        std::chrono::high_resolution_clock::now();
    auto Start =
        std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
            .time_since_epoch()
            .count();
    auto End = std::chrono::time_point_cast<std::chrono::microseconds>(StopTime)
                   .time_since_epoch()
                   .count();
    m_BenchmarkTime = static_cast<float>(End - Start) * Datavar::MS;
    std::cout << "WORKER BENCHMARKED AT ------> " << m_BenchmarkTime << "\n";
  }

private:
  float m_BenchmarkTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

class Worker {
  /**
   * @brief Executes the job, Report result or timing to WorkerBenchmark.
   */
public:
  Worker() : m_JobStatus(false) {}

  bool GetWorkerStatus() const { return m_JobStatus; }

  const WorkerBenchmark &GetWorkerStats() const { return m_WorkerStats; }

  void ExecuteSortingJob(Job &job) {
    auto Req = job.GetRequirements();
    job.GetAlgorithms()(Req.second, Req.first);
    job.IsJobFinished(Req.first);
    if (job.JobStatus()) {
      m_JobStatus = true;
    }
  }

private:
  bool m_JobStatus;
  WorkerBenchmark m_WorkerStats;
};

class ConQueue {
  /**
   * @brief Stores job objects, Allows multiple threads to push (Producers) and
   * pop (Consumers) without corrupting data.
   */
public:
  ConQueue() : m_Done(false) {}

  void PushJob(Job &&job) {
    {
      std::lock_guard<std::mutex> lock(m_ConMutex);
      m_Jobs.push(std::move(job));
    }
    m_ConCondv.notify_one();
  }

  std::optional<Job> PopJob() {
    std::unique_lock<std::mutex> lock(m_ConMutex);
    m_ConCondv.wait(lock, [this] {
      return m_Done || !m_Jobs.empty();
    }); // wake up (return) if system shut down, or job in queue.
    if (m_Done && m_Jobs.empty()) {
      return std::nullopt;
    }
    auto job = std::move(m_Jobs.front());
    m_Jobs.pop();
    return job;
  }

  void Shutdown() {
    {
      std::lock_guard<std::mutex> lock(m_ConMutex);
      m_Done = true;
    }
    m_ConCondv.notify_all();
  }

private:
  std::mutex m_ConMutex;
  std::queue<Job> m_Jobs;
  std::atomic_bool m_Done;
  std::condition_variable m_ConCondv;
};

class Producer {
  /**
   * @brief Generates the job, pushes them into the ConQueue.
   */
public:
  explicit Producer(std::shared_ptr<ConQueue> SharedQueue)
      : m_SharedQueue(std::move(SharedQueue)) {}

  void Produce() {
    Job job;
    m_SharedQueue->PushJob(std::move(job));
  }

  void ProduceLoop();

private:
  std::shared_ptr<ConQueue> m_SharedQueue;
};

class Consumer {
  /**
   * @brief Reads jobs from the ConQueue and processes them,
   * Waits for job, pop the job off the queue, passes them to a Worker.
   */
public:
  explicit Consumer(std::shared_ptr<ConQueue> SharedQueue)
      : m_SharedQueue(std::move(SharedQueue)) {}

  bool GetJob() {
    auto job = m_SharedQueue->PopJob();
    if (!job) {
      return false;
    }
    m_CurrentJob = std::move(*job);
    return true;
  }

  void RunSortingJob() { m_Worker.ExecuteSortingJob(m_CurrentJob); }

private:
  Job m_CurrentJob;
  Worker m_Worker;
  std::shared_ptr<ConQueue> m_SharedQueue;
};

class TPool {
public:
  TPool() {
    m_SharedQueue = std::make_shared<ConQueue>();
    m_Producer = std::make_unique<Producer>(m_SharedQueue);
    m_Threads.reserve((size_t)Datavar::THREADS);
    for (size_t i = 0; i < (size_t)Datavar::THREADS; ++i) {
      m_Threads.emplace_back([this] {
        Consumer consumer(m_SharedQueue);
        while (consumer.GetJob()) {
          consumer.RunSortingJob();
        }
      });
    }
  }
  ~TPool() {
    m_SharedQueue->Shutdown();
    for (auto &Thread : m_Threads) {
      Thread.join();
    }
  }

  void Enque() { m_Producer->Produce(); }

private:
  std::vector<std::thread> m_Threads;
  std::unique_ptr<Producer> m_Producer;
  std::shared_ptr<ConQueue> m_SharedQueue;
};

#endif