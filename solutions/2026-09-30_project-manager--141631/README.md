# [Minimum Time to Finish Project](https://www.geeksforgeeks.org/problems/project-manager--141631/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the minimum time required to complete a project consisting of several modules. Each module has a specific duration. There are also dependencies between modules, meaning some modules cannot start until others are completed. We are given the durations of each module and a list of dependencies, where `dependencies[i] = [u, v]` signifies that module `u` must be completed before module `v` can start. If there's a cycle in the dependencies, it's impossible to complete the project, and we should return -1.

## Intuition & Approach

This problem can be modeled as finding the longest path in a Directed Acyclic Graph (DAG). The modules represent the nodes of the graph, and the dependencies represent the directed edges. The "weight" of traversing an edge from module `u` to module `v` is the duration of module `v`.

The core idea is to determine the earliest possible completion time for each module. A module can only start after all its prerequisite modules are finished. Therefore, the completion time of a module `v` depends on the completion times of all its predecessors. Specifically, `v` can start only after the latest completion time among all its direct prerequisites. Once it starts, it takes `duration[v]` time to complete.

This problem structure strongly suggests a **Topological Sort** approach. Topological sort is an algorithm for ordering the vertices of a DAG such that for every directed edge from vertex `u` to vertex `v`, `u` comes before `v` in the ordering.

Here's the step-by-step approach:

