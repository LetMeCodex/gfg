# [Largest Sum Cycle](https://www.geeksforgreeks.org/problems/largest-sum-cycle--170645/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 1 attempt(s)

## Problem Summary

Given a directed graph represented by an `edge` array of size `N`, where `edge[i]` denotes a directed edge from node `i` to `edge[i]`. If `edge[i]` is -1, it means there is no outgoing edge from node `i`. The value of each node is its index (0 to N-1). The task is to find the largest sum of node values in any cycle present in this graph. If no cycle exists, return -1.

**Key characteristics of the graph:**
*   It's a directed graph.
*   Each node has at most one outgoing edge (this is a functional graph).
*   Node values are their indices.

## Intuition & Approach

### Intuition

The problem asks for the maximum sum of node values within any cycle in a directed graph. The graph has a special property: each node has at most one outgoing edge. Such graphs are often called functional graphs. In a functional graph, starting from any node and following the edges will always lead to either a dead end (a node pointing to -1) or eventually enter a cycle.

To find the largest sum cycle, we need to:
1.  Identify all cycles in the graph.
2.  For each cycle, calculate the sum of its node values.
3.  Keep track of the maximum sum found.

A standard approach for cycle detection in directed graphs is Depth First Search (DFS) using three states for nodes: unvisited, visiting (currently in the recursion stack), and visited (fully processed).

### Approach (DFS with 3 States)

We can iterate through each node and, if it hasn't been fully processed, start a traversal (similar to DFS). During this traversal, we maintain the state of nodes to detect cycles.

