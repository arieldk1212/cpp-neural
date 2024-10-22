## problem 1
* a -> (f5, {f1, f1}, {f3, f4})
* b-> (f5, f4, f2, f1, f3)
* c -> (f3, f4, f2, f5, f1)
* d-> (f1, f5 f2, f4, f3)

## problem 2
```
void reverse(D, i, k) {
  for (int j = (i+k)-1; j = i; j--) {
    int arr[] = D[j];
  } 
void reverse(D, i, k) {
  if ( k < 2 ) return;
  x2 = D.delete_at(i + k - 1)
  x1 = D.delete_at(i)
  D.insert_at(i, x2)
  D.insert_at(i + k - 1, x1)
  reverse(D, i + 1, k - 2);
}

void move(D, i, k, j) {
  if (k < 1) return;
  x = D.delete_at(i)
  if j > i j= j - 1
  D.insert_at(j, x)
  j = j+1
  if i > j i = i + 1
  move(D, i, k - 1, j);
}
}
```
## problem 3
* linked list -> Node(int data, Node* next); LinkedList(Node* head);

# QUIZ 1
* 2-> a->n^2