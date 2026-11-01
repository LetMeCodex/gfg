#include <vector>
#include <algorithm> // For std::max

class Solution {
public:
    long long largestSumCycle(std::vector<int>& edge) {
        int N = edge.size(); // Get N from the size of the input vector
        
        // visited_state[i] can be:
        // 0: Not visited yet (UNVISITED)
        // 1: Currently in the recursion stack (VISITING)
        // 2: Fully processed (VISITED)
        std::vector<int> visited_state(N, 0);
        
        // path_indices[node] stores the index of 'node' in the 'path_nodes' vector for the current traversal.
        // This helps to quickly find where a cycle starts within the current path.
        // Initialized to -1, indicating not part of the current path.
        std::vector<int> path_indices(N, -1); 
        
        // path_nodes stores the sequence of nodes visited in the current traversal.
        std::vector<int> path_nodes; 
        
        long long max_cycle_sum = -1; // Initialize with -1 as per problem statement

        for (int i = 0; i < N; ++i) {
            if (visited_state[i] == 0) { // If node 'i' has not been visited yet
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
                for (int node : path_nodes) {
                    if (visited_state[node] == 1) {
                        visited_state[node] = 2;
                    }
                }
            }
        }

        return max_cycle_sum;
    }
};