1.  **Node States:** We use a `visited_state` array of size `N` to keep track of each node's status:
    *   `0` (UNVISITED): The node has not been encountered yet.
    *   `1` (VISITING): The node is currently part of the path being explored in the current traversal (i.e., it's in the current recursion stack). If we encounter a `VISITING` node again, we've found a cycle.
    *   `2` (VISITED): The node has been fully processed. This means all paths originating from it have been explored, or it's part of a cycle that has already been processed and its sum considered.

2.  **Path Tracking:** To identify the nodes forming a cycle and calculate their sum efficiently, we need two additional data structures for the *current* traversal:
    *   `path_nodes`: A `std::vector<int>` that stores the sequence of nodes encountered in the current path.
    *   `path_indices`: A `std::vector<int>` of size `N`, where `path_indices[node]` stores the index of `node` within the `path_nodes` vector. This allows us to quickly determine where a cycle starts within `path_nodes`.

3.  **Algorithm Steps:**
    *   Initialize `max_cycle_sum = -1`.
    *   Initialize `visited_state` with all `0`s (UNVISITED).
    *   Initialize `path_indices` with all `-1`s.
    *   Iterate `i` from `0` to `N-1`:
        *   If `visited_state[i]` is `0` (UNVISITED), start a new traversal from `i`:
            *   Clear `path_nodes` for the new traversal.
            *   Set `curr = i` and `path_len = 0`.
            *   **Traverse the path:** While `curr` is a valid node (`curr != -1`) AND `visited_state[curr]` is `0` (UNVISITED):
                *   Mark `curr` as `VISITING` (`visited_state[curr] = 1`).
                *   Store its position in the current path: `path_indices[curr] = path_len++`.
                *   Add `curr` to `path_nodes`.
                *   Move to the next node: `curr = edge[curr]`.
            *   **After the loop, check termination condition:**
                *   **If `curr != -1` AND `visited_state[curr] == 1`:** A cycle is detected! `curr` is the node where the cycle starts.
                    *   The cycle consists of nodes from `path_nodes[path_indices[curr]]` to the end of `path_nodes`.
                    *   Calculate `current_cycle_sum` by summing these nodes.
                    *   Update `max_cycle_sum = std::max(max_cycle_sum, current_cycle_sum)`.
                    *   **Crucially:** Mark all nodes *within this detected cycle* as `VISITED` (`visited_state[node] = 2`). This prevents re-processing them and ensures they are not considered part of other paths later.
                *   **If `curr == -1` (dead end) or `visited_state[curr] == 2` (reached an already processed node):** No new cycle is found in this specific branch.
            *   **Clean up `VISITING` nodes:** After the traversal and potential cycle processing, any node in `path_nodes` that is still marked `VISITING` (`visited_state[node] == 1`) means it was part of the current path but *not* part of a cycle found in this specific traversal. These nodes should now be marked as `VISITED` (`visited_state[node] = 2`) to avoid redundant processing by other starting nodes.

    *   Finally, return `max_cycle_sum`.

This approach ensures that each node and edge is processed a constant number of times, leading to an efficient solution.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   Each node `i` is processed at most a constant number of times:
        *   Its `visited_state` changes from `0` to `1` at most once.
        *   Its `visited_state` changes from `1` to `2` at most once.
        *   The `while` loop traverses edges. Since each node has at most one outgoing edge, each edge is traversed at most once across all DFS calls.
        *   Calculating the sum of a cycle takes time proportional to the number of nodes in that cycle. Since each node belongs to at most one cycle (or no cycle), the total sum calculation across all cycles is also $O(N)$.
    *   Therefore, the total time complexity is $O(N)$.

*   **Space Complexity**: $O(N)$
    *   `visited_state`: `O(N)` to store the state of each node.
    *   `path_indices`: `O(N)` to store the index of nodes in the current path.
    *   `path_nodes`: In the worst case, a path can include all `N` nodes (e.g., a long chain leading to a cycle, or a single large cycle). So, `O(N)` space for the path.
    *   The recursion stack depth for DFS would also be `O(N)` in the worst case (a long chain).
    *   Overall, the space complexity is dominated by these `O(N)` auxiliary data structures.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // For std::max

class Solution {
public:
    long long largestSumCycle(std::vector<int>& edge) {
        int N = edge.size(); // Get N from the size of the input vector
        
        // visited_state[i] can be:
        // 0: Not visited yet (UNVISITED)
        // 1: Currently in the recursion stack / part of current path (VISITING)
        // 2: Fully processed (VISITED) - either part of a found cycle or a path leading to a dead end/already visited node.
        std::vector<int> visited_state(N, 0);
        
        // path_indices[node] stores the index of 'node' in the 'path_nodes' vector for the current traversal.
        // This helps to quickly find where a cycle starts within the current path.
        // Initialized to -1, indicating not part of the current path (or not yet encountered in current path).
        std::vector<int> path_indices(N, -1); 
        
        // path_nodes stores the sequence of nodes visited in the current traversal.
        std::vector<int> path_nodes; 
        
        long long max_cycle_sum = -1; // Initialize with -1 as per problem statement

        for (int i = 0; i < N; ++i) {
            if (visited_state[i] == 0) { // If node 'i' has not been visited yet, start a new traversal
                path_nodes.clear(); // Clear path for a new traversal
                
                int curr = i;
                int path_len = 0; // To track current path length for path_indices
                
                // Traverse the path until we hit a node that is:
                // 1. A dead end (-1)
                // 2. Already being visited in the current path (state 1 -> cycle detected)
                // 3. Already fully processed (state 2)
                while (curr != -1 && visited_state[curr] == 0) {
                    visited_state[curr] = 1; // Mark as currently visiting
                    path_indices[curr] = path_len++; // Store its index in the current path
                    path_nodes.push_back(curr);
                    curr = edge[curr];
                }

                // After the loop, check why it terminated:
                if (curr != -1 && visited_state[curr] == 1) {
                    // Case: Cycle detected! 'curr' is the node where the cycle starts.
                    // The cycle consists of nodes from path_indices[curr] to the end of path_nodes.
                    long long current_cycle_sum = 0;
                    int cycle_start_idx_in_path = path_indices[curr];
                    
                    for (int j = cycle_start_idx_in_path; j < path_nodes.size(); ++j) {
                        int node_in_cycle = path_nodes[j];
                        current_cycle_sum += node_in_cycle;
                        // Mark nodes in the cycle as fully processed (state 2)
                        // This prevents re-processing them and ensures they are not considered part of other paths later.
                        visited_state[node_in_cycle] = 2;
                    }
                    max_cycle_sum = std::max(max_cycle_sum, current_cycle_sum);
                }
                
                // After processing the path (either found a cycle or reached a dead end/already visited node),
                // mark all nodes in 'path_nodes' that are still 'VISITING' (state 1) as 'VISITED' (state 2).
                // These are nodes that led to a dead end or an already processed node, but are not part of a cycle
                // found in this specific path, or were part of a path leading to a cycle but not the cycle itself.
                // This ensures that these nodes are not re-processed by other starting nodes, optimizing performance.
                for (int node : path_nodes) {
                    if (visited_state[node] == 1) { // Only mark if still in 'VISITING' state
                        visited_state[node] = 2;
                    }
                }
            }
        }

        return max_cycle_sum;
    }
};
```