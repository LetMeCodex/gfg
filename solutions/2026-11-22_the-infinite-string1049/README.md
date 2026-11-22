# Character at Position in Infinite String

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

We are given an initial string `s_initial`. An infinite string `S` is constructed recursively as follows:
- `S_0 = s_initial`
- `S_k = S_{k-1} + ('$' repeated k times) + reverse(S_{k-1})` for `k > 0`.

The infinite string `S` is the concatenation of `S_0, S_1, S_2, ...`. We are given a 1-based index `pos` and need to find the character at this position in the infinite string `S`.

## Intuition & Approach

The core of the problem lies in understanding the recursive construction of the infinite string and efficiently determining which segment of the recursive construction `pos` falls into.

Let `N` be the length of the initial string `s_initial`.
Let `L_k` be the length of the string `S_k`.

We have the following relations:
- `L_0 = N`
- `L_k = L_{k-1} + k + L_{k-1} = 2 * L_{k-1} + k` for `k > 0`.

The infinite string `S` is formed by concatenating `S_0`, then `S_1`, then `S_2`, and so on. However, the problem statement implies that the infinite string is formed by `S_0` followed by `S_1`, `S_2`, etc., where each `S_k` is defined as above. This means the infinite string is effectively `S_0` followed by `S_1` followed by `S_2`... where `S_k` itself is a composite string.

The problem statement can be interpreted as:
The infinite string is formed by `S_0`, then `S_1`, then `S_2`, ... where `S_k` is defined as `S_{k-1} + ('$' repeated k times) + reverse(S_{k-1})`.
This means the infinite string is `S_0` followed by `S_1` followed by `S_2`...
The length of `S_k` is `L_k`.

The first step is to determine which `S_k` the given `pos` falls into. We can do this by iteratively calculating the lengths `L_0, L_1, L_2, ...` until we find a `k_val` such that `pos <= L_{k_val}`.

Once we find this `k_val`, we know that the character at `pos` is within the string `S_{k_val}`. `S_{k_val}` is constructed from `S_{k_val-1}`, a sequence of `k_val` dollar signs, and the reverse of `S_{k_val-1}`. We can then determine which of these three parts `pos` belongs to:

