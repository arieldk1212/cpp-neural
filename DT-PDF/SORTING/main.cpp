#include <algorithm>
#include <iostream>
#include <vector>

int main() {

  /* BUBBLE SORT O(n^2) */
  int arr[5] = {1,5,2,1,4};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4-1; j++) {
      if (arr[j] > arr[j+1]) {
        std::swap(arr[j], arr[j+1]); } } }

  /* MERGE SORT O(logn) */

  int arr2[7] = {1,5,2,7,8,3,1};

  int a, b;
  a = arr2[0];
  b = arr2[6];
 
  /**/
  // VECTOR
  std::vector<int> v={ 4, 2, 5, 1, 2 };
  sort(v.begin(), v.end());

  // ARRAY
  int n = 7;
  int z[] = {4,2,1,2,4,};
  std::sort(a, a+n);


  std::vector<std::pair<int,int> > zz;
  zz.push_back({1,5});

  /* BINARY SEARCH */
  
}
