# [Values with Equal Array Remainders](https://www.geeksforgeeks.org/problems/k-modulus-array-element0255/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of `N` positive integers, we need to find the number of positive integers `k` such that all elements `arr[i]` in the array leave the same remainder when divided by `k`. If there are infinitely many such `k`, we should return -1.

**Constraints:**
- `1 <= N <= 10^5`
- `1 <= arr[i] <= 10^5`

## Intuition & Approach

The core of this problem lies in understanding the property of numbers that leave the same remainder when divided by a common divisor `k`.

Let's assume all elements `arr[i]` leave the same remainder `r` when divided by a positive integer `k`. This can be expressed as:
`arr[i] % k = r` for all `i`.

This implies that for any two elements `arr[i]` and `arr[j]` in the array:
`arr[i] = q_i * k + r`
`arr[j] = q_j * k + r`
where `q_i` and `q_j` are integer quotients.

Subtracting these two equations:
`arr[i] - arr[j] = (q_i * k + r) - (q_j * k + r)`
`arr[i] - arr[j] = (q_i - q_j) * k`

This crucial observation tells us that the difference between any two elements in the array (`arr[i] - arr[j]`) must be a multiple of `k`. Consequently, `k` must be a divisor of `|arr[i] - arr[j]|` for all possible pairs `(i, j)`.

To satisfy this condition for all elements, `k` must be a common divisor of all absolute differences `|arr[i] - arr[j]|`. A more efficient way to compute this is to fix one element, say `arr[0]`, and then `k` must divide `|arr[i] - arr[0]|` for all `i` from `1` to `N-1`.

Therefore, `k` must be a common divisor of the set of absolute differences:
`{|arr[1]-arr[0]|, |arr[2]-arr[0]|, ..., |arr[N-1]-arr[0]|} `

Let `G` be the greatest common divisor (GCD) of all these absolute differences:
`G = gcd(|arr[1]-arr[0]|, |arr[2]-arr[0]|, ..., |arr[N-1]-arr[0]|)`

Any positive integer `k` that satisfies the problem's condition must be a divisor of `G`. Conversely, if `k` is a divisor of `G`, then `k` divides `|arr[i]-arr[0]|` for all `i`. This means `arr[i] - arr[0]` is a multiple of `k`, which implies `arr[i] % k = arr[0] % k` for all `i`. Thus, all `arr[i]` will leave the same remainder `arr[0] % k` when divided by `k`.

So, the problem reduces to finding the number of positive divisors of `G`.

**Special Cases:**

1.  **All elements in the array are equal (e.g., `arr = [5, 5, 5]`):**
    In this scenario, all absolute differences `|arr[i] - arr[0]|` will be 0.
    The `gcd(0, 0, ..., 0)` is typically defined as 0.
    If `G = 0`, it means all elements are identical. For any positive integer `k`, `arr[i] % k` will always be `arr[0] % k`. This means all elements will leave the same remainder for *infinitely many* positive integers `k`. The problem statement specifies returning -1 in this case.

2.  **Array size is 1 (e.g., `arr = [10]`):**
    If `N=1`, the loop to calculate differences will not execute. The `common_divisor_candidate` (which represents `G`) will remain its initial value of 0. This correctly falls into the "all elements are equal" special case, and the function should return -1.

**Algorithm Steps:**

1.  Initialize a variable `common_divisor_candidate` to 0. This variable will accumulate the GCD of differences. `std::gcd(0, x)` correctly evaluates to `x`, which helps in initializing the GCD with the first non-zero difference.
2.  Iterate from `i = 1` to `N-1`:
    a. Calculate the absolute difference `diff = std::abs(arr[i] - arr[0])`.
    b. Update `common_divisor_candidate = std::gcd(common_divisor_candidate, diff)`.
3.  After the loop, check the value of `common_divisor_candidate`:
    a. If `common_divisor_candidate == 0`: This indicates that all elements in the array are identical (or `N=1`). In this case, there are infinitely many `k` values, so return -1.
    b. If `common_divisor_candidate > 0`: The number of valid `k` values is equal to the number of positive divisors of `common_divisor_candidate`.
4.  To count the positive divisors of `common_divisor_candidate`:
    a. Initialize `count = 0`.
    b. Iterate `i` from `1` up to `sqrt(common_divisor_candidate)`.
    c. If `i` divides `common_divisor_candidate` (i.e., `common_divisor_candidate % i == 0`):
        i. Increment `count` (for divisor `i`).
        ii. If `i * i != common_divisor_candidate`, it means `i` and `common_divisor_candidate / i` are distinct divisors. Increment `count` again (for divisor `common_divisor_candidate / i`).
5.  Return `count`.

## Complexity Analysis

