# Parallel and Grid Computing

## References

* [cppreference](https://en.cppreference.com/w/): The best C++ online reference around.
* [isocpp](https://isocpp.org/): Some blog posts about C++, tips and reference papers.
* [Programming -- Principles and Practice Using C++ (3rd Edition)](https://www.stroustrup.com/programming.html): Book for C++ beginners.
* [A Tour of C++, third edition](https://www.stroustrup.com/tour3.html): Book if you already know basics of C++, it covers C++20.

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
Suppose an integer vector `v` with the values `1,5,4`, then `map(v, [](int x) { return x * 2; })` modifies `v` in-place and double each value.
2. `1-introduction-cpp/exercises/vector.hpp`: Implement a left-fold function (aka. "reduce") which takes a vector, an accumulator and a function `f`.
For instance, `fold_left(v, 0, [](int accu, int x) { return accu - x; })` returns the value `-8` (the difference of the integers in vector `v` from left to right).
[More information](https://en.wikipedia.org/wiki/Fold_(higher-order_function)).
3. [Optional⭐] Same as (2) but with `fold_right`, so it is from right to left.
4. [Optional⭐] Rewrite the loops in `Rule184.cpp` using `map`. If it is not possible, introduce a new generic combinator similar to map to achieve your aim.
