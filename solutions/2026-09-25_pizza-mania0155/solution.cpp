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