1.  **Represent the Dependencies as a Graph**:
    *   We can use an adjacency list (`adj`) to represent the graph, where `adj[u]` stores a list of modules that depend on `u` (i.e., `v` such that there's a dependency `u -> v`).
    *   We also need to keep track of the **in-degree** of each node. The in-degree of a node `v` is the number of incoming edges, which corresponds to the number of prerequisite modules for `v`.

2.  **Initialize Data Structures**:
    *   `adj`: A `vector<vector<int>>` to store the graph.
    *   `in_degree`: A `vector<int>` to store the in-degree of each module.
    *   `completion_time`: A `vector<long long>` to store the earliest possible completion time for each module. Initialize all to 0.

3.  **Build the Graph and Calculate In-Degrees**:
    *   Iterate through the `dependencies` list. For each dependency `[u, v]`:
        *   Add an edge from `u` to `v` in the adjacency list: `adj[u].push_back(v)`.
        *   Increment the in-degree of `v`: `in_degree[v]++`.

4.  **Initialize the Queue for Topological Sort**:
    *   Create a queue (`q`) to store modules that are ready to be processed (i.e., have an in-degree of 0).
    *   Iterate through all modules from 0 to `n-1`. If `in_degree[i] == 0`:
        *   Push module `i` into the queue.
        *   Set its initial `completion_time[i]` to `duration[i]`, as these modules have no prerequisites and can start immediately.

5.  **Perform Topological Sort and Calculate Completion Times**:
    *   While the queue is not empty:
        *   Dequeue a module `u`.
        *   Increment a counter (`count`) to track the number of processed modules. This is crucial for cycle detection.
        *   For each neighbor `v` of `u` (i.e., for each module `v` that depends on `u`):
            *   **Update `completion_time[v]`**: The earliest `v` can be completed is by considering the path through `u`. This time would be `completion_time[u] + duration[v]`. Since `v` might have multiple prerequisites, we need to take the maximum of all such potential completion times to ensure `v` only starts after *all* its prerequisites are met. So, `completion_time[v] = max(completion_time[v], completion_time[u] + duration[v])`.
            *   Decrement the in-degree of `v`: `in_degree[v]--`.
            *   If `in_degree[v]` becomes 0, it means all prerequisites for `v` are now met, so enqueue `v`.

6.  **Cycle Detection**:
    *   After the loop finishes, if `count` is not equal to `n` (the total number of modules), it means some modules were never enqueued because their in-degrees never reached zero. This indicates a cycle in the dependency graph. In this case, return -1.

7.  **Find the Minimum Project Completion Time**:
    *   If no cycle is detected, the minimum time to complete the entire project is the maximum value in the `completion_time` array. This is because the project is only finished when the last module is completed. Iterate through `completion_time` and find the maximum value.

This approach correctly handles the dependencies and ensures that we calculate the earliest possible completion time for each module, ultimately leading to the minimum project completion time. The use of `long long` for `completion_time` is important to avoid potential integer overflow, as durations can be large and summed up.

## Complexity Analysis

-   **Time Complexity**: $O(V + E)$, where $V$ is the number of modules (vertices) and $E$ is the number of dependencies (edges).
    *   Building the graph and calculating in-degrees takes $O(E)$ time.
    *   Initializing the queue takes $O(V)$ time.
    *   The topological sort (while loop) processes each vertex and each edge exactly once. Each vertex is enqueued and dequeued at most once. For each vertex, we iterate through its neighbors. Thus, the total time for the topological sort is $O(V + E)$.
    *   Finding the maximum completion time takes $O(V)$ time.
    *   Therefore, the overall time complexity is $O(V + E)$.

-   **Space Complexity**: $O(V + E)$.
    *   The adjacency list `adj` stores $V$ lists, and the total number of elements across all lists is $E$. So, $O(V + E)$ for `adj`.
    *   The `in_degree` vector takes $O(V)$ space.
    *   The `completion_time` vector takes $O(V)$ space.
    *   The queue `q` can store up to $V$ elements in the worst case, taking $O(V)$ space.
    *   Therefore, the overall space complexity is $O(V + E)$.

## Solution Code

```cpp
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minTime(vector<int>& duration, vector<vector<int>>& dependencies) {
        int n = duration.size();
        // adj[u] stores a list of modules v that depend on u (u -> v)
        vector<vector<int>> adj(n);
        // in_degree[v] stores the number of prerequisites for module v
        vector<int> in_degree(n, 0);

        // Build the graph and calculate in-degrees
        for (const auto& dep : dependencies) {
            int u = dep[0]; // prerequisite module
            int v = dep[1]; // dependent module
            adj[u].push_back(v);
            in_degree[v]++;
        }

        queue<int> q;
        // completion_time[i] stores the maximum time taken to complete module i,
        // considering all its prerequisite modules and their durations.
        // This represents the earliest possible finish time for module i.
        vector<long long> completion_time(n, 0);

        // Initialize the queue with modules that have no dependencies (in-degree 0)
        for (int i = 0; i < n; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
                // For modules with no dependencies, their completion time is just their own duration.
                completion_time[i] = duration[i];
            }
        }

        int count = 0; // To count the number of modules processed (for cycle detection)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            count++; // Increment count for each processed module

            // For each module v that depends on u
            for (int v : adj[u]) {
                // The time to complete module v, if it were to start immediately after module u finishes,
                // would be completion_time[u] (when u finishes) + duration[v] (time for v).
                // We need to take the maximum of all such paths leading to v,
                // because v can only start after ALL its prerequisites are done.
                // completion_time[v] is updated to be the maximum of its current value
                // and the time it would take if it starts after u.
                completion_time[v] = max(completion_time[v], completion_time[u] + duration[v]);
                
                // Decrement the in-degree of v as one of its prerequisites (u) is now processed.
                in_degree[v]--;
                // If in-degree of v becomes 0, it means all its prerequisites are met,
                // so it can now be added to the queue for processing.
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        // If count is not equal to n, it means not all modules could be processed.
        // This happens if there's a cycle in the dependency graph, as modules within
        // a cycle will never have their in-degrees reduced to zero.
        if (count != n) {
            return -1; // Cycle detected, impossible to complete the project.
        }

        // The minimum time to complete the entire project is the maximum
        // completion time among all modules. The project is finished
        // only when the last module is completed.
        long long max_completion_time = 0;
        for (long long time : completion_time) {
            max_completion_time = max(max_completion_time, time);
        }

        return max_completion_time;
    }
};
```