This writeup provides a detailed explanation of the problem, the intuition behind the solution, the step-by-step approach, and a complexity analysis, followed by the provided C++ code.

---

# [Geek's Destination with Special Jump](https://www.geeksforg eeks.org/problems/geek-and-her-destination--145724/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 2 attempt(s)

## Problem Summary

Geek starts at position 0 and wants to reach a destination `d`. There are two types of moves:
1.  **Normal Move**: Move `a` units forward, then `b` units backward. This cycle takes `a+b` time and results in a net forward movement of `a-b`.
2.  **Special Jump**: From the current position `P`, Geek can jump `j` units forward to `P+j`. This takes `1` unit of time. This special jump can be taken at any point during the journey.

We are given a range `[l, r]` for the special jump length `j`. The task is to calculate the sum of minimum times required to reach `d` for each `j` in `[l, r]`, modulo `10^9 + 7`.

For each `j` in `[l, r]`, the minimum time to reach `d` involves considering two possibilities:
*   **Scenario 1**: Reach `d` using only normal moves. Let this time be `T_normal`.
*   **Scenario 2**: Reach an intermediate position `d-j` using normal moves, then take a special jump of `j` units (costing 1 unit of time) to reach `d`. Let this time be `T_special_j = calc_time(d-j, a, b) + 1`.

The minimum time for a given `j` is `min(T_normal, T_special_j)`. We need to sum these minimum times for all `j` from `l` to `r`.

## Intuition & Approach

The core of the problem lies in two parts:
1.  Efficiently calculating `calc_time(target_pos, a, b)`, which is the minimum time to reach `target_pos` using only normal moves.
2.  Efficiently summing `min(T_normal, calc_time(d-j, a, b) + 1)` over a large range of `j` values (`[l, r]`, up to `10^9` elements).

Let's break down the approach:

### 1. `calc_time(target_pos, a, b)` Function

This function determines the time to reach `target_pos` using only normal moves.
*   **Base Case**: If `target_pos <= 0`, the time is `0`.
*   **First Forward Move**: If `0 < target_pos <= a`, Geek can reach `target_pos` in `target_pos` time by simply moving forward.
*   **Impossible Case**: If `target_pos > a` and `a <= b`, Geek cannot make net forward progress beyond `a` (since `a-b <= 0`). In this case, `target_pos` is unreachable. We return a very large number (e.g., `2e18`) to represent infinity. This value is chosen to be larger than any possible finite time but small enough to avoid overflow issues when added to other values.
*   **General Case (`target_pos > a` and `a > b`)**:
    *   Let `net_move_per_cycle = a - b` and `time_per_cycle = a + b`.
    *   We need to find the number of full cycles `k` such that `k * net_move_per_cycle + a >= target_pos`.
    *   The smallest such `k` is `ceil((target_pos - a) / net_move_per_cycle)`. Since `target_pos > a` and `net_move_per_cycle > 0`, `k` will be at least 1.
    *   The formula for `ceil(N/D)` for positive `N, D` is `(N + D - 1) / D` using integer division. So, `k = (target_pos - a + net_move_per_cycle - 1) / net_move_per_cycle`.
    *   The total time taken is `k * time_per_cycle + (target_pos - k * net_move_per_cycle)`. This simplifies to `target_pos + k * (time_per_cycle - net_move_per_cycle)`.
    *   Since `time_per_cycle - net_move_per_cycle = (a+b) - (a-b) = 2b`, the time is `target_pos + k * 2b`.

### 2. Summing `min(T_normal, calc_time(d-j, a, b) + 1)`

Let `X = d-j`. As `j` goes from `l` to `r`, `X` goes from `d-l` down to `d-r`. Let `X_min = d-r` and `X_max = d-l`. We need to sum `min(T_normal, calc_time(X, a, b) + 1)` for `X` in `[X_min, X_max]`.

The function `f(X) = calc_time(X, a, b) + 1` is monotonically non-decreasing with `X`. This means there's a threshold `X_0` such that:
*   For `X < X_0`, `f(X) < T_normal`, so `min(T_normal, f(X)) = f(X)`.
*   For `X >= X_0`, `f(X) >= T_normal`, so `min(T_normal, f(X)) = T_normal`.

We can find this `X_0` using **binary search**. The search range for `X` can be from `d-r` to `d-l`, which can span `[-10^9, 10^9]`. A slightly wider range like `[-2*10^9, 2*10^9]` is used for safety.

