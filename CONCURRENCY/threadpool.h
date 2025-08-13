#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <stdexcept>
#include <thread>

namespace Datavar {
static constexpr int DATA_SIZE{100};
static constexpr int THREADS{100};
static constexpr float MS = 0.001;
}; // namespace Datavar

namespace RandomGenerator {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> AlgoDist(1, 5);
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
    if (Name == "Bubble Sort") {
      for (size_t i = 0; i < Data.size() - 1; ++i) {
        for (size_t j = 0; j < Data.size() - i - 1; ++j) {
          if (Data[j] > Data[j + 1]) {
            std::swap(Data[j], Data[j + 1]);
          }
        }
      }
      return true;
    } else if (Name == "Selection Sort") {
      unsigned int Min = Data[0];
      for (size_t i = 0; i < Data.size(); ++i) {
        int MinIndx = i;
        for (size_t j = i + 1; j < Data.size(); ++j) {
          if (Data[j] < Data[MinIndx]) {
            MinIndx = j;
          }
        }
        std::swap(Data[i], Data[MinIndx]);
      }
      return true;
    } else if (Name == "Insertion Sort") {
      for (size_t i = 0; i < Data.size(); ++i) {
        int Key = Data[i];
        int Prev = i - 1;
        while (Prev >= 0 && Key < Data[Prev]) {
          Data[Prev + 1] = Data[Prev];
          Prev--;
        }
        Data[Prev + 1] = Key;
      }
      return true;
    } else if (Name == "Merge Sort") {
      auto Merge = [](std::vector<unsigned int> &Data, unsigned int l,
                      unsigned int m, unsigned int r) -> void {
        int n1 = m - l + 1;
        int n2 = r - m;
        std::vector<int> lv(n1), rv(n2);
        for (int i = 0; i < n1; ++i) {
          lv[i] = Data[l + i];
        }
        for (int i = 0; i < n2; ++i) {
          rv[i] = Data[m + 1 + i];
        }
        int i = 0, j = 0;
        int k = l;
        while (i < n1 && j < n2) {
          if (lv[i] <= rv[i]) {
            Data[k] = lv[i];
            i++;
          } else {
            Data[k] = rv[j];
            j++;
          }
          k++;
        }
        while (i < n1) {
          Data[k] = lv[i];
          i++;
          k++;
        }
        while (j < n2) {
          Data[k] = rv[j];
          j++;
          k++;
        }
      };
      auto MergeSort = [&](auto &&self, std::vector<unsigned int> &Data,
                           unsigned int left, unsigned int right) -> void {
        if (left >= right) {
          return;
        }
        int mid = left + (right - left) / 2;
        self(self, Data, left, mid);
        self(self, Data, mid + 1, right);
        Merge(Data, left, mid, right);
      };
      MergeSort(MergeSort, Data, 0, Data.size() - 1);
      return true;
    } else if (Name == "Heap Sort") {
      auto Heapify = [&](auto &&self, std::vector<unsigned int> &Data,
                         unsigned int n, unsigned int i) -> void {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < n && Data[l] > Data[largest]) {
          largest = l;
        }
        if (r < n && Data[r] > Data[largest]) {
          largest = r;
        }
        if (largest != i) {
          std::swap(Data[i], Data[largest]);
          self(self, Data, n, largest);
        }
      };
      auto HeapSort = [&](std::vector<unsigned int> &Data) -> void {
        int n = Data.size();
        for (size_t i = n / 2 - 1; i >= 0; i--) {
          Heapify(Heapify, Data, n, i);
        }
        for (size_t i = n - 1; i > 0; i--) {
          std::swap(Data[0], Data[i]);
          Heapify(Heapify, Data, i, 0);
        }
      };
      HeapSort(Data);
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
      m_ScrambledData[i] = RandomGenerator::DataDist(RandomGenerator::gen);
    }
    m_SortedData = m_ScrambledData;
    std::sort(m_SortedData.begin(), m_SortedData.end());
  }

  bool JobStatus() const { return m_JobStatus; }

  Algorithms &GetAlgorithms() { return m_Algorithm; }

  std::pair<std::vector<unsigned int>, std::string> GetRequirements() {
    /**
     * @brief this function return the scrambled data and the algorithm that
     * needs to be used.
     */
    return {m_ScrambledData, m_Algorithm.GetChosenAlgo()};
  }

  void IsJobFinished(const std::vector<unsigned int> &Data) {
    if (SetCorrectedData(Data)) {
      m_JobStatus = true;
    } else {
      m_JobStatus = false;
    }
  }