1.  **The first `S_{k_val-1}` segment**: If `pos` is less than or equal to the length of `S_{k_val-1}` (let's call this `len_S_prev`), then the character is within this first segment. We then recursively solve the problem for `pos` within `S_{k_val-1}`.

2.  **The `k_val` dollar signs segment**: If `pos` is greater than `len_S_prev` but less than or equal to `len_S_prev + k_val`, then the character is a dollar sign. We can directly return '$'.

3.  **The reversed `S_{k_val-1}` segment**: If `pos` is greater than `len_S_prev + k_val`, then the character is in the reversed `S_{k_val-1}` segment. We need to calculate the corresponding position in the original `S_{k_val-1}`. If the position within the reversed segment is `relative_pos_in_reversed_part`, then the corresponding position in the original `S_{k_val-1}` is `len_S_prev - relative_pos_in_reversed_part + 1`. We then recursively solve the problem for this new `pos` within `S_{k_val-1}`.

We repeat this process, reducing `k_val` and adjusting `pos` until `k_val` becomes 0. At this point, `pos` will be an index within the original string `s_initial`, and we can return `s_initial[pos - 1]`.

**Edge Case**: If the initial `pos` is already less than or equal to `N` (the length of `s_initial`), we can directly return `s_initial[pos - 1]`. This is handled by the initial check.

**Overflow Considerations**: The lengths `L_k` can grow very rapidly. For `pos` up to $10^{18}$, `k_val` can be around 60. The calculation `2 * len_S_k + k_val` needs to be done carefully to avoid `long long` overflow. However, since `pos` is the upper bound we are interested in, `len_S_k` will not significantly exceed `pos`. For `pos = 10^{18}`, `len_S_k` will be around $10^{18}$. `2 * 10^{18} + 60` fits within `long long`.

The approach involves two main phases:
1.  **Finding `k_val`**: Iteratively calculate `L_k` until `pos <= L_k`.
2.  **Backtracking**: Iteratively reduce `k_val` and adjust `pos` based on which segment of `S_{k_val}` it falls into, until `k_val` is 0.

## Complexity Analysis

-   **Time Complexity**: $O(\log(\text{pos}))$
    The first phase, finding `k_val`, involves iteratively calculating `L_k`. Since `L_k` grows exponentially (roughly as $N \cdot 2^k$), the number of iterations to reach a `pos` of $10^{18}$ is logarithmic with respect to `pos`. For example, if $N=1$, $L_k \approx 2^k$. To reach $10^{18}$, $k$ is around 60.
    The second phase, backtracking, also reduces `k_val` by 1 in each step. The number of steps is equal to the `k_val` found in the first phase, which is also logarithmic with respect to `pos`.
    Therefore, the overall time complexity is dominated by these logarithmic operations.

-   **Space Complexity**: $O(1)$
    The solution uses a constant amount of extra space for variables like `N`, `len_S_k`, `k_val`, `len_S_prev`, and `relative_pos_in_reversed_part`. No auxiliary data structures that grow with the input size are used.

## Solution Code

```cpp
#include <string> // Required for std::string
#include <algorithm> // Required for std::reverse (though not directly used in the final logic, conceptually relevant)

class Solution {
public:
    /**
     * @brief Finds the character at a given 1-based position in an infinite string.
     *
     * The infinite string is constructed recursively:
     * S_0 = s_initial
     * S_k = S_{k-1} + ('$' repeated k times) + reverse(S_{k-1}) for k > 0.
     * The infinite string is the concatenation of S_0, S_1, S_2, ...
     *
     * @param s_initial The initial string.
     * @param pos The 1-based index of the character to find.
     * @return The character at the specified position.
     */
    char posInfinite(const std::string& s_initial, long long pos) {
        long long N = s_initial.size();

        // Base case: If pos is within the initial string S_0.
        if (pos <= N) {
            return s_initial[pos - 1];
        }

        // Phase 1: Find the smallest k_val such that pos falls within S_{k_val}.
        // L_k represents the length of S_k.
        // L_0 = N
        // L_k = 2 * L_{k-1} + k
        long long len_S_k = N; // Stores L_k, starting with L_0
        int k_val = 0;         // Stores k, starting with 0

        // Iterate to find the smallest k_val where pos <= L_{k_val}.
        // We start with k_val = 0, len_S_k = N (L_0).
        // If pos > L_0, we need to consider S_1.
        // L_1 = 2 * L_0 + 1.
        // If pos > L_1, we need to consider S_2.
        // L_2 = 2 * L_1 + 2.
        // And so on.
        while (pos > len_S_k) {
            k_val++;
            // Calculate the next length L_k = 2 * L_{k-1} + k.
            // We need to be mindful of potential `long long` overflow.
            // Since `pos` is at most 10^18, `len_S_k` will not exceed `pos` by a huge margin.
            // For `pos = 10^18`, `k_val` will be around 60.
            // `2 * len_S_k + k_val` will be roughly `2 * 10^18 + 60`, which fits within `long long`.
            // A more robust check for overflow if `pos` could be closer to `LLONG_MAX`:
            // if (len_S_k > (LLONG_MAX - k_val) / 2) { /* handle overflow, e.g., break or throw */ }
            len_S_k = 2 * len_S_k + k_val;
        }

        // Now we have:
        // `pos`: the 1-based index we are looking for, relative to the start of S_{k_val}.
        // `len_S_k`: the length of the string S_{k_val}.
        // `k_val`: the step number such that pos <= L_{k_val}.
        // We know `pos <= len_S_k`.

        // Phase 2: Iteratively trace back from S_{k_val} to S_0.
        // The loop continues as long as we are looking within S_k where k > 0.
        while (k_val > 0) {
            // S_{k_val} is constructed as:
            // Part 1: S_{k_val-1} (length len_S_prev)
            // Part 2: ('$' repeated k_val times) (length k_val)
            // Part 3: reverse(S_{k_val-1}) (length len_S_prev)

            // Calculate the length of S_{k_val-1} using the relation:
            // L_k = 2 * L_{k-1} + k  =>  L_{k-1} = (L_k - k) / 2
            long long len_S_prev = (len_S_k - k_val) / 2;

            // Determine which part `pos` falls into within S_{k_val}:

            // Part 1: The first S_{k_val-1} segment.
            if (pos <= len_S_prev) {
                // `pos` is in the first S_{k_val-1} part.
                // We update `len_S_k` to `len_S_prev` and `k_val` to `k_val-1`
                // to continue searching in the previous string S_{k_val-1}.
                len_S_k = len_S_prev;
                k_val--;
            }
            // Part 2: The '$' characters segment.
            else if (pos <= len_S_prev + k_val) {
                // `pos` falls within the sequence of `k_val` dollar signs.
                return '$';
            }
            // Part 3: The reversed S_{k_val-1} segment.
            else {
                // `pos` is in the reversed S_{k_val-1} part.
                // Calculate the 1-based position relative to the start of this reversed segment.
                long long relative_pos_in_reversed_part = pos - (len_S_prev + k_val);
                
                // Map this relative position back to the corresponding 1-based index
                // in the original (unreversed) S_{k_val-1} string.
                // If relative_pos_in_reversed_part is 1, it corresponds to the last char of S_{k_val-1}.
                // If relative_pos_in_reversed_part is len_S_prev, it corresponds to the first char of S_{k_val-1}.
                // The mapping is: new_pos = len_S_prev - relative_pos_in_reversed_part + 1.
                pos = len_S_prev - relative_pos_in_reversed_part + 1;
                
                // Update `len_S_k` and `k_val` to continue searching in S_{k_val-1}.
                len_S_k = len_S_prev;
                k_val--;
            }
        }

        // If the loop finishes, `k_val` is 0.
        // This means `pos` has been reduced to an index within the initial string `s_initial` (S_0).
        // The initial `if (pos <= N)` check handles cases where pos is initially within S_0.
        // For cases where pos was initially greater than N, this final `pos` will be
        // guaranteed to be `1 <= pos <= N`.
        return s_initial[pos - 1];
    }
};
```