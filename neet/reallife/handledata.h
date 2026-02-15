#ifndef DATA_H
#define DATA_H

#include <ctime>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * order in an ascending order by timestamp, message in lexicographical order.
 * (abc). if two logs are the same, the one with the smaller id comes first.
 * timestamp format -> YYYY-MM-DD HH:MM:SS.
 */

struct Log {
  int m_LogId;
  std::string m_Timestamp;
  std::string m_Message;

  bool operator>(const Log &other) {
    if (this->m_Timestamp != other.m_Timestamp) {
      return this->m_Timestamp > other.m_Timestamp;
    }
    if (this->m_Message != other.m_Message) {
      return this->m_Message > other.m_Message;
    }
    return this->m_LogId > other.m_LogId;
  }
};

class HandleData {
public:
  HandleData(const std::vector<Log> &Entries);
  ~HandleData() = default;

  /**
   * @brief check if message is valid
   * @param Message
   * @return true
   * @return false
   */
  bool MessageValid(const std::string &Message);
  /**
   * @brief check if timestamp is valid
   * @param Timestamp
   * @return true
   * @return false
   */
  bool TimestampValid(const std::string &Timestamp);
  /**
   * @brief sorts the entries
   * @return std::vector<Log>
   */

  std::vector<Log> SortLogs();

private:
  std::vector<Log> m_Entries;
};

#endif