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
        long long k = (target_pos - a + net_move_per_cycle - 1) / net_move_per_cycle;

        // Position after k full cycles: k * (a-b)
        // Remaining distance to cover: target_pos - k * (a-b)
        // This remaining distance must be covered by a forward move.
        // Since we chose k such that k * (a-b) + a >= target_pos,
        // it means target_pos - k * (a-b) <= a.
        // So, we just move forward by target_pos - k * (a-b) units.
        long long time_taken = k * time_per_cycle + (target_pos - k * net_move_per_cycle);
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
            long long Y_start = start_part2 - a - 1;
            long long Y_end = X_end - a - 1;

            long long sum_floor_Y_div_net_move = (sum_floor_div(Y_end, net_move) - sum_floor_div(Y_start - 1, net_move) + MOD) % MOD;
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
        // X can be d-j, so from d-r to d-l. d, j up to 10^9. So X can be from 1-10^9 to 10^9-1.
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
            // The problem implies T_normal will always be finite for the sum to be modulo MOD.
            if (T_normal < 2e18) { 
                long long sum_T_normal_part = (count_range2 % MOD * (T_normal % MOD)) % MOD;
                total_sum = (total_sum + sum_T_normal_part) % MOD;
            }
        }

        return static_cast<int>(total_sum);
    }
};