#include <vector>
#include <queue>
#include <functional> // Required for std::greater

class Solution {
public:
    // Function to find the minimum total computation cost to merge all files.
    // The input is a vector of integers representing file sizes.
    // The problem guarantees that the total cost fits within a 32-bit signed integer.
    // Renamed from 'minCost' to 'minComputation' to match the GeeksforGeeks driver code.
    int minComputation(std::vector<int>& files) {
        // Create a min-priority queue and initialize it with all file sizes.
        // std::priority_queue is a max-heap by default.
        // To make it a min-heap, we specify std::greater<int> as the comparator.
        // Initializing with iterators (files.begin(), files.end()) builds the heap in O(N) time.
        std::priority_queue<int, std::vector<int>, std::greater<int>> pq(files.begin(), files.end());

        // Variable to store the total minimum cost.
        // As per problem guarantee, it fits in a 32-bit signed integer.
        int total_cost = 0;

        // Continue merging until only one file remains in the priority queue.
        // Each iteration merges two files into one, effectively reducing the count of files by one.
        // Thus, for N initial files, this loop runs N-1 times.
        while (pq.size() > 1) {
            // Extract the two smallest files from the priority queue.
            // Each pop operation takes O(log K) time, where K is the current number of elements in PQ.
            int file1 = pq.top();
            pq.pop();
            int file2 = pq.top();
            pq.pop();

            // The cost of merging these two files is their sum.
            // This sum also represents the size of the new merged file.
            // The maximum value of file1 or file2 can be the sum of all initial files (10^5 * 10^3 = 10^8).
            // So, current_merge_cost can be up to 2 * 10^8, which fits within a 32-bit signed int.
            int current_merge_cost = file1 + file2;

            // Add this merge cost to the total cost.
            // The total_cost can be up to ~1.7 * 10^9, which fits within a 32-bit signed int.
            total_cost += current_merge_cost;

            // Insert the newly merged file (with its size as current_merge_cost)
            // back into the priority queue for further merges.
            // This push operation takes O(log K) time.
            pq.push(current_merge_cost);
        }

        // Return the accumulated minimum total cost.
        return total_cost;
    }
};