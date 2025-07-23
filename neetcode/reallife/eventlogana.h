#ifndef EVENT_LOG_ANALYZER_H
#define EVENT_LOG_ANALYZER_H

#include <algorithm>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

struct Event {
  int16_t m_EventCode;
  long m_Duration;
  std::string m_Timestamp;

  bool operator<(const Event &e) {
    if (this->m_Timestamp != e.m_Timestamp) {
      return this->m_Timestamp < e.m_Timestamp;
    }
    if (this->m_Duration != e.m_Duration) {
      return this->m_Duration < e.m_Duration;
    }
    return this->m_EventCode < e.m_EventCode;
  }
};

class EventAnalyzer {
public:
  EventAnalyzer(const std::vector<Event> &Events);
  ~EventAnalyzer() = default;

  bool IsValid(const Event &e);
  std::string To24Hour(const std::string &Timestamp);

  void SortEvents();

  std::tuple<double, double, double> CodeFractions();
  std::pair<long, long> MiniMaxDurations();

private:
  std::vector<Event> m_Events;
};

#endif