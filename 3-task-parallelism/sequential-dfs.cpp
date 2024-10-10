/*
 * Author: Guillaume HELBECQUE (Université du Luxembourg)
 * Date: 10/10/2024
 *
 * Description:
 * This program solves the N-Queens problem using a sequential Depth-First tree-Search
 * (DFS) algorithm. It serves as a basis for task-parallel implementations.
 */

#include <iostream>
#include <cstring>
#include <chrono>
#include <stack>

// N-Queens node
struct Node {
  int depth; // depth in the tree
  int board[20]; // board configuration (permutation)
};

// check if placing a queen is safe  (i.e., check if all the queens already placed share
// a same diagonal)
bool isSafe(const int* board, const int row, const int col)
{
  for (int i = 0; i < row; ++i) {
    if (board[i] == col - row + i || board[i] == col + row - i) {
      return false;
    }
  }

  return true;
}

// swap function
inline void swap(int* a, int* b)
{
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

// evaluate a given node (i.e., check its board configuration) and branch it if it is valid
// (i.e., generate its child nodes.)
void evaluate_and_branch(const size_t N, const Node parent, std::stack<Node>* pool,
  unsigned long long int* tree_loc, unsigned long long int* num_sol)
{
  const int depth = parent.depth;

  // if the given node is a leaf, then update counter and do nothing
  if (depth == N) {
    *num_sol += 1;
  }
  // if the given node is not a leaf, then update counter and evaluate/branch it
  else {
    for (int j = depth; j < N; j++) {
      if (isSafe(parent.board, depth, parent.board[j])) {
        Node child;
        memcpy(child.board, parent.board, N * sizeof(int));
        swap(&child.board[depth], &child.board[j]);
        child.depth = depth + 1;
        pool->push(child);

        *tree_loc += 1;
      }
    }
  }
}

int main(int argc, char** argv) {
  // helper
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <number of queens> " << std::endl;
    exit(1);
  }

  // problem size (number of queens)
  size_t N = std::stoll(argv[1]);
  std::cout << "Solving " << N << "-Queens problem\n" << std::endl;

  // initialization of the root node (the board configuration where no queen is placed)
  Node root;
  root.depth = 0;
  for (int i = 0; i < N; i++) {
    root.board[i] = i;
  }

  // initialization of the pool of nodes (stack -> DFS exploration order)
  std::stack<Node> pool;
  pool.push(root);

  // statistics to check correctness (number of nodes explored and number of solutions found)
  unsigned long long int exploredTree = 0, exploredSol = 0;

  // beginning of the Depth-First tree-Search
  auto start = std::chrono::high_resolution_clock::now();

  while (pool.size() != 0) {
    // get a node from the pool
    Node currentNode = pool.top();
    pool.pop();

    // check the board configuration of the node and branch it if it is valid.
    evaluate_and_branch(N, currentNode, &pool, &exploredTree, &exploredSol);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

  // outputs
  std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
  std::cout << "Total solutions: " << exploredSol << std::endl;
  std::cout << "Size of the explored tree: " << exploredTree << std::endl;

  return 0;
}
