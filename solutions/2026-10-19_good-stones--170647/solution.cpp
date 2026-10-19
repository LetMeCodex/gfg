#include <vector> // Required for std::vector

class Solution {
private:
    // Member variables to store the input array, its size, and the DP states.
    // Using member variables avoids passing them through recursive calls,
    // making the code cleaner for competitive programming.
    std::vector<int> arr_global; 
    std::vector<int> dp;         
    int N;                        

    // States for dp array:
    // 0: UNKNOWN (not yet visited or processed)
    // 1: GOOD (eventually escapes the array)
    // 2: BAD (eventually enters an infinite loop)
    // 3: VISITING (currently in the recursion stack for the current path,
    //              used to detect cycles)

    // Helper function to determine the state of an index.
    // Returns 1 if GOOD, 2 if BAD.
    int solve(int current_idx) {
        // Base case 1: If current_idx is outside the array bounds, it means we escaped.
        if (current_idx < 0 || current_idx >= N) {
            return 1; // GOOD (escaped)
        }

        // Base case 2: Check memoized state to avoid redundant computations.
        if (dp[current_idx] == 1) { // Already known to be GOOD
            return 1;
        }
        if (dp[current_idx] == 2) { // Already known to be BAD
            return 2;
        }
        
        // Base case 3: If dp[current_idx] is 3 (VISITING), it means we have
        // encountered this index again in the current path. This indicates a cycle.
        // Any index leading to a cycle is considered BAD.
        if (dp[current_idx] == 3) { 
            return 2; // BAD (cycle detected)
        }

        // Mark current_idx as VISITING to detect cycles in the current path.
        dp[current_idx] = 3;

        // Calculate the next index based on the jump value.
        int next_idx = current_idx + arr_global[current_idx];

        // Recursively determine the state of the next index.
        int result = solve(next_idx);

        // Memoize the result for current_idx.
        // Once the recursive call returns, current_idx is no longer VISITING
        // for this specific path, and its final state (GOOD or BAD) is determined.
        dp[current_idx] = result;

        return result;
    }

public:
    // Main function to count good indices.
    // Renamed from countGoodIndices to goodIndices to match the GFG driver code.
    int goodIndices(std::vector<int>& arr) {
        // Copy the input array and its size to member variables for easy access
        // within the helper function. This copy happens once per test case.
        arr_global = arr; 
        N = arr.size();

        // Initialize the dp array with 0 (UNKNOWN) for all indices.
        dp.assign(N, 0);

        int good_indices_count = 0;

        // Iterate through all indices to determine their state.
        for (int i = 0; i < N; ++i) {
            // If the state of index 'i' is UNKNOWN, compute it using the solve function.
            // If it's already known (1 or 2), we don't need to recompute.
            if (dp[i] == 0) { 
                solve(i);
            }
            
            // After solve(i) (or if it was already computed), check its final state.
            // If dp[i] is 1, it means index 'i' is GOOD.
            if (dp[i] == 1) {
                good_indices_count++;
            }
        }

        return good_indices_count;
    }
};