template<typename S, typename T>
class D {
public:
  D(S ds, T ds_type, int size)
  : m_ds(ds), m_type(ds_type), m_s_size(size) {
  }
private:
  S m_ds;
  T m_type;
  int m_s_size;
};

int del_first(int arr[]) {
  int x;
  x << arr[0];
}

void shift_left(int arr[], int num) {
  if (num < 1 || num > (5-1)) {
    return;
  } else {

  }
}


struct Node {
  int data;
  Node* next;
};

class Linkedlist {
public:
  void insert(int x) {
    Node* node = new Node; // heap allocated arr, dynamic.
    node->data = x; // appending data to m_data.
    node->next = head; // current
    head = node; // building.
  }
private:
  Node* head;
};


int main () {

  int arr[5] = {5,2,1,3,4};



 return 0;
}
