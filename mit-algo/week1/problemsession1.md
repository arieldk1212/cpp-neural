## 1-1
* a -> (f1, f5, f2, f3, f4)
* b-> ({f4, f5, f3}, f2, f1), fixed->({f2, f5}, f3, f1, f4)

## 1-2
* D.insert_first(x) -> first we will choose the data structure that will do
all of the following functions in the most efficient time.
we dont have a data structure that can do all in O(1),
but we will use linked list, in the first function we will add
a new node,  point the old new to the new, and the new
point to the old new.
D.delete_first() -> we save the ptr in the first, we can use doubly
linked list to keep track of the first and last, thus deleting the last
by just getting the D.first;
the same goes for the last remaining functions...

* (a) when using linked doubly linked list we can just extract those two
using the delete, setting head and tail to the desired variables.
using the function we talked about before

* (b)
if k < 1 or k>len(d) -1 return
x = D.del_f()
D.ins_l(x)
shift_left(D, k-1) // recursion

## 1-3
so for that we would need to use a different more complex data structure
due to the fact that we want to combine efficient fucntions from two
different data structures, thus we will use doubly linked list
or create an array that allocates new exatra space and merges them.

## 1-4

* a -> find nth node, reverse next pointer of everything after nth node, and then cleanup ends, meaning change pointers and stuff.
* b ->
```
void reorder_students(L l) {
  int n = l.size() / 2;
  a = l.head;
  for (int i : l) {
    a = a.next;
  }
  b = a.next;
  x_p = a;
  x = b;
  for (int i : n) {
    x_n = x.next;
    x.next = x_p;
    x_p = x;
    x = x_n;
  }
  c = x_p;
  a.next = c;
  b.next = null;
}
```
