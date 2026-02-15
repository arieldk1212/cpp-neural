Rule of six

if you define any of the next special methods, you should probably define all six.

1. default ctor
2. dtor
3. copy ctor
4. copy assignment operator
5. move ctor
6. move assignment operator

or, you can use the rule of zero, dont use any of them, just throw it
to a shared ptr in a vector, it will me your manager.
see DatabaseConnectionManager source code for understanding.

from a confrence, cpp on sea -> Exploring the intricacies of object lifetimes.