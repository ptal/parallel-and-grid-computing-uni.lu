#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <random>

std::mutex mtx;
std::atomic<bool> stop{false}; 
void apply_rule_184_part(const std::vector<int> &current, std::vector<int> &next, size_t start,
                         size_t end, size_t n)
{
    for (size_t i = start; i < end; ++i)
    {
        int curr = current[i];
        int right = current[(i + 1) % n];
        if (curr == 1 && right == 0)
        {
            next[i] = 0;
            next[(i + 1) % n] = 1;
        }
        else
        {
            next[i] = curr;
        }
    }
}

size_t longest_queue(const std::vector<int> &cells)
{
    size_t max_queue = 0, current_queue = 0;
    for (int cell : cells)
    {
        if (cell == 1)
        {
            current_queue++;
            max_queue = std::max(max_queue, current_queue);
        }
        else
        {
            current_queue = 0;
        }
    }
    return max_queue;
}

void worker(std::vector<int> &current, std::vector<int> &next, size_t start, size_t end, size_t n,
            std::atomic<size_t> &iterations, std::atomic<size_t> &longest)
{
    while (!stop)
    {
        apply_rule_184_part(current, next, start, end, n);
        {
            std::unique_lock<std::mutex> lock(mtx);
            longest = std::max(longest.load(), longest_queue(current));
        }
        current = next;
        
        iterations.fetch_add(1);
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " <vector size> <num_threads>" << std::endl;
        exit(1);
    }
    size_t size = std::stoi(argv[1]);
    size_t num_threads = std::stoi(argv[2]);
    std::atomic<size_t> iterations(0);
    std::atomic<size_t> longest(0);
    // Initialize the current and next states
    std::vector<int> current(size, 0);
    std::vector<int> next(size, 0);
    std::mt19937 m{0};
    std::uniform_int_distribution<int> u{0, 1};
    for (size_t i = 0; i < size; ++i)
    {
        current[i] = u(m); // Randomly initialize cells
    }
    std::vector<std::thread> threads;
    size_t chunk_size = size / num_threads;
    // Start the threads
    for (size_t i = 0; i < num_threads; ++i)
    {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        threads.emplace_back(worker, std::ref(current), std::ref(next), start, end, size,
                             std::ref(iterations), std::ref(longest));
    }
    // Run the simulation for 1 minute
    auto start_time = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(60)); // Simulation duration
    stop = true;                                           // Set the stop flag
    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }
    // Print the final state and the longest queue
    std::cout << "Final state: ";
    for (int i = 0; i < 50; ++i)
    { // Print first 50 cells for visualization
        std::cout << current[i];
    }
    std::cout << std::endl;
    std::cout << "Iterations completed: " << iterations.load() << std::endl;
    std::cout << "Longest queue: " << longest.load() << std::endl;
    return 0;
}