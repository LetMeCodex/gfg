class Solution {
public:
    /**
     * @brief Calculates the bitwise XOR of all integers in the range [l, r].
     *
     * This function leverages a helper function `xor_up_to(n)` which calculates
     * the XOR of all integers from 0 to n. The XOR of the range [l, r] can then
     * be computed as `xor_up_to(r) ^ xor_up_to(l - 1)`.
     *
     * The `xor_up_to(n)` function has a pattern based on n % 4:
     * - If n % 4 == 0, the XOR sum is n.
     * - If n % 4 == 1, the XOR sum is 1.
     * - If n % 4 == 2, the XOR sum is n + 1.
     * - If n % 4 == 3, the XOR sum is 0.
     *
     * @param l The lower bound of the range (inclusive).
     * @param r The upper bound of the range (inclusive).
     * @return The bitwise XOR of all integers from l to r.
     */
    int findXOR(int l, int r) {
        return xor_up_to(r) ^ xor_up_to(l - 1);
    }

private:
    /**
     * @brief Calculates the bitwise XOR of all integers from 0 to n.
     *
     * @param n The upper bound of the range (inclusive).
     * @return The bitwise XOR of all integers from 0 to n.
     */
    int xor_up_to(int n) {
        switch (n % 4) {
            case 0: return n;
            case 1: return 1;
            case 2: return n + 1;
            case 3: return 0;
        }
        return 0; // Should not reach here
    }
};