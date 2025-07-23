#include "handledata.h"

HandleData::HandleData(const std::vector<Log> &Entries) : m_Entries(Entries) {}

bool HandleData::MessageValid(const std::string &Message) {
  return !Message.empty();
}

bool HandleData::TimestampValid(const std::string &Timestamp) {
  if (Timestamp.size() != 19)
    return false;
  if (Timestamp[4] != '-' || Timestamp[7] != '-' || Timestamp[10] != ' ' ||
      Timestamp[13] != ':' || Timestamp[16] != ':')
    return false;

  try {
    int year = std::stoi(Timestamp.substr(0, 4));
    int month = std::stoi(Timestamp.substr(5, 2));
    int day = std::stoi(Timestamp.substr(8, 2));
    int hour = std::stoi(Timestamp.substr(11, 2));
    int min = std::stoi(Timestamp.substr(14, 2));
    int sec = std::stoi(Timestamp.substr(17, 2));

    if (year < 1970 || year > 2025)
      return false;
    if (month < 1 || month > 12)
      return false;
    if (day < 1 || day > 31)
      return false;
    if (hour < 0 || hour > 23)
      return false;
    if (min < 0 || min > 59)
      return false;
    if (sec < 0 || sec > 59)
      return false;
  } catch (...) {
    return false;
  }
  return true;
  // int l = 0;
  // std::unordered_map<char, int> Delimiters;
  // std::unordered_map<std::string, std::string> Data;

  // if (Timestamp.size() != 19) {
  //   return false;
  // }

  // for (int r = 0; r < Timestamp.length(); ++r) {
  //   if (Timestamp[r] == '-') {
  //     Delimiters['-']++;
  //   }
  //   if (Timestamp[r] == ' ') {
  //     Delimiters[' ']++;
  //   }
  //   if (Timestamp[r] == ':') {
  //     Delimiters[':']++;
  //   }
  //   while (l < Timestamp.length() && Timestamp[l] != '-' &&
  //          Timestamp[l] != ' ' && Timestamp[l] != ':') {
  //     if (Delimiters['-'] == 0) {
  //       Data["Year"] += Timestamp[l];
  //     } else if (Delimiters['-'] == 1) {
  //       Data["Month"] += Timestamp[l];
  //     } else {
  //       Data["Day"] += Timestamp[l];
  //     }
  //     if (Delimiters[' '] && Delimiters[';'] == 0) {
  //       Data["Hour"] += Timestamp[l];
  //     }
  //     if (Delimiters[':'] == 1) {
  //       Data["Min"] += Timestamp[l];
  //     } else {
  //       Data["Sec"] += Timestamp[l];
  //     }
  //     l++;
  //   }

  //   for (auto &[key, value] : Data) {
  //     if (key == "Year" && std::stoi(value) > 2023) {
  //       return false;
  //     }
  //     if (key == "Month" && std::stoi(value) > 12) {
  //       return false;
  //     }
  //     if (key == "Day" && std::stoi(value) > 31) {
  //       return false;
  //     }
  //     if (key == "Hour" && std::stoi(value) > 24) {
  //       return false;
  //     }
  //     if (key == "Min" && std::stoi(value) > 59) {
  //       return false;
  //     }
  //     if (key == "Sec" && std::stoi(value) > 59) {
  //       return false;
  //     }
  //   }
  // }
  // return true;
}

std::vector<Log> HandleData::SortLogs() {
  int minn;
  std::vector<Log> SortedEntries;

  for (int i = 1; i < m_Entries.size(); ++i) {
    auto Key = m_Entries[i];
    auto Prev = i - 1;
    if (!MessageValid(m_Entries[Prev].m_Message) ||
        !TimestampValid(m_Entries[Prev].m_Timestamp)) {
      continue;
    }
    while (Prev >= 0 && m_Entries[Prev] > Key) {
      m_Entries[Prev + 1] = m_Entries[Prev];
      Prev = Prev - 1;
    }
    m_Entries[Prev + 1] = Key;
  }
}