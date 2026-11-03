# [XOR of All Subarray XORs](https://www.geeksforgeeks.org/problems/game-of-xor1541/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary
Given an array `arr` of size `n`, we need to find the XOR sum of all possible subarray XORs. A subarray is defined as a contiguous part of the array. If the array is `[1, 2, 3]`, the subarrays are `[1], [2], [3], [1, 2], [2, 3], [1, 2, 3]`. We need to calculate the XOR of the XOR sums of all these subarrays.

## Intuition & Approach
To solve this efficiently without generating all subarrays (which would be $O(n^2)$), we analyze how many times each element `arr[i]` contributes to the final XOR sum.

1.  **Contribution Analysis**: An element at index `i` (0-indexed) is part of a subarray starting at index `start` and ending at index `end` if `start <= i <= end`.
    *   The number of possible `start` positions is `(i + 1)`.
    *   The number of possible `end` positions is `(n - i)`.
    *   Therefore, `arr[i]` appears in exactly `(i + 1) * (n - i)` subarrays.
2.  **Parity Logic**: Since we are performing XOR operations, an element contributes to the final result if and only if it appears in an **odd number** of subarrays.
    *   If `(i + 1) * (n - i)` is odd, `arr[i]` is included in the final XOR sum.
    *   If `(i + 1) * (n - i)` is even, `arr[i]` cancels itself out.
3.  **Case Breakdown**:
    *   **If `n` is even**: For any index `i`, either `(i + 1)` or `(n - i)` must be even. Thus, the product is always even. The result is `0`.
    *   **If `n` is odd**: The product `(i + 1) * (n - i)` is odd only when both `(i + 1)` and `(n - i)` are odd. This happens when `i` is even. Thus, we XOR all elements at even indices.

## Complexity Analysis
- **Time Complexity**: $O(n)$, where $n$ is the size of the array, as we iterate through the array once.
- **Space Complexity**: $O(1)$, as we only use a single variable to store the result.

## Solution Code

```cpp
#include <vector>

class Solution {
public:
    /**
     * Calculates the XOR of all subarray XORs.
     * 
     * Logic:
     * An element at index i appears in (i + 1) * (n - i) subarrays.
     * If n is even, every element appears an even number of times -> Result is 0.
     * If n is odd, elements at even indices appear an odd number of times -> XOR them.
     */
    int subarrayXor(std::vector<int>& arr) {
        int n = arr.size();
        
        // If n is even, every element appears an even number of times.
        // XORing an element an even number of times results in 0.
        if (n % 2 == 0) {
            return 0;
        }
        
        // If n is odd, only elements at even indices (0, 2, 4...) 
        // appear an odd number of times.
        int result = 0;
        for (int i = 0; i < n; i += 2) {
            result ^= arr[i];
        }
        
        return result;
    }
};
```