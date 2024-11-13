/*
The Strategy pattern is a behavioral design pattern that allows you to define a family of algorithms,
encapsulate each one as an object, and make them interchangeable at runtime.


What does family of algorithms mean?

It refers to a group of related algorithms that perform similar tasks but may have different implementations or strategies to achieve those tasks.
For example, if you have multiple sorting algorithms like Bubble Sort, Quick Sort, and Merge Sort, they form a family (or a group) of sorting algorithms.
Each algorithm within this family performs the task of sorting elements, but they use different strategies and techniques to accomplish it.


What does encapsulate each one as an object mean?

It means that every strategy or implementation is encapsulated in its own class. Continuing with the sorting example,
all three sorting algorithms will be defined separately in their own classes like: BubbleSortStrategy, MergeSortStrategy, QuickSortStrategy.


What does interchangeable at runtime mean?

It means that the different strategies (or algorithms) encapsulated within their own classes can be
swapped or replaced dynamically during the execution of the program. This indirectly talks about Polymorpishm.
*/

class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(vector<int>& data) = 0;
};

// Concrete Strategies (implementations)
class BubbleSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        // bubble sort algorithm
        cout << "bubblesort\n";
    }
};

class QuickSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        // quick sort algorithm
        cout << "quicksort\n";
    }
};

// Context (uses Strategy interface)
class Sorter {
private:
    SortStrategy* strategy;

public:
    Sorter(SortStrategy* strategy) : strategy(strategy) {}

    void set_strategy(SortStrategy* strategy) {
        this->strategy = strategy;
    }

    void sort(vector<int>& data) const {
        strategy->sort(data);
    }
};

int main() {
    vector<int> numbers = {5, 2, 8, 1, 4};

    Sorter sorter(new BubbleSort); // Use BubbleSort initially
    sorter.sort(numbers);

    // Dynamically change strategy to QuickSort
    sorter.set_strategy(new QuickSort);
    sorter.sort(numbers);

    return 0;
}