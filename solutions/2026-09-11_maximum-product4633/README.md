# [Max Product Subsequence of Size K](https://www.geeksforgeeks.org/problems/maximum-product4633/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of integers and an integer `k`, find the maximum product of a subsequence of size `k`.

## Intuition & Approach

The core idea to maximize a product is to select numbers that contribute the most to the magnitude of the product. This generally means picking the largest absolute values. However, the sign of the numbers plays a crucial role.

1.  **Sorting is Key**: To easily identify the largest and smallest (most negative) numbers, sorting the array is the first logical step. After sorting, the smallest numbers will be at the beginning of the array, and the largest numbers will be at the end.

2.  **Handling Negatives**:
    *   **Even `k`**: If `k` is even, we want to pair up numbers to maximize the product. The largest positive numbers will contribute positively. The most negative numbers, when multiplied in pairs, also result in a positive product. Therefore, we should consider picking pairs from both ends of the sorted array.
    *   **Odd `k`**: If `k` is odd, we need to be more careful.
        *   If the largest element in the array is positive, we can pick it to ensure a positive contribution. Then, we are left with an even `k` to find the maximum product from the remaining elements.
        *   If all elements are negative and `k` is odd, the final product *must* be negative. To maximize a negative product, we need to pick the `k` numbers closest to zero (i.e., the `k` largest negative numbers).

3.  **Greedy Strategy**:
    *   **Initialization**: Sort the array `arr`. Initialize `max_prod` to 1. Use two pointers, `left` pointing to the start of the array and `right` pointing to the end.
    *   **Odd `k` Handling**: If `k` is odd, we need to make a decision about the sign.
        *   If the largest element (`arr[n-1]`) is negative and `k` is odd, it means all numbers are negative. In this scenario, to get the largest (least negative) product, we must pick the `k` largest elements (which are the `k` elements closest to zero). So, we iterate from `n-1` down to `n-k` and multiply them.
        *   If the largest element is positive (or zero), we can greedily pick the largest positive element (`arr[right]`) to ensure a positive start. We multiply `max_prod` by `arr[right]`, decrement `right`, and decrement `k`. Now `k` is even.
    *   **Even `k` Pairing**: With `k` now being even, we can iterate while `k > 0`. In each step, we compare the product of the two smallest remaining elements (`arr[left] * arr[left+1]`) with the product of the two largest remaining elements (`arr[right] * arr[right-1]`).
        *   If `arr[left] * arr[left+1]` is greater than `arr[right] * arr[right-1]`, it means picking the two smallest (most negative) numbers yields a larger positive product. We multiply `max_prod` by `arr[left] * arr[left+1]`, and advance `left` by 2.
        *   Otherwise, picking the two largest numbers is better. We multiply `max_prod` by `arr[right] * arr[right-1]`, and decrement `right` by 2.
        *   In both cases, we decrement `k` by 2.

This greedy approach ensures that at each step, we are making the locally optimal choice that leads to the globally maximum product.

## Complexity Analysis

-   **Time Complexity**: $O(N \log N)$
    The dominant operation is sorting the array, which takes $O(N \log N)$ time, where $N$ is the number of elements in the array. The subsequent traversal using two pointers takes $O(K)$ time, which is at most $O(N)$. Therefore, the overall time complexity is $O(N \log N)$.

-   **Space Complexity**: $O(1)$ (or $O(\log N)$ depending on sort implementation)
    The solution uses a constant amount of extra space for variables like `left`, `right`, `max_prod`, and `k`. If the sorting algorithm used is in-place, the space complexity is $O(1)$. Some sorting algorithms might use auxiliary space, typically $O(\log N)$ for recursive calls (e.g., quicksort, mergesort).

## Solution Code

```cpp
#include <vector>
#include <algorithm>

class Solution {
public:
    long long maxProduct(std::vector<int>& arr, int k) {
        int n = arr.size();
        // Sort the array to easily access smallest and largest elements.
        std::sort(arr.begin(), arr.end());

        // Special case: If all numbers are negative and k is odd,
        // the maximum product will be negative. To maximize a negative product,
        // we need to pick the k largest elements (closest to 0).
        if (arr[n - 1] < 0 && k % 2 != 0) {
            long long prod = 1;
            // Pick the k largest elements (which are the k elements closest to 0).
            for (int i = 0; i < k; ++i) {
                prod *= arr[n - 1 - i];
            }
            return prod;
        }

        // General case:
        long long max_prod = 1;
        int left = 0;
        int right = n - 1;

        // If k is odd, we need to ensure the final product is as large as possible.
        // If the largest element is positive, pick it to make k even and ensure a positive start.
        // If the largest element is negative (and we've already handled the all-negative odd k case),
        // this branch won't be taken because arr[n-1] would be positive.
        if (k % 2 != 0) {
            max_prod *= arr[right];
            right--; // Move the right pointer inwards
            k--;     // Decrement k as we've picked one element
        }

        // Now k is guaranteed to be even. We pick pairs from either end.
        // We compare the product of two smallest (most negative) elements
        // with the product of two largest (most positive) elements.
        while (k > 0) {
            // Product of the two smallest remaining elements
            long long left_prod = (long long)arr[left] * arr[left + 1];
            // Product of the two largest remaining elements
            long long right_prod = (long long)arr[right] * arr[right - 1];

            // If the product of the two smallest (most negative) is greater,
            // it means they contribute more positively to the overall product.
            if (left_prod > right_prod) {
                max_prod *= left_prod;
                left += 2; // Move left pointer past the two elements picked
            } else {
                // Otherwise, the product of the two largest is greater.
                max_prod *= right_prod;
                right -= 2; // Move right pointer past the two elements picked
            }
            k -= 2; // We picked a pair of elements
        }

        return max_prod;
    }
};
```