Here's a high-quality GitHub solution writeup for the "Max Equal Sum in 3 Stacks" problem:

---

# [Max Equal Sum in 3 Stacks](https://www.geeksforgeeks.org/problems/find-maximum-equal-sum-of-three-stacks/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 1 attempt(s)

## Problem Summary

Given three stacks, `s1`, `s2`, and `s3`, each containing positive integers. We are allowed to remove elements only from the top of any stack. The goal is to find the maximum possible sum `S` such that all three stacks can be made to have a total sum of `S` by removing zero or more elements from their tops. If it's impossible to achieve a positive equal sum, return 0.

**Example:**
`s1 = [4, 7]`
`s2 = [10]`
`s3 = [1, 2, 3]`

Initial sums: `sum1 = 11`, `sum2 = 10`, `sum3 = 6`.
1. `sum1` is largest. Remove 4 from `s1`. `s1 = [7]`. `sum1 = 7`.
2. `sum2` is largest. Remove 10 from `s2`. `s2 = []`. `sum2 = 0`.
3. Now `s2` is empty. It's impossible to make sums equal to a positive value. Return 0.

**Example 2:**
`s1 = [3, 2, 1, 1, 1]`
`s2 = [4, 3, 2]`
`s3 = [2, 5, 4, 1]`

Initial sums: `sum1 = 8`, `sum2 = 9`, `sum3 = 12`.
1. `sum3` is largest. Remove 2 from `s3`. `s3 = [5, 4, 1]`. `sum3 = 10`.
2. `sum3` is largest. Remove 5 from `s3`. `s3 = [4, 1]`. `sum3 = 5`.
3. `sum2` is largest. Remove 4 from `s2`. `s2 = [3, 2]`. `sum2 = 5`.
4. Now `sum1 = 8`, `sum2 = 5`, `sum3 = 5`. `sum1` is largest. Remove 3 from `s1`. `s1 = [2, 1, 1, 1]`. `sum1 = 5`.
5. All sums are 5. Return 5.

## Intuition & Approach

The core idea behind solving this problem efficiently is a greedy approach. To maximize the final equal sum, we want to remove as few elements as possible. If the current sums of the three stacks are not equal, we must reduce one or more sums. To get closer to an equal sum while keeping the potential value as high as possible, we should always reduce the stack that currently has the *largest* sum.

Here's why this greedy strategy works:
1.  **Maximizing the sum:** If we were to reduce a stack with a smaller sum, we would be moving its sum further away from the largest sum, making it harder to equalize at a high value. By reducing the largest sum, we bring it closer to the other sums, maintaining the highest possible common target.
2.  **Stack property:** We can only remove elements from the top. This means we process elements in a fixed order. We can use pointers (indices) to keep track of the current "top" element for each stack.

**Algorithm Steps:**

1.  **Calculate Initial Sums:** First, compute the total sum of elements in each of the three stacks. It's good practice to use `long long` for sums to prevent potential integer overflow, although for the given constraints (max sum 5 * 10^5 * 10^3 = 5 * 10^8), `int` might suffice.
2.  **Initialize Pointers:** Create three pointers, `ptr1`, `ptr2`, `ptr3`, initialized to `0`. These pointers will represent the index of the current top element for each stack.
3.  **Iterative Reduction:** Enter a `while` loop that continues as long as the three sums are not equal (`sum1 == sum2 && sum2 == sum3` is false).
    *   **Impossibility Check:** Inside the loop, before making any reductions, check if any of the pointers have reached the end of their respective stacks (`ptr1 == s1.size() || ptr2 == s2.size() || ptr3 == s3.size()`). If any stack becomes empty and the sums are still not equal, it means it's impossible to achieve a positive equal sum. In this case, we return `0`.
    *   **Greedy Reduction:**
        *   Identify the stack with the maximum current sum.
        *   Remove its top element: subtract `sX[ptrX]` from `sumX` and increment `ptrX`.
        *   If there's a tie for the maximum sum, it doesn't matter which one we pick; the logic `if (sum1 >= sum2 && sum1 >= sum3)` handles this by prioritizing `s1`, then `s2`, then `s3`.
