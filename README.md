# Parallel and Grid Computing

## References

* [cppreference](https://en.cppreference.com/w/): The best C++ online reference around.
* [isocpp](https://isocpp.org/): Some blog posts about C++, tips and reference papers.
* [Programming -- Principles and Practice Using C++ (3rd Edition)](https://www.stroustrup.com/programming.html): Book for C++ beginners.
* [A Tour of C++, third edition](https://www.stroustrup.com/tour3.html): Book if you already know basics of C++, it covers C++20.

## Cellular Automaton: Rule 184

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

## Data Structure: Vector

## Modularity

See the files in `demo/modular` for setting up a multi-files project with headers (.hpp) and .cpp files.
