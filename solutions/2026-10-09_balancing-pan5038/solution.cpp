class Solution {
public:
    /**
     * @brief Determines if a target weight 'b' can be balanced on a scale using distinct powers of 'a'.
     *
     * The problem asks if the equation `b + (sum of some powers of a) = (sum of some other powers of a)`
     * can be satisfied. This can be rearranged to `b = (sum of some other powers of a) - (sum of some powers of a)`.
     *
     * This implies that 'b' must be representable as a sum `sum(c_i * a^i)`, where each `c_i` can be:
     * - `0`: if `a^i` is not used.
     * - `1`: if `a^i` is placed on the opposite pan (contributing `+a^i` to 'b's side).
     * - `-1`: if `a^i` is placed on the same pan as 'b' (contributing `-a^i` to 'b's side).
     *
     * We can determine the coefficients `c_i` greedily, starting from `c_0` (for `a^0`).
     * Consider the equation `b = c_0 * a^0 + c_1 * a^1 + c_2 * a^2 + ...`
     * Taking this equation modulo `a`, we get `b % a = c_0 % a`.
     * Since `c_0` can only be `-1`, `0`, or `1`:
     *   - If `c_0 = 0`, then `b % a` must be `0`.
     *   - If `c_0 = 1`, then `b % a` must be `1`.
     *   - If `c_0 = -1`, then `b % a` must be `-1 % a`, which is equivalent to `(a-1) % a`.
     *
     * Based on the remainder `b % a`, we make a choice for `c_0` and update `b` for the next iteration:
     * 1. If `b % a == 0`: We choose `c_0 = 0`. The remaining value to balance for higher powers is `b / a`.
     * 2. If `b % a == 1`: We choose `c_0 = 1`. The remaining value to balance is `(b - 1) / a`.
     * 3. If `b % a == a - 1`: We choose `c_0 = -1`. The remaining value to balance is `(b + 1) / a`.
     * 4. If `b % a` is any other value (not `0`, `1`, or `a-1`), it's impossible to satisfy the condition,
     *    so we return `false`.
     *
     * We repeat this process until `b` becomes `0`. If `b` reaches `0`, it means we have successfully
     * represented the original target weight using the allowed coefficients.
     *
     * @param a The base for the powers of weights (2 <= a <= 10^9).
     * @param b The target weight to balance (1 <= b <= 10^9).
     * @return true if 'b' can be balanced, false otherwise.
     */
    bool balancePan(long long a, long long b) {
        // Loop until b becomes 0.
        // In each iteration, 'b' represents the remaining value to be balanced,
        // and we determine the coefficient for the current power of 'a' (a^0, then a^1, etc.).
        while (b > 0) {
            long long remainder = b % a;

            if (remainder == 0) {
                // If remainder is 0, we don't need a^k for this position (coefficient 0).
                // Update b to consider the next higher power: b = b / a.
                b /= a;
            } else if (remainder == 1) {
                // If remainder is 1, we use a^k with coefficient +1.
                // This means b = 1 * a^k + a * (remaining_value).
                // So, (b - 1) must be divisible by 'a'.
                // Update b: b = (b - 1) / a.
                b = (b - 1) / a;
            } else if (remainder == a - 1) {
                // If remainder is a-1, we use a^k with coefficient -1.
                // This means b = -1 * a^k + a * (remaining_value).
                // So, (b + 1) must be divisible by 'a'.
                // Update b: b = (b + 1) / a.
                b = (b + 1) / a;
            } else {
                // If the remainder is anything else (e.g., 2 when a=4), it's impossible
                // to choose a coefficient from {-1, 0, 1} to match the remainder.
                return false;
            }
        }

        // If the loop completes, 'b' has been successfully reduced to 0,
        // indicating that the original target weight can be balanced.
        return true;
    }
};