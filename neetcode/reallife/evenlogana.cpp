#include "eventlogana.h"

EventAnalyzer::EventAnalyzer(const std::vector<Event> &Events)
    : m_Events(Events) {}

bool EventAnalyzer::IsValid(const Event &e) {
  if (e.m_Duration < 0) {
    return false;
  }
  if (e.m_Timestamp.size() != 10) {
    return false;
  }
  std::string MaxTime = "12:59:59";
  std::string MinTime = "01:00:00";

  std::string Stamp = e.m_Timestamp.substr(8, 2);
  std::string RawTime = e.m_Timestamp.substr(0, 8);
  if (Stamp != "AM" && Stamp != "PM") {
    return false;
  }
  if (RawTime <= MaxTime || RawTime >= MinTime) {
    return false;
  }
  // or we can use regex for that, because in HackerRank they want regex and not
  // by the ISO 8
  // static const std::regex ValidPattern(
  //     R"(^(0[1-9]|1[0-2]):[0-5]\d:[0-5]\d(AM|PM)$)");
  // if (!std::regex_match(e.m_Timestamp, ValidPattern)) {
  //   return false;
  // }
  return true;
}

std::string EventAnalyzer::To24Hour(const std::string &Timestamp) {
  std::string Stamp = Timestamp.substr(8, 2);
  std::string Hour = Timestamp.substr(0, 2);
  if (Stamp == "AM") {
    if (Hour == "12") {
      return "00" + Timestamp.substr(2, 7);
    }
    return Timestamp.substr(0, 7);
  }
  if (Stamp == "PM") {
    auto ModHour = std::stoi(Hour) + 12;
    return std::to_string(ModHour) + Stamp.substr(2, 7);
  }
  return Timestamp;
}

void EventAnalyzer::SortEvents() {
  m_Events.erase(std::remove_if(m_Events.begin(), m_Events.end(),
                                [&](const Event &e) { return !IsValid(e); }));
  for (int i = 1; i < m_Events.size(); ++i) {
    auto Key = m_Events[i];
    auto Prev = i - 1;

    while (Prev >= 0 && Key < m_Events[Prev]) {
      m_Events[Prev + 1] = m_Events[Prev];
      Prev = Prev - 1;
    }
    m_Events[Prev + 1] = Key;
  }
}

std::tuple<double, double, double> EventAnalyzer::CodeFractions() {
  int Neg, Zero, Pos = 0;
  for (const auto &e : m_Events) {
    if (e.m_EventCode < 0) {
      Neg++;
    } else if (e.m_EventCode > 0) {
      Pos++;
    } else {
      Zero++;
    }
  }
  double PosFrac = Pos / static_cast<double>(m_Events.size());
  double NegFrac = Neg / static_cast<double>(m_Events.size());
  double ZeroFrac = Zero / static_cast<double>(m_Events.size());
  return {PosFrac, NegFrac, ZeroFrac};
  // to print 6 after the point -> std::fixed << std::setprecision(6);
}

std::pair<long, long> EventAnalyzer::MiniMaxDurations() {
  long Total = 0;
  long MinDur = m_Events[0].m_Duration;
  long MaxDur = m_Events[0].m_Duration;

  if (m_Events.empty()) {
    return {0, 0};
  }

  for (const auto &e : m_Events) {
    Total += e.m_Duration;
    MinDur = std::min(MinDur, e.m_Duration);
    MaxDur = std::max(MaxDur, e.m_Duration);
  }
  return {Total - MaxDur, Total - MinDur};
}