Once `X_0` is found, the total sum can be split into two parts:
*   **Part 1**: Sum `(calc_time(X, a, b) + 1)` for `X` in `[X_min, min(X_max, X_0 - 1)]`.
*   **Part 2**: Sum `T_normal` for `X` in `[max(X_min, X_0), X_max]`.

### 3. `sum_calc_time_range(X_start, X_end, a, b)` Function

This helper function calculates `sum_{X=X_start}^{X_end} calc_time(X, a, b)` modulo `MOD`. It handles `X <= 0` by effectively ignoring them (as `calc_time(X)=0`). The range `[X_start, X_end]` is further split based on the structure of `calc_time(X, a, b)`:

*   **Sub-part 1: `X` in `[current_X_start, min(X_end, a)]`**: Here `calc_time(X, a, b) = X`. The sum is an arithmetic series `sum X`.
*   **Sub-part 2: `X` in `[max(current_X_start, a+1), X_end]`**: (This part is only relevant if `a > b`). Here `calc_time(X, a, b) = X + k_X * 2b`, where `k_X = floor((X - a - 1) / (a-b)) + 1`.
    *   The `sum X` part is another arithmetic series.
    *   The `sum k_X * 2b` part requires summing `(floor((X - a - 1) / (a-b)) + 1)` over the range, then multiplying by `2b`.
    *   Let `Y = X - a - 1`. We need to sum `(floor(Y / net_move) + 1)`. This involves summing `floor(Y / net_move)` over a range `[Y_start, Y_end]`.

### 4. `sum_floor_div(N, C)` Function

This helper function calculates `sum_{i=0}^{N} floor(i/C)` modulo `MOD`. This is a standard formula:
`sum_{i=0}^{N} floor(i/C) = (C * q * (q-1) / 2) + (r+1) * q`, where `q = N/C` and `r = N%C`.
All calculations are performed modulo `MOD`. Division by 2 requires modular inverse of 2 (`INV2 = (MOD + 1) / 2`).

### Modular Arithmetic and Data Types

*   All intermediate sums and products must be taken modulo `10^9 + 7`.
*   Subtraction `(A - B)` should be `(A - B + MOD) % MOD` to handle potential negative results.
*   Input parameters `a, b, d, l, r` can be up to `10^9`. All calculations involving these values and intermediate sums (especially `calc_time` results) must use `long long` to prevent overflow before applying the modulo operator.

### Overall Algorithm

1.  Initialize `MOD = 10^9 + 7` and `INV2 = 500000004`.
2.  Convert input `int` values `a, b, d, l, r` to `long long`.
3.  Calculate `T_normal = calc_time(d, a, b)`.
4.  Define `X_min = d-r` and `X_max = d-l`.
5.  Perform binary search for `X_0` (the smallest `X` such that `calc_time(X, a, b) + 1 >= T_normal`).
6.  Calculate `total_sum` by splitting the range `[X_min, X_max]` into two parts based on `X_0`:
    *   For `X` in `[X_min, min(X_max, X_0 - 1)]`: Add `sum_calc_time_range(X, a, b) + (count % MOD)` to `total_sum`.
    *   For `X` in `[max(X_min, X_0), X_max]`: Add `(count % MOD * (T_normal % MOD))` to `total_sum`. (Ensure `T_normal` is not the "infinity" value `2e18`).
7.  Return `total_sum` cast to `int`.

This approach efficiently handles the large input ranges by converting sums into arithmetic series and sums of floor divisions, all computed in constant time after the initial binary search.

## Complexity Analysis

*   **Time Complexity**:
    *   `calc_time` function: Performs a constant number of arithmetic operations, so it's $O(1)$.
    *   `sum_floor_div` function: Also performs a constant number of arithmetic operations, so it's $O(1)$.
    *   `sum_calc_time_range` function: This function makes a few calls to `sum_floor_div` and performs arithmetic series sums, all of which are $O(1)$. Thus, `sum_calc_time_range` is $O(1)$.
    *   `minTime` function (main logic):
        *   Calculating `T_normal`: $O(1)$.
        *   Binary search for `X_0`: The search range for `X` is approximately `4 * 10^9`. A binary search over this range takes $O(\log(\text{range of X}))$ iterations. Each iteration involves a call to `calc_time`, which is $O(1)$. So, the binary search is $O(\log(4 \cdot 10^9)) \approx O(32-35)$ operations.
        *   Two calls to `sum_calc_time_range`: $O(1)$ each.
        *   Other operations: $O(1)$.
    *   Therefore, the overall time complexity is dominated by the binary search, resulting in $O(\log(\text{max_val}))$, where `max_val` is the maximum possible value of `X`.

