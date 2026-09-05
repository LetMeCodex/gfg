#include <vector>

class Solution {
public:
    /**
     * Calculates the sum of bitwise AND for all pairs (i, j) where i < j.
     * Logic: For each bit position 'b', if 'k' numbers have the 'b'-th bit set,
     * then there are k * (k - 1) / 2 pairs that will have the 'b'-th bit set in their AND result.
     * Each such pair contributes 2^b to the total sum.
     */
    long long pairAndSum(std::vector<int>& arr) {
        long long totalSum = 0;
        int n = arr.size();

        // Iterate through each bit position (0 to 30, since arr[i] <= 10^8 < 2^30)
        for (int bit = 0; bit < 31; ++bit) {
            long long countSetBits = 0;
            
            // Count how many numbers in the array have the current bit set
            for (int x : arr) {
                if ((x >> bit) & 1) {
                    countSetBits++;
                }
            }

            // If 'k' numbers have the 'bit'-th bit set, then there are k * (k - 1) / 2 pairs
            // where both numbers have the 'bit'-th bit set.
            // For each such pair, the 'bit'-th bit contributes 2^bit to the sum of ANDs.
            if (countSetBits > 1) {
                long long pairs = (countSetBits * (countSetBits - 1)) / 2;
                totalSum += pairs * (1LL << bit);
            }
        }

        return totalSum;
    }
};