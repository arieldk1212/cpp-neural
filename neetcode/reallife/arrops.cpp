#include "arrops.h"
#include <algorithm>

ArrOps::ArrOps(const std::vector<long> &Base) : m_Base(Base) {}

void ArrOps::ReverseBase() {
  int l = 0;
  int r = m_Base.size() - 1;
  while (l < r) {
    long End = m_Base[r];
    m_Base[r] = m_Base[l];
    m_Base[l] = End;
    l++;
    r--;
    // could just use swap
    // std::swap(m_Base[l++], m_Base[r--]);
  }
}

long ArrOps::MaxHourglassSum(const std::vector<std::vector<long>> &mat6x6) {
  long MaxSum = 0;
  for (int i = 0; i <= 3; i++) {
    for (int j = 0; j <= 3; j++) {
      long Sum = mat6x6[i][j] + mat6x6[i][j + 1] + mat6x6[i][j + 2] +
                 mat6x6[i + 1][j + 1] + mat6x6[i + 2][j] +
                 mat6x6[i + 2][j + 1] + mat6x6[i + 2][j + 2];
      MaxSum = std::max(MaxSum, Sum);
    }
  }
}

void ArrOps::InitSequences(int Q) {
  m_Sequences.clear();
  m_Sequences.resize(Q);
}

void ArrOps::AppendToSeq(int i, long value) {
  if (i >= m_Sequences.size() || i < 0) {
    throw "Please Init Sequences on a Bigger Size";
  }
  m_Sequences[i].push_back(value);
}

void ArrOps::RotateBaseLeft(int d) {
  while (d != 0) {
    long Key = m_Base[0];
    m_Base.erase(m_Base.begin());
    m_Base.push_back(Key);
    d--;
  }
}

std::vector<int>
ArrOps::CountPatterns(const std::vector<std::string> &logs,
                      const std::vector<std::string> &patterns) {
  std::vector<int> Res;
  std::unordered_map<std::string, int> Freq;

  for (const auto &log : logs) {
    ++Freq[log];
  }
  for (int i = 0; i < (int)patterns.size(); ++i) {
    Res[i] = Freq[patterns[i]];
  }
  return Res;
}

void ArrOps::RangeAdd(int l, int r, long x) {
  for (int i = l; i <= r; i++) {
    m_Base[i] += x;
  }
}

long ArrOps::MaxAfterAdds() const {
  long Maxx = m_Base[0];
  for (int i = 0; i < m_Base.size(); i++) {
    Maxx = std::max(Maxx, m_Base[i]);
  }
  return Maxx;
}