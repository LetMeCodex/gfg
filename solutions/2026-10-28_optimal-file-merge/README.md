# Optimal File Merge

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the minimum total computation cost to merge a given set of files. The cost of merging two files is equal to the sum of their sizes. When two files are merged, they form a new file whose size is the sum of the two merged files. This process continues until only one file remains. We are given an array of integers representing the sizes of the initial files.

## Intuition & Approach

This problem is a classic example of a problem that can be optimally solved using a greedy approach. The key insight is that to minimize the total cost, we should always merge the two smallest files available at any given step.

Let's consider why this greedy strategy works. Suppose we have file sizes $f_1, f_2, f_3, \dots, f_n$. If we merge two files, say $f_i$ and $f_j$, the cost incurred is $f_i + f_j$. This new file of size $f_i + f_j$ will then be part of future merges. If we choose to merge larger files early on, their large sum will be added to the total cost multiple times as it participates in subsequent merges. By merging the smallest files first, we ensure that the smaller sums are propagated through the merging process, thus minimizing the overall cost.

This strategy naturally leads to the use of a min-priority queue (min-heap). A min-priority queue efficiently allows us to:
1. Store all initial file sizes.
2. Repeatedly extract the two smallest elements (file sizes).
3. Insert the sum of these two elements (the size of the newly merged file) back into the queue.

The algorithm proceeds as follows:
1. Initialize a min-priority queue with all the given file sizes.
2. Initialize a variable `total_cost` to 0.
3. While the priority queue contains more than one element:
    a. Extract the two smallest file sizes from the priority queue (let's call them `file1` and `file2`).
    b. Calculate the cost of merging them: `current_merge_cost = file1 + file2`.
    c. Add `current_merge_cost` to `total_cost`.
    d. Insert `current_merge_cost` back into the priority queue.
4. Once the loop finishes (i.e., only one element remains in the priority queue), `total_cost` will hold the minimum total computation cost.

The problem statement guarantees that the total cost fits within a 32-bit signed integer, which is important for choosing appropriate data types.

## Complexity Analysis

- **Time Complexity**: $O(N \log N)$
    - Initializing the priority queue with $N$ elements takes $O(N)$ time if done efficiently (e.g., using `std::priority_queue` constructor with iterators).
    - The `while` loop runs $N-1$ times because each iteration reduces the number of elements in the priority queue by one.
    - Inside the loop, we perform two `pop` operations and one `push` operation. Each of these operations on a priority queue of size $K$ takes $O(\log K)$ time.
    - In the worst case, the priority queue can hold up to $N$ elements. Therefore, each iteration takes $O(\log N)$ time.
    - The total time complexity is dominated by the loop: $(N-1) \times O(\log N) = O(N \log N)$.

- **Space Complexity**: $O(N)$
    - The priority queue stores up to $N$ file sizes at any given time.
    - Therefore, the space required is proportional to the number of files, which is $O(N)$.

## Solution Code

```cpp
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
```