private:
  bool SetCorrectedData(const std::vector<unsigned int> &Data) {
    if (Data.size() != m_ScrambledData.size()) {
      throw std::length_error("Sizes Are Different.");
      return false;
    }
    if (m_SortedData == Data) {
      return true;
    }
    return false;
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
    std::cout << "WORKER BENCHMARKED AT ------> " << m_BenchmarkTime;
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
  Worker() : m_JobStatus(false), m_Cache("") {}
  ~Worker() { m_WorkerStats.~WorkerBenchmark(); }

  bool GetWorkerStatus() const { return m_JobStatus; }

  const WorkerBenchmark &GetWorkerStats() const { return m_WorkerStats; }

  void ExecuteJob(Job &job) {
    auto Data = job.GetRequirements();
    if (m_Cache == Data.second) {
      job.GetAlgorithms()(m_Cache, Data.first);
    } else {
      m_Cache = Data.second;
      job.GetAlgorithms()(m_Cache, Data.first);
    }
    job.IsJobFinished(Data.first);
    if (job.JobStatus()) {
      m_JobStatus = true;
    }
  }

private:
  bool m_JobStatus;
  std::string m_Cache;
  WorkerBenchmark m_WorkerStats;
};

class ConQueue {
  /**
   * @brief Stores job objects, Allows multiple threads to push (Producers) and
   * pop (Consumers) without corrupting data.
   */
public:
  ConQueue() = default;
  ~ConQueue() = default;

  bool IsEmpty() const { return m_Jobs.empty(); }

  void PushJob(Job &&job) { m_Jobs.push(std::move(job)); }

  Job PopJob() {
    auto job = m_Jobs.front();
    m_Jobs.pop();
    return job; // RVO
  }

private:
  std::queue<Job> m_Jobs;
};

class Producer {
  /**
   * @brief Generates the job, pushes them into the ConQueue.
   */
public:
  Producer(std::shared_ptr<ConQueue> SharedQueue)
      : m_SharedQueue(SharedQueue) {}
  ~Producer() = default;

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
  Consumer(std::shared_ptr<ConQueue> SharedQueue)
      : m_SharedQueue(SharedQueue) {}
  ~Consumer() = default;

  void RunJob() { m_Worker.ExecuteJob(m_CurrentJob); }

  std::optional<Job> GetJob() {
    if (!m_SharedQueue->IsEmpty()) {
      m_CurrentJob = m_SharedQueue->PopJob();
      return m_CurrentJob;
    }
    return std::nullopt;
  }

private:
  Job m_CurrentJob;
  Worker m_Worker;
  std::shared_ptr<ConQueue> m_SharedQueue;
};

class TPool {
public:
  TPool()
      : m_SharedQueue(std::make_shared<ConQueue>()), m_Stop(false),
        m_Consumer(std::make_unique<Consumer>(m_SharedQueue)),
        m_Producer(std::make_unique<Producer>(m_SharedQueue)) {
    for (size_t i = 0; i < Datavar::THREADS; ++i) {
      m_Threads.emplace_back([this] {
        for (;;) {
          std::unique_lock<std::mutex> lock(m_TTMtx);
          m_TTCondv.wait(
              lock, [this] { return m_Stop || !m_SharedQueue->IsEmpty(); });
          if (m_Stop && m_SharedQueue->IsEmpty()) {
            return;
          }
          m_Consumer->GetJob();
          if (m_Consumer->GetJob().has_value()) {
            m_SharedQueue->PopJob();
            lock.unlock();
            m_Consumer->RunJob();
          }
        }
      });
    }
  }
  ~TPool() {
    std::unique_lock<std::mutex> lock(m_TTMtx);
    m_Stop = true;
    lock.unlock();
    m_TTCondv.notify_all();
    for (auto &Thread : m_Threads) {
      Thread.join();
    }
  }

  void Enque() {
    std::unique_lock<std::mutex> lock(m_TTMtx);
    m_Producer->Produce();
    lock.unlock();
    m_TTCondv.notify_one();
  }

private:
  std::mutex m_TTMtx;
  std::condition_variable m_TTCondv;

private:
  bool m_Stop;
  std::vector<Worker> m_Workers;
  std::vector<std::thread> m_Threads;
  std::unique_ptr<Consumer> m_Consumer;
  std::unique_ptr<Producer> m_Producer;
  std::shared_ptr<ConQueue> m_SharedQueue;
};

#endif