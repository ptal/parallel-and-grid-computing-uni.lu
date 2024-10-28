/*
 * Author: Guillaume HELBECQUE (Université du Luxembourg)
 * Date: 26/10/2024
 *
 * Description:
 * Sequential backtracking to solve instances of the N-Queens problem in Chapel.
 * It serves as a basis for parallel implementations.
 */

use Time;

param INITIAL_CAPACITY = 1024;
param MAX_QUEENS = 20;

config const N = 14;

// pool implementation
record SinglePool {
  type eltType;
  var dom: domain(1);
  var elements: [dom] eltType;
  var capacity: int;
  var front: int;
  var size: int;

  proc init(type eltType) {
    this.eltType = eltType;
    this.dom = {0..#INITIAL_CAPACITY};
    this.capacity = INITIAL_CAPACITY;
  }

  // Insertion to the end of the deque
  proc ref pushBack(node: eltType) {
    if (this.front + this.size >= this.capacity) {
      this.capacity *= 2;
      this.dom = {0..#this.capacity};
    }

    this.elements[this.front + this.size] = node;
    this.size += 1;
  }

  // Removal from the end of the deque
  proc ref popBack() {
    if (this.size > 0) {
      this.size -= 1;
      return this.elements[this.front + this.size];
    }

    var default: eltType;
    return default;
  }
}

// N-Queens node
record Node {
  var depth: uint(8);
  var board: MAX_QUEENS*uint(8);

  proc init() {};

  proc init(const N: int) {
    init this;
    for i in 0..#N do this.board[i] = i:uint(8);
  }

  proc init(other: Node) {
    this.depth = other.depth;
    this.board = other.board;
  }
}

// check if placing a queen is safe (i.e., check if all the queens already placed share
// a same diagonal)
proc isSafe(const board, const row, const col)
{
  for i in 0..#row {
    if (board[i] == col - (row - i) || board[i] == col + (row - i)) {
      return false;
    }
  }

  return true;
}

// evaluate a given node (i.e., check its board configuration) and branch it if it is valid
// (i.e., generate its child nodes.)
proc evaluate_and_branch(const parent: Node, ref tree_loc: uint, ref num_sol: uint, ref pool: SinglePool(Node))
{
  const depth = parent.depth;

  // if the given node is a leaf, then update counter and do nothing
  if (depth == N) {
    num_sol += 1;
  }
  // if the given node is not a leaf, then update counter and evaluate/branch it
  else {
    for j in depth..(N-1) {
      if isSafe(parent.board, depth, parent.board[j]) {
        var child = new Node(parent);
        child.board[depth] <=> child.board[j]; // swap
        child.depth += 1;
        pool.pushBack(child);
        tree_loc += 1;
      }
    }
  }
}

proc main()
{
  writeln("Sequential resolution of the ", N, "-Queens instance in Chapel");

  // initialization of the root node (the board configuration where no queen is placed)
  var root = new Node(N);

  // initialization of the pool of nodes (stack -> DFS exploration order)
  var pool = new SinglePool(Node);
  pool.pushBack(root);

  // statistics to check correctness (number of nodes explored and number of solutions found)
  var exploredTree: uint = 0;
  var exploredSol: uint = 0;

  // beginning of the Depth-First tree-Search
  var timer: stopwatch;
  timer.start();

  while pool.size != 0 {
    // get a node from the pool
    var parent = pool.popBack();

    // check the board configuration of the node and branch it if it is valid
    evaluate_and_branch(parent, exploredTree, exploredSol, pool);
  }

  timer.stop();

  // outputs
  writeln("\nSize of the explored tree: ", exploredTree);
  writeln("Number of explored solutions: ", exploredSol);
  writeln("Elapsed time: ", timer.elapsed(), " [s]");

  return 0;
}
