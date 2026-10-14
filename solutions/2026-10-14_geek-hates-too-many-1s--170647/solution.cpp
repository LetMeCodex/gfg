class Solution {
public:
    int maxValue(int n) {
        int ans = 0; // Initialize the result to 0

        // Iterate from the most significant bit (31 for a 32-bit integer) down to 0
        for (int j = 31; j >= 0; --j) {
            // Check if the j-th bit of the original number 'n' is set
            if ((n >> j) & 1) {
                // If the j-th bit of 'n' is 1, we consider setting it in 'ans'.
                // We can only set it if it doesn't create three consecutive set bits.

                // Check if the (j+1)-th bit in 'ans' is set
                // (j + 1 <= 31) ensures we don't try to shift beyond integer limits,
                // though for standard int types, shifting by >= bit width results in 0.
                bool prev_bit_is_set = (j + 1 <= 31) && ((ans >> (j + 1)) & 1);

                // Check if the (j+2)-th bit in 'ans' is set
                bool prev_prev_bit_is_set = (j + 2 <= 31) && ((ans >> (j + 2)) & 1);

                // If both the (j+1)-th and (j+2)-th bits in 'ans' are set,
                // then setting the j-th bit would create '111'.
                if (prev_bit_is_set && prev_prev_bit_is_set) {
                    // In this case, we must set the j-th bit to 0.
                    // Since 'ans' is initialized to 0 and we only set bits when allowed,
                    // doing nothing here effectively keeps the j-th bit as 0.
                } else {
                    // Otherwise, it's safe to set the j-th bit to 1.
                    ans |= (1 << j); // THIS IS THE PROBLEM LINE (now correctly placed)
                }
            }
            // If the j-th bit of 'n' is 0, we cannot change it to 1 (problem constraint).
            // So, the j-th bit of 'ans' remains 0. No action needed.
        }
        return ans;
    }
};