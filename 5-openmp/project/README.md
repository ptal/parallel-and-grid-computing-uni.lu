# Project: Big Graph Processing in OpenMP

## Important Information

### Rules

* Send the code and report in "name.surname.biggraph.zip" before the next course begins (25th October 13h59) by email with the subject "[MHPC][PGC] BigGraph Project".
* This project accounts for 10% of the total grade.
* Don't share your code.
* This is a solo project.
* Want a deadline extension?
    - 1st November 13h59 with a penalty of 2 points (you will be graded over 18 points).
    - 8th November 13h59 with a total penalty of 5 points (you will be graded over 15 points).
    - 15th November 13h59 with a total penalty of 10 points (you will be graded over 10 points).
* Discuss your results and design in a small PDF report, in particular:
    - Run bfs_grader and insert a copy of the score table in your solutions.
    - Describe the process of optimizing your code:
     * In Part 1 (Top Down) and 2 (Bottom Up), where is the synchronization in each of your solutions? Do you do anything to limit the overhead of synchronization?
     * In Part 3 (Hybrid), did you decide to switch between the top-down and bottom-up BFS implementations dynamically? How did you decide which implementation to use?
    - Why do you think your code (and the staff reference) is unable to achieve perfect speedup? (Is it workload imbalance? communication/synchronization? data movement?)
* Use OpenMP for the parallelism (compare and swap operation allowed).
* The code provided is C-ished, not the beautiful C++11 (and above) standard we have seen previously. You must get trained to read the code of others, which is not necessarily beautiful.
* Get extra 2 points (over 100) on your final average if you turn the whole code into C++ (it can be hard, and you must maintain compatibility with `ref_bfs.o`!).
* To have an indication of how efficient is your approach, you can use:
```
./bfs_grader <PATH_TO_GRAPHS_DIRECTORY>
```
your final grade will include more than just this "performance grade", including the report, the correctness (no datarace), and the code in general.
Note that it is quite hard to get the full score according to this program.

### Overview

In this assignment, you will implement a parallel [breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search) (BFS) using OpenMP.
A good implementation of this assignment will be able to run this algorithm on graphs containing hundreds of millions of edges on a multi-core machine in only seconds.

### Ressources