*   **Space Complexity**:
    *   The solution uses a fixed number of `long long` variables for calculations and does not use any data structures that scale with input size.
    *   Therefore, the space complexity is $O(1)$.

## Solution Code

```cpp
#include <iostream>
#include <algorithm> // For std::min, std::max
#include <climits>   // For LLONG_MAX (though 2e18 is used instead)

class Solution {
public:
    long long MOD = 1e9 + 7;
    long long INV2 = 500000004; // Modular inverse of 2 for MOD = 10^9 + 7

    // Helper function to calculate time to reach or pass target_pos using normal moves
    // Returns a very large number (2e18) if target_pos > a and a <= b, indicating impossibility.
    long long calc_time(long long target_pos, long long a, long long b) {
        if (target_pos <= 0) {
            return 0;
        }

        // Case 1: target_pos is reached during the first forward move (k=0 cycles)
        if (target_pos <= a) {
            return target_pos;
        }

        // If target_pos > a and a <= b, Geek can never reach target_pos using normal moves.
        // Net movement per cycle is a-b. If a-b <= 0, Geek cannot make net forward progress beyond 'a'.
        if (a <= b) {
            // Return a very large number to represent infinity.
            // This value must be large enough to be greater than any possible finite time,
            // but not so large that it causes overflow when adding 1.
            // Max d is 1e9. Max time could be d * (a+b)/(a-b) ~ 1e9 * (2e9)/(1) = 2e18.
            // So 2e18 is a safe "infinity" for time.
            return 2e18; 
        }

        long long net_move_per_cycle = a - b;
        long long time_per_cycle = a + b;

        // k = ceil((target_pos - a) / net_move_per_cycle)
        // Since target_pos > a, target_pos - a > 0. net_move_per_cycle > 0.
        // So k will be at least 1.
        // This is equivalent to floor((target_pos - a - 1) / net_move_per_cycle) + 1
        long long k = (target_pos - a + net_move_per_cycle - 1) / net_move_per_cycle;

        // Simplified time calculation: target_pos + k * (time_per_cycle - net_move_per_cycle)
        // time_per_cycle - net_move_per_cycle = (a+b) - (a-b) = 2b
        long long time_taken = target_pos + k * (2 * b);
        return time_taken;
    }

    // Helper function to calculate sum_{i=0}^{N} floor(i/C) modulo MOD
    // This is sum_{i=0}^{N} floor(i/C) = (C * q * (q-1) / 2) + (r+1) * q
    // where q = N/C, r = N%C
    long long sum_floor_div(long long N, long long C) {
        if (N < 0) return 0; // Sum for negative N is 0
        // C (net_move) is guaranteed to be > 0 when this function is called in sum_calc_time_range's Part 2.
        long long q = N / C;
        long long r = N % C;
        
        long long term1_val = (C % MOD * q % MOD * (q - 1 + MOD) % MOD * INV2 % MOD) % MOD;
        long long term2_val = ((r + 1) % MOD * q % MOD) % MOD;
        return (term1_val + term2_val) % MOD;
    }

    // Helper function to calculate sum_{X=X_start}^{X_end} calc_time(X, a, b) modulo MOD
    // This function assumes that for all X in [X_start, X_end], calc_time(X, a, b) is finite.
    // (i.e., X <= a or a > b).
    long long sum_calc_time_range(long long X_start, long long X_end, long long a, long long b) {
        if (X_start > X_end) return 0;

        long long total_sum = 0;
        // net_move is only used in Part 2, which is only entered if a > b.
        long long net_move = a - b; 

        // Handle X <= 0 separately, where calc_time(X) = 0. These contribute 0 to the sum.
        long long current_X_start = std::max(1LL, X_start);
        if (current_X_start > X_end) return 0; // All X were <= 0 or range is empty

        // Part 1: X in [current_X_start, min(X_end, a)]. Here calc_time(X) = X.
        long long end_part1 = std::min(X_end, a);
        if (current_X_start <= end_part1) {
            long long count = end_part1 - current_X_start + 1;
            long long sum_X = (count % MOD * (current_X_start % MOD + end_part1 % MOD) % MOD * INV2 % MOD) % MOD;
            total_sum = (total_sum + sum_X) % MOD;
        }

        // Part 2: X in [max(current_X_start, a + 1LL), X_end]. Here calc_time(X) = X + 2b * k_X
        // This part is only reached if a > b, as per the logic in the main function.
        long long start_part2 = std::max(current_X_start, a + 1LL);
        if (start_part2 <= X_end) {
            // Sum X part
            long long count_part2 = X_end - start_part2 + 1;
            long long sum_X_part2 = (count_part2 % MOD * (start_part2 % MOD + X_end % MOD) % MOD * INV2 % MOD) % MOD;
            total_sum = (total_sum + sum_X_part2) % MOD;

            // Sum k_X * 2b part
            // k_X = floor((X - a - 1) / net_move) + 1
            // So we need to sum (floor((X - a - 1) / net_move) + 1) for X in [start_part2, X_end]
            // Let Y = X - a - 1.
            long long Y_start = start_part2 - a - 1;
            long long Y_end = X_end - a - 1;

            // Sum floor(Y / net_move) from Y_start to Y_end
            long long sum_floor_Y_div_net_move = (sum_floor_div(Y_end, net_move) - sum_floor_div(Y_start - 1, net_move) + MOD) % MOD;
            
            // Sum of 1s (for the +1 in k_X = floor(...) + 1)
            long long sum_ones = (Y_end - Y_start + 1) % MOD;
            
            long long sum_k_X = (sum_floor_Y_div_net_move + sum_ones) % MOD;
            
            long long term_2b = (2 * b) % MOD;
            long long sum_k_X_2b = (sum_k_X * term_2b) % MOD;
            
            total_sum = (total_sum + sum_k_X_2b) % MOD;
        }
        return total_sum;
    }

    int minTime(int a_int, int b_int, int d_int, int l_int, int r_int) {
        // Convert inputs to long long to prevent overflow during calculations
        long long a = a_int;
        long long b = b_int;
        long long d = d_int;
        long long l = l_int;
        long long r = r_int;

        // Calculate T_normal: time to reach d using only normal moves
        long long T_normal = calc_time(d, a, b);

        // X is d - j. As j goes from l to r, X goes from d-l down to d-r.
        // We are interested in X values in the range [X_min, X_max].
        long long X_min = d - r;
        long long X_max = d - l;

        // Binary search for X_0: smallest X such that calc_time(X, a, b) + 1 >= T_normal
        // The search range for X_0 should cover all possible X values that could be relevant.
        // X can be d-j, so from d-r to d-l. d, j up to 1e9. So X can be from 1-1e9 to 1e9-1.
        // A safe range for X is roughly [-1e9, 1e9]. Let's use a slightly wider range.
        long long low = -2000000000LL; // A value smaller than any possible X_min
        long long high = 2000000000LL;  // A value larger than any possible X_max
        long long X_0 = high + 1;       // Initialize X_0 to a value that ensures it will be updated

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (calc_time(mid, a, b) + 1 >= T_normal) {
                X_0 = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        long long total_sum = 0;

        // Part 1: X in [X_min, min(X_max, X_0 - 1)]
        // For these X, calc_time(X) + 1 < T_normal, so min_time = calc_time(X) + 1
        long long range1_start = X_min;
        long long range1_end = std::min(X_max, X_0 - 1);
        if (range1_start <= range1_end) {
            long long sum_calc_X_part = sum_calc_time_range(range1_start, range1_end, a, b);
            long long count_range1 = range1_end - range1_start + 1;
            long long sum_ones_part = count_range1 % MOD; // Add 1 for each X in the range
            total_sum = (total_sum + sum_calc_X_part + sum_ones_part) % MOD;
        }

        // Part 2: X in [max(X_min, X_0), X_max]
        // For these X, calc_time(X) + 1 >= T_normal, so min_time = T_normal
        long long range2_start = std::max(X_min, X_0);
        long long range2_end = X_max;
        if (range2_start <= range2_end) {
            long long count_range2 = range2_end - range2_start + 1;
            // Only add T_normal if it's a finite value.
            // If T_normal is 2e18, it means d is unreachable by normal moves.
            // In this case, X_0 would be very large, making range2_start > range2_end,
            // so this block might not be entered. But it's good to be explicit.
            if (T_normal < 2e18) { 
                long long sum_T_normal_part = (count_range2 % MOD * (T_normal % MOD)) % MOD;
                total_sum = (total_sum + sum_T_normal_part) % MOD;
            }
        }

        return static_cast<int>(total_sum);
    }
};
```