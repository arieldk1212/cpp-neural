#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <algorithm>
#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

/**
 * @brief to create our thread pool we need to first take care of a concurrent
 * queue - add tasks, create a threadworkers data structure, execution
 * @brief create a simple broker, prod consumer.
 * @todo FLOW -> Producer (Creates a Job), ConQueue (Holds the Job), Consumer
 * (Pulls Job from ConQueue), Worker (Executes Job Logic).
 */

struct RandomGenerator {
  unsigned int operator()() {
    srand(time(0));
    return 1 + (rand() % 101);
  }
};

class Job {
public:
  Job()
      : m_JobStatus{false}, m_DataSize{100},
        m_SortingAlgorithms{"Bubble Sort", "Selection Sort", "Insertion Sort",
                            "Merge Sort",  "Quick Sort",     "Heap Sort"},
        m_ScrambledData(m_DataSize) {
    for (int i = 0; i < m_DataSize; ++i) {
      m_ScrambledData[i] = RandomGenerator()();
    }
  }
  ~Job() = default;

  bool JobStatus() const { return m_JobStatus; }
  std::pair<std::vector<unsigned int>, std::string> GetRequirements() const {
    /**
     * @brief this function return the scrambled data and the algorithm that
     * needs to be used.
     */
    return {m_ScrambledData, m_SortingAlgorithms[rand() % m_DataSize]};
  }

  void IsJobFinished(const std::vector<unsigned int> &Data) {
    std::vector<unsigned int> SortedData = m_ScrambledData;
    if (SetCorrectedData(Data)) {
      std::sort(SortedData.begin(), SortedData.end());
      if (SortedData == m_ScrambledData) {
        m_JobStatus = true;
      }
    } else {
      m_JobStatus = false;
    }
  }

private:
  bool SetCorrectedData(const std::vector<unsigned int> &Data) {
    if (Data.size() != m_ScrambledData.size()) {
      throw "Sizes are different.";
      return false;
    }
    m_ScrambledData = Data;
    return true;
  }

private:
  bool m_JobStatus;
  size_t m_DataSize;
  std::vector<unsigned int> m_ScrambledData;
  std::vector<std::string> m_SortingAlgorithms;
};

struct Algorithms {
  /**
   * @todo implement each algorithm here.
   */
  bool operator()(const std::string &Name, std::vector<unsigned int> &Data) {
    if (Name == "Bubble Sort") {
      for (size_t i = 0; i < Data.size(); ++i) {
        for (size_t j = 0; j < Data.size(); ++j) {
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
        unsigned int Key = Data[i];
        unsigned int Prev = i - 1;
        while (Prev >= 0 && Key < Data[Prev]) {
          Data[Prev + 1] = Data[Prev];
          Prev--;
        }
        Data[Prev + 1] = Key;
      }
      return true;
    } else if (Name == "Merge Sort") {
      return true;
    } else if (Name == "Quick Sort") {
      return true;
    } else if (Name == "Heap Sort") {
      return true;
    }
    return false;
  }
};

class Worker {
  /**
   * @brief Executes the job, Report result or timing to WorkerBenchmark.
   */
public:
  Worker() : m_JobStatus(false) {}
  ~Worker() = default;

  void ExecuteJob(Job &job) {
    std::string Algo = job.GetRequirements().second;
    std::vector<unsigned int> ScrambledData = job.GetRequirements().first;
    m_Algos(Algo, ScrambledData);
    job.IsJobFinished(ScrambledData);
    if (job.JobStatus()) {
      m_JobStatus = true;
    }
  }

private:
  Algorithms m_Algos;
  bool m_JobStatus;
  std::thread m_Worker;
};
class WorkerBenchmark {};

class ConQueue {
  /**
   * @brief Stores job objects, Allows multiple threads to push (Producers) and
   * pop (Consumers) without corrupting data.
   */
public:
  ConQueue() = default;
  ~ConQueue() = default;

  void PushJob(Job &&job) { m_ActiveJobs.push(std::move(job)); }
  [[nodiscard("Get The Job!")]] Job PopJob() {
    auto job = m_ActiveJobs.front();
    m_ActiveJobs.pop();
    return job;
  }

private:
  std::queue<Job> m_ActiveJobs;
};

class Producer {
  /**
   * @brief Generates the job, pushes them into the ConQueue.
   */

public:
  Producer(std::shared_ptr<ConQueue> SharedQueue)
      : m_SharedQueue(SharedQueue) {}
  ~Producer() = default;

  void GenerateJob() {
    Job job;
    m_SharedQueue->PushJob(std::move(job));
  }

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

  void RunJob() {
    GetJob();
    m_Worker.ExecuteJob(m_CurrentJob);
  }

private:
  void GetJob() { m_CurrentJob = m_SharedQueue->PopJob(); }

private:
  Job m_CurrentJob;
  Worker m_Worker;
  std::shared_ptr<ConQueue> m_SharedQueue;
};

#endif