* [Official OpenMP documentation](http://openmp.org/)
* The slides of the course on OpenMP.
* [This is a nice guide](http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf) for the `omp parallel_for` directives.

## Background

### Representing Graphs

The code provided operates on directed graphs, whose implementation is in `graph.h` and `graph_internal.h`.
We recommend you begin by understanding the graph representation in these files.
A graph is represented by an array of edges (both `outgoing_edges` and `incoming_edges`), where each edge is represented by an integer describing the id of the destination vertex.
Edges are stored in the graph sorted by their source vertex, so the source vertex is implicit in the representation.
This makes for a compact representation of the graph, and also allows it to be stored contiguously in memory.
For example, to iterate over the outgoing edges for all nodes in the graph, you'd use the following code which makes use of convenient helper functions defined in `graph.h` (and implemented in `graph_internal.h`):
```c
for (int i=0; i<num_nodes(g); i++) {
    // Vertex is typedef'ed to an int. Vertex* points into g.outgoing_edges[]
    const Vertex* start = outgoing_begin(g, i);
    const Vertex* end = outgoing_end(g, i);
    for (const Vertex* v=start; v!=end; v++)
    printf("Edge %u %u\n", i, *v);
}
```

### Dataset

In this project, you will use a large graph dataset to test the performance.
The dataset can be downloaded from <http://cs149.stanford.edu/cs149asstdata/all_graphs.tgz>.
You can download the dataset using `wget http://cs149.stanford.edu/cs149asstdata/all_graphs.tgz`, and then untar it with `tar -xzvf all_graphs.tgz`.
Be careful, this is a 3 GB download.

Some interesting real-world graphs include:

 * com-orkut_117m.graph 
 * oc-pokec_30m.graph
 * soc-livejournal1_68m.graph
 
Your useful synthetic, but large graphs include:

 * random_500m.graph
 * rmat_200m.graph

There are also some very small graphs for testing.

## Part 1: Parallel "Top Down" Breadth-First Search (50 points)

Breadth-first search (BFS) is a common algorithm that might have seen in a prior algorithms class (See [here](https://www.hackerearth.com/practice/algorithms/graphs/breadth-first-search/tutorial/) and [here](https://www.youtube.com/watch?v=oDqjPvD54Ss) for helpful references.)
Please familiarize yourself with the function `bfs_top_down()` in `bfs/bfs.cpp`, which contains a sequential implementation of BFS.
The code uses BFS to compute the distance to vertex 0 for all vertices in the graph.
You may wish to familiarize yourself with the graph structure defined in `common/graph.h` as well as the simple array data structure `vertex_set` (`bfs/bfs.h`), which is an array of vertices used to represent the current frontier of BFS.

You can run bfs using:

```
./bfs <PATH_TO_GRAPHS_DIRECTORY>/rmat_200m.graph
```

where `<PATH_TO_GRAPHS_DIRECTORY>` is the path to the directory containing the graph files (see the "Dataset" section above).

When you run `bfs`, you'll see execution time and the frontier size printed for each step in the algorithm (if `VERBOSE` is set to 1).
Correctness will pass for the top-down version (since we've given you a correct sequential implementation), but it will be slow.
(Note that `bfs` will report failures for a "bottom up" and "hybrid" versions of the algorithm, which you will implement later in this assignment.)

In this part of the assignment your job is to parallelize top-down BFS.
You'll need to focus on identifying parallelism, as well as inserting the appropriate synchronization to ensure correctness.
We wish to remind you that you __should not__ expect to achieve near-perfect speedups on this problem (we'll leave it to you to think about why!).

__Tips/Hints:__

* Always start by considering what work can be done in parallel.
* Some parts of the computation may need to be synchronized, for example, by wrapping the appropriate code within a critical region using `#pragma omp critical` or `#pragma omp atomic`.
__However, in this problem you should think about how to make use of the simple atomic operation called `compare and swap`.__  You can read about [GCC's implementation of compare and swap](http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html), which is exposed to C code as the function `__sync_bool_compare_and_swap`.
If you can figure out how to use compare-and-swap for this problem, you will achieve much higher performance than using a critical region.
* Updating a shared counter can be done efficiently using `#pragma omp atomic` before a line like `counter++;`.
* Are there conditions where it is possible to avoid using `compare_and_swap`?  In other words, when you *know* in advance that the comparison will fail?
* There is a preprocessor macro `VERBOSE` to make it easy to disable useful print per-step timings in your solution (see the top of `bfs/bfs.cpp`).
In general, these printfs occur infrequently enough (only once per BFS step) that they do not notably impact performance, but if you want to disable the printfs during timing, you can use this `#define` as a convenience.

## Part 2: "Bottom Up" BFS (25 points) ##

Think about what behavior might cause a performance problem in the BFS implementation from Part 1.
An alternative implementation of a breadth-first search step may be more efficient in these situations.
Instead of iterating over all vertices in the frontier and marking all vertices adjacent to the frontier, it is possible to implement BFS by having *each vertex check whether it should be added to the frontier!*.
Basic pseudocode for the algorithm is as follows:

```
for each vertex v in graph:
  if v has not been visited AND
    v shares an incoming edge with a vertex u on the frontier:
      add vertex v to frontier;
```

This algorithm is sometimes referred to as a "bottom up" implementation of BFS, since each vertex looks "up the BFS tree" to find its ancestor.
(As opposed to being found by its ancestor in a "top down" fashion, as was done in Part 1.)

Please implement a bottom-up BFS to compute the shortest path to all the vertices in the graph from the root (see `bfs_bottom_up()` in `bfs/bfs.cpp`).
Start by implementing a simple sequential version.
Then parallelize your implementation.

__Tips/Hints:__

* It may be useful to think about how you represent the set of unvisited nodes.
Do the top-down and bottom-up versions of the code lend themselves to different implementations?
* How do the synchronization requirements of the bottom-up BFS change?

## Part 3: Hybrid BFS (25 points) ##

Notice that in some steps of the BFS, the "bottom up" BFS is signficantly faster than the top-down version.
In other steps, the top-down version is signficantly faster.
This suggests a major performance improvement in your implementation, if __you could dynamically choose between your "top down" and "bottom up" formulations based on the size of the frontier or other properties of the graph!__
If you want a solution competitive with the reference one, your implementation will likely have to implement this dynamic optimization.
Please provide your solution in `bfs_hybrid()` in `bfs/bfs.cpp`.

__Tips/Hints:__

* If you used different representations of the frontier in Parts 1 and 2, you may have to convert between these representations in the hybrid solution.
How might you efficiently convert between them? Is there an overhead in doing so?

## Acknowledgments

This project is adapted from an assignment of the course [Parallel Computing](https://gfxcourses.stanford.edu/cs149/fall23/) at Stanford and used with the permission of Kayvon Fatahalian.
Thanks to him and his teaching team!
