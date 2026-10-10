#include <vector> // Required for std::vector

class Solution {
public:
    // Renamed the method from 'sortTwoPartsSorted' to 'mergeTwoParts'
    // to match the name expected by the GeeksforGeeks driver code.
    void mergeTwoParts(std::vector<int>& arr) {
        int n = arr.size();

        // Handle base cases: An array with 0 or 1 element is already sorted.
        if (n <= 1) {
            return;
        }

        // Step 1: Find the breakpoint.
        // The breakpoint is the index where the second sorted part begins.
        // This is the first index 'i' such that arr[i-1] > arr[i].
        // If the array is already fully sorted, no such 'i' will be found,
        // and breakpoint_idx will remain -1.
        int breakpoint_idx = -1;
        for (int i = 0; i < n - 1; ++i) {
            if (arr[i] > arr[i+1]) {
                breakpoint_idx = i + 1;
                break; // Found the breakpoint, exit loop
            }
        }

        // Step 2: If no breakpoint was found, the array is already sorted.
        // In this case, we don't need to perform any merge operation.
        if (breakpoint_idx == -1) {
            return;
        }

        // Step 3: Merge the two sorted parts.
        // Part 1: arr[0 ... breakpoint_idx - 1]
        // Part 2: arr[breakpoint_idx ... n - 1]
        
        // Create a temporary array to store the merged result.
        // This uses O(N) auxiliary space.
        std::vector<int> temp_arr(n);
        
        // Pointers for the two parts and the temporary array.
        int p1 = 0; // Pointer for the first part (starts at index 0)
        int p2 = breakpoint_idx; // Pointer for the second part (starts at breakpoint_idx)
        int k = 0; // Pointer for the temporary array (starts at index 0)

        // Merge elements from both parts into temp_arr until one part is exhausted.
        while (p1 < breakpoint_idx && p2 < n) {
            if (arr[p1] <= arr[p2]) {
                temp_arr[k++] = arr[p1++];
            } else {
                temp_arr[k++] = arr[p2++];
            }
        }

        // Copy any remaining elements from the first part (if any).
        // This loop will run only if p1 has not reached breakpoint_idx,
        // meaning p2's part was exhausted first.
        while (p1 < breakpoint_idx) {
            temp_arr[k++] = arr[p1++];
        }

        // Copy any remaining elements from the second part (if any).
        // This loop will run only if p2 has not reached n,
        // meaning p1's part was exhausted first.
        while (p2 < n) {
            temp_arr[k++] = arr[p2++];
        }

        // Step 4: Copy the sorted elements from temp_arr back to the original arr.
        for (int i = 0; i < n; ++i) {
            arr[i] = temp_arr[i];
        }
    }
};