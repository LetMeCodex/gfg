# [Pyramid Array with Reduce Operations](https://www.geeksforgeeks.org/problems/pyramid-form3044/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary
Given an array of $N$ integers, we want to transform it into a "pyramid" shape. A pyramid shape is defined as a sequence where elements increase by 1 until a peak and then decrease by 1, ending at 1 on both sides (e.g., `1, 2, 3, 2, 1`). We are allowed to reduce any element in the array to a smaller value. The goal is to find the minimum total reduction required to make the array a pyramid.

## Intuition & Approach
To minimize the reduction, we must maximize the sum of the elements in the resulting pyramid. If the peak of the pyramid is at index $i$ with height $x$, the pyramid elements are $1, 2, \dots, x, \dots, 2, 1$. The sum of these elements is $x^2$.

1. **Constraint Analysis**: For a peak of height $x$ at index $i$, the element at $i-k$ must be at least $x-k$, and the element at $i+k$ must be at least $x-k$.
2. **Dynamic Programming**:
   - Let `L[i]` be the maximum height of a non-decreasing sequence ending at index $i$. This is calculated as `L[i] = min(arr[i], L[i-1] + 1)`.
   - Let `R[i]` be the maximum height of a non-increasing sequence starting at index $i$. This is calculated as `R[i] = min(arr[i], R[i+1] + 1)`.
3. **Peak Calculation**: For any index $i$, the maximum possible height of a pyramid centered at $i$ is `min(L[i], R[i])`.
4. **Result**: The maximum sum of a pyramid we can form is `max(height[i]^2)` for all $i$. The minimum reduction is `Total Sum of Array - Max Pyramid Sum`.

## Complexity Analysis
- **Time Complexity**: $O(N)$, where $N$ is the size of the array. We perform three linear passes over the array: one to calculate `L`, one for `R`, and one to find the maximum pyramid sum.
- **Space Complexity**: $O(N)$ to store the `L` and `R` arrays.

## Solution Code

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /**
     * The cost to form a pyramid with peak at i and height x is:
     * Total sum of arr - (sum of pyramid elements).
     * The pyramid elements are 1, 2, ..., x, ..., 1, which sum to x^2.
     * We need to find the max x for each i such that arr[i-k] >= x-k and arr[i+k] >= x-k.
     */
    long long formPyramid(vector<int>& arr) {
        int n = arr.size();
        long long total_sum = 0;
        for (int x : arr) total_sum += x;

        // Precompute L[i]: max height of a non-decreasing sequence ending at i
        // L[i] = min(arr[i], L[i-1] + 1)
        vector<int> L(n);
        L[0] = 1;
        for (int i = 1; i < n; ++i) {
            L[i] = min(arr[i], L[i - 1] + 1);
        }

        // Precompute R[i]: max height of a non-increasing sequence starting at i
        // R[i] = min(arr[i], R[i+1] + 1)
        vector<int> R(n);
        R[n - 1] = 1;
        for (int i = n - 2; i >= 0; --i) {
            R[i] = min(arr[i], R[i + 1] + 1);
        }

        // The max height x at peak i is min(L[i], R[i])
        long long max_pyramid_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long x = min(L[i], R[i]);
            max_pyramid_sum = max(max_pyramid_sum, x * x);
        }

        return total_sum - max_pyramid_sum;
    }
};
```