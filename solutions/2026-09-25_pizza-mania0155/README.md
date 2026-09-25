# GeeksforGeeks POTD: Minimum Cost Pizza Selection

## Problem Link
[Minimum Cost Pizza Selection](https://www.geeksforgeeks.org/problems/pizza-mania0155/1)

## Platform
GeeksforGeeks (Problem of the Day)

## Difficulty
Medium

## Solved In
2 attempt(s)

## Problem Summary
The problem asks us to find the minimum cost to obtain at least a target area `x` of pizza. We are given three types of pizzas: small, medium, and large, with their respective areas (`s`, `m`, `l`) and costs (`cs`, `cm`, `cl`). We can buy any number of pizzas of each type.

## Intuition & Approach

This problem can be modeled as a variation of the **Unbounded Knapsack** or **Coin Change** problem. We want to achieve a certain "value" (area) with minimum "cost".

**Key Observations:**

1.  **Target Area:** We need *at least* `x` area. This means if we achieve an area `y` where `y >= x`, it's a valid solution.
2.  **Pizza Combinations:** We can combine pizzas of different sizes to reach the target area.
3.  **Minimum Cost:** We are looking for the minimum cost.

**Dynamic Programming Approach:**

Let `dp[i]` be the minimum cost to achieve *exactly* `i` units of pizza area.

*   **State:** `dp[i]` = minimum cost to get exactly `i` area.
*   **Base Case:** `dp[0] = 0` (0 area costs 0).
*   **Transitions:** To calculate `dp[i]`, we can consider adding each type of pizza to a previously achievable area.
    *   If we add a small pizza (area `s`, cost `cs`) to achieve area `i`, it means we must have previously achieved `i - s` area. The cost would be `dp[i - s] + cs`. This is only possible if `i >= s` and `dp[i - s]` is not infinity (meaning `i - s` area is achievable).
    *   Similarly, for a medium pizza (area `m`, cost `cm`): `dp[i - m] + cm` if `i >= m` and `dp[i - m]` is not infinity.
    *   And for a large pizza (area `l`, cost `cl`): `dp[i - l] + cl` if `i >= l` and `dp[i - l]` is not infinity.

    Therefore, the recurrence relation is:
    `dp[i] = min(dp[i], dp[i - s] + cs)` (if `i >= s`)
    `dp[i] = min(dp[i], dp[i - m] + cm)` (if `i >= m`)
    `dp[i] = min(dp[i], dp[i - l] + cl)` (if `i >= l`)

*   **Maximum Area to Consider:** What is the maximum area we need to compute DP for? If our target is `x`, and the largest pizza has area `l`, we might end up buying pizzas that sum up to an area slightly larger than `x`. For example, if `x = 16` and `l = 9`, we might buy two large pizzas to get `18` area. If we have `x-1` area and buy a large pizza, we get `x-1+l` area. So, a safe upper bound for the DP table size would be `x + max(s, m, l)`. Since `s <= m <= l`, this simplifies to `x + l`. Given the constraints (`x <= 500`, `l <= 100`), `max_area` will be at most `600`.

*   **Final Answer:** After filling the `dp` table up to `max_area`, we need to find the minimum cost to achieve *at least* `x` area. This means we iterate from `dp[x]` to `dp[max_area]` and find the minimum value among them.

**Initialization:**
The `dp` array should be initialized with a large value (representing infinity) for all entries except `dp[0]`. A value like `1e9` (1,000,000,000) is suitable as the maximum possible cost (e.g., 600 units of area * 500 cost/unit = 300,000) is well within this limit and won't cause overflow when adding costs.

**Code Structure:**

1.  Define `max_area = x + l`.
2.  Create a `dp` vector of size `max_area + 1` and initialize it with `1e9`.
3.  Set `dp[0] = 0`.
4.  Iterate from `i = 1` to `max_area`:
    *   Apply the transition rules for small, medium, and large pizzas, updating `dp[i]` with the minimum cost.
5.  Initialize `min_total_cost = 1e9`.
6.  Iterate from `i = x` to `max_area`:
    *   Update `min_total_cost = min(min_total_cost, dp[i])`.
7.  Return `min_total_cost`.

**Correction during attempts:**
The initial attempt might have had a function name mismatch with the driver code. The provided solution code correctly renames the function to `minimumCost` to match the expected signature.

## Complexity Analysis

*   **Time Complexity**: $O(\text{max\_area} \times \text{number of pizza types})$
    The DP table has a size of `max_area + 1`. For each entry in the DP table, we perform a constant number of operations (checking three pizza types).
    `max_area` is approximately `x + l`.
    So, the time complexity is $O((x + l) \times 3)$, which simplifies to $O(x + l)$. Given the constraints, this is efficient.

*   **Space Complexity**: $O(\text{max\_area})$
    We use a DP table of size `max_area + 1` to store the minimum costs.
    So, the space complexity is $O(x + l)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // For std::min
#include <limits>    // For std::numeric_limits<int>::max() - though 1e9 is used here

class Solution {
public:
    // The compilation error indicates that the function name in the driver code
    // is 'minimumCost', but the implemented function is 'minCostPizzaSelection'.
    // Renaming the function to match the driver code.
    int minimumCost(int x, int s, int m, int l, int cs, int cm, int cl) {
        // Determine the maximum area we might need to calculate DP for.
        // If we need at least 'x' area, and the largest pizza has area 'l',
        // then the maximum area we might need to consider is x + l.
        // For example, if x=16 and l=9, we might need 18 area (2 large pizzas).
        // If we have 15 area, and buy a large pizza, we get 15+9=24 area.
        // The smallest pizza has area 's'. If we need 'x' area, and we have 'x-1' area,
        // buying a small pizza gives 'x-1+s'. So, the maximum relevant area is x + max(s, m, l).
        // Since s <= m <= l, this simplifies to x + l.
        // Given x <= 500 and l <= 100, max_area will be at most 500 + 100 = 600.
        int max_area = x + l; 
        
        // dp[i] will store the minimum cost to achieve *exactly* i units of area.
        // Initialize with a large value representing infinity.
        // A value like 1e9 (1,000,000,000) is suitable for 'int' as the maximum
        // possible cost (600 units of area * 500 cost/unit) is 300,000,
        // which is much smaller than 1e9 and won't cause overflow when adding costs.
        std::vector<int> dp(max_area + 1, 1e9); 
        
        // Base case: 0 area costs 0.
        dp[0] = 0;
        
        // Iterate through all possible areas from 1 up to max_area.
        // This is the core dynamic programming loop, similar to unbounded knapsack.
        for (int i = 1; i <= max_area; ++i) {
            // Option 1: Achieve area 'i' by adding a small pizza to a configuration
            // that previously achieved 'i - s' area.
            if (i >= s && dp[i - s] != 1e9) { // Check if (i-s) area is reachable (not infinity)
                dp[i] = std::min(dp[i], dp[i - s] + cs);
            }
            
            // Option 2: Achieve area 'i' by adding a medium pizza.
            if (i >= m && dp[i - m] != 1e9) { // Check if (i-m) area is reachable
                dp[i] = std::min(dp[i], dp[i - m] + cm);
            }
            
            // Option 3: Achieve area 'i' by adding a large pizza.
            if (i >= l && dp[i - l] != 1e9) { // Check if (i-l) area is reachable
                dp[i] = std::min(dp[i], dp[i - l] + cl);
            }
        }
        
        // After filling the DP table, we need to find the minimum cost for
        // *at least* 'x' units of area. This means we look at all areas
        // from 'x' up to 'max_area' and find the minimum cost among them.
        int min_total_cost = 1e9; // Initialize with infinity
        for (int i = x; i <= max_area; ++i) {
            min_total_cost = std::min(min_total_cost, dp[i]);
        }
        
        // The problem constraints imply that a solution is always possible
        // (e.g., by buying enough pizzas of any type). So min_total_cost
        // will not remain 1e9.
        return min_total_cost;
    }
};
```