#include <string> // Required for std::string

class Solution {
public:
    char posInfinite(const std::string& s_initial, long long pos) { // Renamed findChar to posInfinite
        long long N = s_initial.size();

        // If pos is within the initial string, return it directly.
        // This handles the base case S_0.
        if (pos <= N) {
            return s_initial[pos - 1];
        }

        // Find the smallest k_val such that pos <= L_{k_val}
        // L_0 = N
        // L_k = 2 * L_{k-1} + k
        long long len_S_k = N; // This will store L_k, the length of S_k
        int k_val = 0;         // This will store k, the current step number

        // Iterate to find the smallest k_val such that pos falls within S_{k_val}
        // We start with k_val = 0, len_S_k = N (L_0).
        // If pos > L_0, we need to consider S_1.
        // L_1 = 2 * L_0 + 1.
        // If pos > L_1, we need to consider S_2.
        // L_2 = 2 * L_1 + 2.
        // And so on.
        while (pos > len_S_k) {
            k_val++;
            // Check for potential overflow before calculation.
            // If len_S_k becomes too large, 2 * len_S_k + k_val might exceed long long max.
            // However, pos is up to 10^18. L_k grows roughly as N * 2^k + k * 2^k.
            // For pos = 10^18, k_val will be around 60.
            // 2 * 10^18 + 60 would overflow.
            // The maximum value for pos is typically 10^18.
            // If len_S_k exceeds pos, we stop.
            // The maximum value of len_S_k we need to calculate is slightly larger than pos.
            // If pos is 10^18, len_S_k will be around 10^18.
            // 2 * len_S_k could overflow if len_S_k is already > LLONG_MAX / 2.
            // A safer check: if (len_S_k > (LLONG_MAX - k_val) / 2) { /* handle overflow or break */ }
            // Given pos <= 10^18, len_S_k will not exceed this significantly.
            // For N=1, k=60, L_60 approx 2^60 which is ~1.15 * 10^18.
            // So 2 * L_k + k can reach ~2.3 * 10^18, which fits in long long.
            len_S_k = 2 * len_S_k + k_val;
        }

        // Now we have:
        // `pos`: the 1-based index we are looking for, relative to S_{k_val}.
        // `len_S_k`: the length of the string S_{k_val}.
        // `k_val`: the step number that produced S_{k_val}.
        // We know `pos <= len_S_k`.

        // Iteratively trace back to S_0.
        // The loop continues as long as we are looking in S_k where k > 0.
        while (k_val > 0) {
            // S_{k_val} is constructed as:
            // S_{k_val-1} (length len_S_prev)
            // + ('$' repeated k_val times) (length k_val)
            // + reverse(S_{k_val-1}) (length len_S_prev)

            // Calculate the length of S_{k_val-1}
            // From L_k = 2 * L_{k-1} + k, we get L_{k-1} = (L_k - k) / 2
            long long len_S_prev = (len_S_k - k_val) / 2;

            // Check which part `pos` falls into within S_{k_val}:
            // Part 1: The first S_{k_val-1} segment
            if (pos <= len_S_prev) {
                // `pos` is in the first S_{k_val-1} part.
                // We update `len_S_k` to `len_S_prev` and `k_val` to `k_val-1`
                // to continue searching in the previous string.
                len_S_k = len_S_prev;
                k_val--;
            }
            // Part 2: The '$' characters segment
            else if (pos <= len_S_prev + k_val) {
                // `pos` is in the '$' part.
                return '$';
            }
            // Part 3: The reversed S_{k_val-1} segment
            else {
                // `pos` is in the reversed S_{k_val-1} part.
                // Calculate the position relative to the start of this reversed segment.
                long long relative_pos_in_reversed_part = pos - (len_S_prev + k_val);
                
                // Map this relative position back to the corresponding 1-based index
                // in the original (unreversed) S_{k_val-1} string.
                // If relative_pos_in_reversed_part is 1, it corresponds to the last char of S_{k_val-1}.
                // If relative_pos_in_reversed_part is len_S_prev, it corresponds to the first char of S_{k_val-1}.
                pos = len_S_prev - relative_pos_in_reversed_part + 1;
                
                // Update `len_S_k` and `k_val` to continue searching in the previous string.
                len_S_k = len_S_prev;
                k_val--;
            }
        }

        // If the loop finishes, `k_val` is 0.
        // This means `pos` has been reduced to an index within the initial string `s_initial`.
        // The initial `if (pos <= N)` check handles cases where pos is initially within S_0.
        // For cases where pos was initially greater than N, this final `pos` will be
        // guaranteed to be `1 <= pos <= N`.
        return s_initial[pos - 1];
    }
};