-   **Time Complexity**:
    -   **GCD Calculation**: The loop runs `N-1` times. Inside the loop, `std::gcd` is called. The Euclidean algorithm for `std::gcd(a, b)` has a time complexity of $O(\log(\min(a, b)))$. The maximum value of `arr[i]` is $10^5$, so the maximum difference is also around $10^5$. Thus, this step is $O(N \log(\max(arr_i)))$. Given $N=10^5$ and $\max(arr_i)=10^5$, this is approximately $10^5 \times \log(10^5) \approx 10^5 \times 17$, which is about $1.7 \times 10^6$ operations.
    -   **Divisor Counting**: This loop runs up to `sqrt(common_divisor_candidate)` times. The maximum value of `common_divisor_candidate` is $10^5 - 1$. `sqrt(10^5 - 1) \approx 316$. This step is $O(\sqrt{\max(arr_i)})$.
    -   The dominant factor is the GCD calculation.
    -   Total Time Complexity: $O(N \log(\max(arr_i)) + \sqrt{\max(arr_i)})$.

-   **Space Complexity**:
    -   The solution uses a few integer variables to store the GCD candidate and the divisor count. It does not use any auxiliary data structures that scale with the input size `N`.
    -   Total Space Complexity: $O(1)$.

## Solution Code

```cpp
#include <vector>    // Required for std::vector
#include <numeric>   // Required for std::gcd (available in C++17)
#include <cmath>     // Required for std::abs and std::sqrt

class Solution {
public:
    // Renamed the method from 'countValues' to 'sameMod' as per the GFG compilation error.
    int sameMod(std::vector<int>& arr) {
        // According to constraints, arr.size() >= 1, so arr will not be empty.
        // If arr.size() == 1 (e.g., arr = [10]), there's only one element.
        // Any positive integer k will make 10 % k the remainder for all elements
        // (since there's only one). This means infinitely many k.
        // My current logic handles this:
        // If arr.size() == 1, the loop for differences won't execute.
        // `common_divisor_candidate` will remain 0.
        // The `if (common_divisor_candidate == 0)` block will then execute, returning -1.
        // This is the correct behavior for arr.size() == 1.

        // Step 1: Calculate the greatest common divisor (GCD) of all absolute differences
        // |arr[i] - arr[0]| for i from 1 to arr.size() - 1.
        // If all elements arr[i] leave the same remainder 'r' when divided by 'k',
        // then arr[i] = q_i * k + r for some integer q_i.
        // This implies that arr[i] - arr[j] must be a multiple of k for any i, j.
        // Specifically, k must divide arr[i] - arr[0] for all i > 0.
        // Thus, k must be a common divisor of all |arr[i] - arr[0]|.
        // The values of k that satisfy the condition are precisely the positive divisors
        // of G = gcd(|arr[1]-arr[0]|, |arr[2]-arr[0]|, ..., |arr[N-1]-arr[0]|).

        // We initialize common_divisor_candidate to 0.
        // std::gcd(0, x) = x, std::gcd(x, 0) = x, std::gcd(0, 0) = 0.
        // This property correctly handles the first non-zero difference encountered
        // (it becomes the initial GCD) and subsequent differences.
        int common_divisor_candidate = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            // std::abs is used to ensure positive differences.
            // The differences can be up to 10^5 - 1, which fits within an int.
            common_divisor_candidate = std::gcd(common_divisor_candidate, std::abs(arr[i] - arr[0]));
        }

        // Step 2: Check if common_divisor_candidate is 0.
        // If common_divisor_candidate is 0, it means all absolute differences |arr[i] - arr[0]| were 0.
        // This implies that all elements in the array `arr` are equal to `arr[0]`.
        // If all elements are equal (e.g., [5, 5, 5]), then for any positive integer k,
        // all elements will leave the same remainder (arr[0] % k).
        // In this scenario, there are infinitely many such values of k.
        if (common_divisor_candidate == 0) {
            return -1;
        }

        // Step 3: If common_divisor_candidate is positive, count its positive divisors.
        // The values of k that satisfy the condition are precisely the positive divisors
        // of `common_divisor_candidate`.
        int count = 0;
        // Iterate from 1 up to the square root of common_divisor_candidate.
        // This is an efficient way to find all divisors.
        // The maximum value of common_divisor_candidate is 10^5 - 1, so sqrt(10^5) is approx 316.
        // The loop runs a maximum of ~316 times.
        for (int i = 1; i * i <= common_divisor_candidate; ++i) {
            if (common_divisor_candidate % i == 0) {
                // If 'i' divides common_divisor_candidate, then 'i' is a divisor.
                count++;
                // Also, 'common_divisor_candidate / i' is a divisor.
                // We need to check if 'i' and 'common_divisor_candidate / i' are distinct.
                // They are distinct if i*i is not equal to common_divisor_candidate.
                if (i * i != common_divisor_candidate) {
                    count++;
                }
            }
        }

        return count;
    }
};
```