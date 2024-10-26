#include <iostream>


int delete_at(int* arr, int i) {
  int answer = arr[i];
  arr[i] = 0;
  return answer;
}

void insert_at(int i, int k) {

}

void reverse(int arr[], int i, int k) {
  if ( k < 1) { return; }
  int x = delete_at(arr, i);
  int x2 = delete_at(arr, i+k-1);
  arr[i] = x2;
  arr[i+k-1] = x;
  reverse(arr, i, k);
}

void move(int arr[], int i, int k, int j) {

}

int main () {
  int arr[5] = {1,2,3,4,5};
  int i = 0;
  int k = 5;
  reverse(arr, i, k);
  std::cout << arr;
}

// 1-4
/*

*/