#ifndef ARR_OPS_H
#define ARR_OPS_H

#include <unordered_map>
#include <vector>

class ArrOps {
public:
  // Initialize with a base array of N elements.
  ArrOps(const std::vector<long> &Base);
  ~ArrOps() = default;
  // 1. Reverse the base array in-place.
  void ReverseBase();
  // 2. Given a fixed 6×6 matrix, return the maximum "hourglass" sum.
  //    An hourglass is defined as:
  //      a b c
  //        d
  //      e f g
  long MaxHourglassSum(const std::vector<std::vector<long>> &mat6x6);
  // 3. Create Q named sequences, indexed 0…Q-1.
  void InitSequences(int Q);
  // 4. Append 'value' to sequence at index i.
  void AppendToSeq(int i, long value);
  // 5. Left-rotate the base array by d positions.
  void RotateBaseLeft(int d);
  // 6. Given:
  //      - a vector<string> logs
  //      - a vector<string> patterns
  //    return a vector<int> counts where counts[j] is
  //    how many times patterns[j] appears in logs.
  std::vector<int> CountPatterns(const std::vector<std::string> &logs,
                                 const std::vector<std::string> &patterns);
  // 7. Range-add operations on the base array:
  //    apply (add 'x') to every element in [l..r].
  void RangeAdd(int l, int r, long x);
  // 8. After any number of rangeAdd calls, return the maximum value
  //    in the base array.
  long MaxAfterAdds() const;

private:
  std::vector<long> m_Base;
  std::vector<std::vector<long>> m_Sequences;
};

#endif