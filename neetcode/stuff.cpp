#include <unordered_set>
#include <vector>

struct ListNode {
  int data;
  ListNode *next;
};

class Solution {
public:
  bool hascycle(ListNode *head) {
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
      if (slow == fast) {
        return true;
      }
    }
    return false;
  }

  int findmax(std::vector<int> data) {
    int Max = data[0];
    for (int i = 0; i < data.size(); i++) {
      Max = std::max(Max, data[i]);
    }
    return Max;
  }

  int binarysearch(std::vector<int> &nums, int target) {
    sort(nums.begin(), nums.end());

    int l = 0;
    int h = nums.size() - 1;

    while (l <= h) {
      int mid = l + (h - l) / 2;
      if (nums[mid] == target) {
        return mid;
      }
      if (nums[mid] < target) {
        l = mid + 1;
      } else {
        h = mid - 1;
      }
    }
    return -1;
  }

  bool subarray(std::vector<int> data) {
    std::unordered_set<int> res;
    int sum = 0;
    for (int i = 0; i < data.size(); i++) {
      sum += data[i];
      if (sum == 0 || res.find(sum) != res.end()) {
        return true;
      }
      res.insert(sum);
    }
    return false;
  }

  void reverse(std::vector<int> data) {
    sort(data.begin(), data.end());
    auto n = data.size();
    for (int i = 0; i < data.size() / 2; i++) {
      auto low = data[i];
      data[i] = data[n - i - 1];
      data[n - i - 1] = low;
    }
  }

  void reversenosort(std::vector<int> data) {
    int n = data.size();
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < n - i - 1; ++j) {
        if (data[j] > data[j + 1]) {
          std::swap(data[j], data[j + 1]);
        }
      }
    }

    for (int i = 0; i < data.size() / 2; i++) {
      auto low = data[i];
      data[i] = data[n - i - 1];
      data[n - i - 1] = low;
    }
  }
};