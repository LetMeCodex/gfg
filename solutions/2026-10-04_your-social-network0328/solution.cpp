#include <vector>
#include <algorithm> // Required for std::reverse

class Solution {
public:
    // The problem statement implies the method name should be 'solve' based on the provided code.
    // However, the driver code in the diagnostic shows it expects 'socialNetwork'.
    // Renaming the method to 'socialNetwork' to match the driver.
    std::vector<std::vector<int>> socialNetwork(std::vector<int>& arr) {
        // N is the total number of users.
        // arr.size() is N-1, as it contains friends for users 2 to N.
        int N = arr.size() + 1;

        // parent[u] will store the friend of user u.
        // We use 1-based indexing for users, so the parent array size is N + 1.
        // parent[1] is set to 0 to indicate that user 1 has no friend and acts as a stopping point.
        std::vector<int> parent(N + 1);
        parent[1] = 0; 

        // Populate the parent array based on the input arr.
        // arr[idx] is the friend of user (idx + 2).
        // For example, arr[0] is friend of user 2, arr[1] is friend of user 3, etc.
        for (int idx = 0; idx < arr.size(); ++idx) {
            parent[idx + 2] = arr[idx];
        }

        std::vector<std::vector<int>> result;

        // Iterate through users 'i' from 2 to N as the starting user.
        for (int i = 2; i <= N; ++i) {
            // This temporary vector will store all reachable pairs [i, j, k] for the current 'i'.
            std::vector<std::vector<int>> current_i_reachables;
            
            int current_user = i;
            int links_followed = 0;

            // Traverse up the friend links until we reach user 1's "parent" (0).
            while (current_user != 0) {
                // Move to the friend of the current user.
                current_user = parent[current_user];
                links_followed++;

                // If the reachable user 'j' (current_user) is valid (1 <= j) and
                // has a smaller user number than 'i' (j < i), add it to our list.
                if (current_user >= 1 && current_user < i) {
                    current_i_reachables.push_back({i, current_user, links_followed});
                }
                // If current_user becomes 0, it means we've gone past user 1,
                // and the loop condition `current_user != 0` will terminate the traversal.
            }
            
            // The reachable users 'j' are collected in decreasing order during the traversal
            // (e.g., for i=3, we find 2 then 1).
            // The problem requires 'j' to be in increasing order.
            // Reversing the collected list achieves this.
            std::reverse(current_i_reachables.begin(), current_i_reachables.end());
            
            // Append all reachable pairs for user 'i' (now correctly ordered by 'j')
            // to the final result vector.
            for (const auto& entry : current_i_reachables) {
                result.push_back(entry);
            }
        }

        return result;
    }
};