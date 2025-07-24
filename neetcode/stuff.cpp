#include <set>
#include <unordered_map>
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
    // or we use 2 pointers, 0, data.size()-1, l+, r-- and for sort we use
    // insertion sort (start at 1, save it, get prev, while prev above or zero
    // and key smaller, prev+1 equlles prev, decrement prev, prev+1 = key)
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

  int dynamicslidwin(std::vector<int> data, int k) {
    int l = 0;
    int sum = 0;
    int maxlen = 0;

    for (int r = 0; r < data.size(); r++) {
      sum += data[r];

      while (sum > k) {
        sum -= data[l];
        l++;
      }
      maxlen = std::max(maxlen, r - l + 1);
    }
    return maxlen;
  }

  int staticslidwin(std::vector<int> data, int d, int m) {
    int res = 0;
    int winsum = 0;
    // or if its sum we need to use another int, winsum = maxx, maxx =
    // std::max(maxx, winsum);

    for (int i = 0; i < m; i++) {
      winsum += data[m];
    }
    if (winsum == d) {
      res++;
    }

    for (int i = m; i < data.size(); i++) {
      winsum += data[m] - data[i - m];
      if (winsum == d) {
        res++;
      }
    }

    return res;
  }

  int dynamicslidwintwo(std::string data) {
    int l = 0;
    int maxlen = 0;
    std::unordered_set<char> seen;

    for (int r = 0; r < data.size(); r++) {
      while (seen.count(data[r])) {
        seen.erase(data[l]);
        l++;
      }
      seen.insert(data[r]);
      maxlen = std::max(maxlen, r - l + 1);
    }
    return maxlen;
  }

  int dynamicslidwinthree(std::string data, int k) {
    int l = 0;
    int maxlen = 0;
    std::unordered_map<char, int> freq;

    for (int r = 0; r < data.size(); r++) {
      ++freq[data[r]];

      while (freq.size() > k) {
        --freq[data[l]];
        if (freq[data[l]] == 0) {
          freq.erase(data[l]);
        }
        l++;
      }
      maxlen = std::max(maxlen, r - l + 1);
    }
    return maxlen;
  }
};