# Parallel and Grid Computing

## References

### About C++

* [cppreference](https://en.cppreference.com/w/): The best C++ online reference around.
* [isocpp](https://isocpp.org/): Some blog posts about C++, tips and reference papers.
* [Programming -- Principles and Practice Using C++ (3rd Edition)](https://www.stroustrup.com/programming.html): Book for C++ beginners.
* [A Tour of C++, third edition](https://www.stroustrup.com/tour3.html): Book if you already know basics of C++, it covers C++20.

### About Parallel Programming

* [E. A. Lee, “The Problem with Threads”](https://digitalassets.lib.berkeley.edu/techreports/ucb/text/EECS-2006-1.pdf): A seminal paper on why shared-state parallelism is hard.
* [The Art of Multiprocessor Programming](https://shop.elsevier.com/books/the-art-of-multiprocessor-programming/herlihy/978-0-12-415950-1): General introduction to parallelism.
* [P. E. McKenney, “Is Parallel Programming Hard, And, If So, What Can You Do About It?”](https://cdn.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html): Advanced textbook on parallelism.


## C++ Crash Course

### Cellular Automaton: Rule 184

1. `1-introduction-cpp/exercises/rule184.cpp`: Implement the [Rule184](https://en.wikipedia.org/wiki/Rule_184).
You write the simulation in a loop using two arrays of cells (the current array and the next array).
To see the evolution of the queue, pause the program 1 second between two iterations.
2. Decompose the program into several functions:
```cpp
print(cells);
simulate(steps, current, next);
simulate_step(current, next);
```
3. Add a functionality to compute the longest size of consecutive '1' at any iteration of the simulation (it represents the longest traffic jam).

### Modularity

See the files in `demo/modular` for setting up a multi-files project with headers (.hpp) and .cpp files.

### Data Structure: Vector

1. `1-introduction-cpp/exercises/int_vector.hpp`: Implement the necessary constructors, destructors and methods given in the file. For additionnal documentation see [std::vector](https://en.cppreference.com/w/cpp/container/vector), we try to follow it as close as possible.
2. `1-introduction-cpp/exercises/vector.hpp`: Update the previous example using a templated class.
3. Extend (2) with a move constructor.
4. [Optional⭐⭐] Extend (3) to allow the type `T` to lack a default constructor (e.g. `Vector<S> x; x.reserve(10);` with `struct S { S(int x) {} };` should work.
  See [placement new](https://en.cppreference.com/w/cpp/language/new) and [std::malloc](https://en.cppreference.com/w/cpp/memory/c/malloc).

### Lambda Function

1. `1-introduction-cpp/exercises/vector.hpp`: Implement a map function taking a vector and applying a function `f` to each of its component.
Suppose an integer vector `v` with the values `1,-5,6`, then `map(v, [](int x) { return x * 2; })` modifies `v` in-place and double each value.
2. `1-introduction-cpp/exercises/vector.hpp`: Implement a left-fold function (aka. "reduce") which takes a vector, an accumulator and a function `f`.
For instance, `fold_left(v, 0, [](int accu, int x) { return accu - x; })` returns the value `2` (the difference of the integers in vector `v` from left to right).
[More information](https://en.wikipedia.org/wiki/Fold_(higher-order_function)).
3. [Optional⭐] Same as (2) but with `fold_right`, so it is from right to left. On the previous example, it returns `-2`.
4. [Optional⭐] Rewrite the loops in `Rule184.cpp` using `map`. If it is not possible, introduce a new generic combinator similar to map to achieve your aim.

## C++ Parallelism with std::thread

### Map/Reduce

* `2-std-thread/demo/parallel_map.cpp`: Do some experiments with different sizes of array. When is it interesting to have 2 threads? Why?
* `2-std-thread/demo/parallel_map.cpp`: Use the function `more_work` and redo the experiments.
* [Optional⭐] `2-std-thread/demo/parallel_map.cpp`: Extend `parallel_map` to any number of threads.
* `2-std-thread/exercises/parallel_fold_left.cpp`: Implement `parallel_fold_left_commutative`  where 2 threads compute on half the array, wait for completion and then both results are merged. Note that the operation must be commutative otherwise you might not get the same result as in sequential computation.
* [Optional⭐] `2-std-thread/exercises/parallel_fold_left.cpp`: Extend `parallel_fold_left` to any number of threads.
* `2-std-thread/exercises/parallel_map_reduce.cpp`: Perform a map in parallel, followed by a fold. Find a (simple) way to reuse the threads from the map to compute the fold. Think about the pros and cons of your design, can we do better?

### Histogram

* `2-std-thread/exercises/histogram.cpp`: Given an array of size N taking value in the range [0,1000], implements a sequential algorithm `histogram_seq` which computes the frequency of each value and store the result in another array `histogram`.
* Parallelize the algorithm using thread and mutex when writing in the histogram array.
* [Optional⭐⭐] Lock-free algorithm: Use atomic to write values in the histogram, in order to avoid locks.

### Project: Parallelizing Rule 184

* Using the C++ threads, parallelize the simulation algorithm of Rule 184. Print only the final iteration and the size of the longest queue obtained.
* The goal is to perform the most iterations in 1 minute for different values of the array.
* Plot your results using Python or any tool to see the impact of 1, 2, ..., N threads on the efficiency according to the size of the array.
* Discuss your results and design in a small PDF report.
* Send the code and report in ".zip" before the next course begins (4th October 13h59) by email with the subject "[MHPC][PGC] Project".
* You can discuss your design and your results on Discord or orally, but please don't share your code.
* This is a solo project.