4.  **Return Result:** Once the `while` loop terminates, it means `sum1 == sum2 && sum2 == sum3`. At this point, all three stacks have the same sum, and this is the maximum possible equal sum we could achieve. Return `sum1` (or `sum2` or `sum3`), cast back to `int` as per the function signature.

This approach effectively simulates removing elements from the stacks, always prioritizing the reduction of the largest sum, until all sums are equal or it becomes impossible to make them equal.

## Complexity Analysis

-   **Time Complexity**: $O(N_1 + N_2 + N_3)$
    *   Calculating initial sums takes $O(N_1 + N_2 + N_3)$ time, where $N_1, N_2, N_3$ are the number of elements in stacks `s1`, `s2`, and `s3` respectively.
    *   The `while` loop continues until all sums are equal or one stack becomes empty. In each iteration, exactly one element is "removed" (its value is subtracted from the sum, and its pointer is incremented). In the worst case, we might iterate through almost all elements across all three stacks. Therefore, the loop runs at most $N_1 + N_2 + N_3$ times.
    *   Inside the loop, operations (comparisons, subtractions, increments) are all constant time $O(1)$.
    *   Combining these, the total time complexity is dominated by the initial sum calculation and the loop, resulting in $O(N_1 + N_2 + N_3)$.

-   **Space Complexity**: $O(1)$
    *   We only use a few auxiliary variables to store the current sums (`sum1`, `sum2`, `sum3`) and pointers (`ptr1`, `ptr2`, `ptr3`). These variables consume a constant amount of extra space, regardless of the input stack sizes.

## Solution Code

```cpp
#include <vector>
#include <numeric> // For std::accumulate (optional, direct loop used)
#include <algorithm> // For std::max (optional, if-else chain used)

class Solution {
public:
    int maxEqualSum(std::vector<int>& s1, std::vector<int>& s2, std::vector<int>& s3) {
        // Calculate initial sums for each stack.
        // Using long long for sums to prevent potential overflow, although for the given constraints
        // (max sum 5 * 10^5 * 10^3 = 5 * 10^8), a standard int would also suffice.
        // It's a good habit in competitive programming to use long long for sums.
        long long sum1 = 0;
        for (int x : s1) {
            sum1 += x;
        }

        long long sum2 = 0;
        for (int x : s2) {
            sum2 += x;
        }

        long long sum3 = 0;
        for (int x : s3) {
            sum3 += x;
        }
        
        // Pointers to keep track of the current top element of each stack.
        // These act as indices into the input vectors.
        int ptr1 = 0;
        int ptr2 = 0;
        int ptr3 = 0;

        // The main loop continues as long as the sums of the three stacks are not equal.
        while (!(sum1 == sum2 && sum2 == sum3)) {
            // Impossibility Check:
            // If any stack pointer reaches the end of its vector, it means that stack has become empty.
            // If the sums are still not equal at this point, it's impossible to make them equal
            // to a positive value. The only possible equal sum would be 0.
            // The problem asks for the maximum equal sum. If we can't achieve a positive
            // equal sum, 0 is the answer.
            // This condition correctly handles cases where one stack becomes empty while others
            // still have positive sums (e.g., s1=[4,7], s2=[10], s3=[1,2,3] from example).
            if (ptr1 == s1.size() || ptr2 == s2.size() || ptr3 == s3.size()) {
                return 0;
            }

            // Greedily remove the top element from the stack with the largest current sum.
            // This strategy aims to reduce the largest sum, bringing it closer to the others,
            // thus maximizing the potential equal sum.
            if (sum1 >= sum2 && sum1 >= sum3) {
                sum1 -= s1[ptr1];
                ptr1++;
            } else if (sum2 >= sum1 && sum2 >= sum3) {
                sum2 -= s2[ptr2];
                ptr2++;
            } else { // sum3 must be the largest
                sum3 -= s3[ptr3];
                ptr3++;
            }
        }

        // Once the loop terminates, all three sums are equal.
        // Return this common sum, cast back to int as per the function signature.
        return static_cast<int>(sum1);
    }
};
```