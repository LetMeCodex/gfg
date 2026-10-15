#include <vector>
#include <algorithm> // Required for std::max and std::min

class Solution {
public:
    // Renamed from successfulBinarySearches to countAlwaysFound to match the driver code.
    int countAlwaysFound(std::vector<int>& arr) {
        int n = arr.size();
        
        // Handle empty array case
        if (n == 0) {
            return 0;
        }

        // prefixMax[i] stores the maximum element in arr[0...i]
        // This helps to find max(arr[0]...arr[i-1]) efficiently.
        std::vector<int> prefixMax(n);
        prefixMax[0] = arr[0];
        for (int i = 1; i < n; ++i) {
            prefixMax[i] = std::max(prefixMax[i-1], arr[i]);
        }

        // suffixMin[i] stores the minimum element in arr[i...n-1]
        // This helps to find min(arr[i+1]...arr[n-1]) efficiently.
        std::vector<int> suffixMin(n);
        suffixMin[n-1] = arr[n-1];
        for (int i = n - 2; i >= 0; --i) {
            suffixMin[i] = std::min(suffixMin[i+1], arr[i]);
        }

        int count = 0;
        // Iterate through each index i to check if arr[i] is a "successful" target
        for (int i = 0; i < n; ++i) {
            // leftMax represents max(arr[0]...arr[i-1])
            // If i is 0, there are no elements to its left. We use 0 as a sentinel
            // because all arr[k] >= 1 (from constraints), so arr[i] will always be greater than 0.
            // If negative numbers were possible, we'd need a sentinel like INT_MIN.
            int leftMax = (i == 0) ? 0 : prefixMax[i-1];

            // rightMin represents min(arr[i+1]...arr[n-1])
            // If i is n-1, there are no elements to its right. We use 1000001 as a sentinel
            // because all arr[k] <= 10^6 (from constraints), so arr[i] will always be less than 1000001.
            // If numbers larger than 10^6 were possible, we'd need a sentinel like INT_MAX.
            int rightMin = (i == n - 1) ? 1000001 : suffixMin[i+1]; 

            // An element arr[i] is always found if it's strictly greater than all elements to its left
            // and strictly smaller than all elements to its right.
            // This ensures that no matter which pivot is chosen, the search range will always
            // contain arr[i] until it's found.
            if (leftMax < arr[i] && arr[i] < rightMin) {
                count++;
            }
        }

        return count;
    }
};