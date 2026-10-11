#include <algorithm> // Required for std::swap

class Solution {
public:
    /**
     * @brief Finds the minimum number of squares that can be formed by cutting a rectangular paper.
     * Each cut must be made completely across the remaining paper, either horizontally or vertically.
     * This problem follows a greedy strategy similar to the Euclidean algorithm for GCD.
     * We repeatedly cut out the largest possible square(s) from the current rectangle.
     *
     * @param n The length of one side of the rectangular paper.
     * @param m The length of the other side of the rectangular paper.
     * @return The minimum number of squares.
     */
    long long minSquares(long long n, long long m) {
        // Base case 1: If either dimension is 0, no paper is left, so 0 squares are formed.
        if (n == 0 || m == 0) {
            return 0;
        }
        
        // Base case 2: If both dimensions are equal, the paper is already a square.
        // We need 1 square (the paper itself).
        if (n == m) {
            return 1;
        }
        
        // To simplify the logic, ensure 'n' is always the smaller dimension.
        // This allows us to consistently think about cutting 'n x n' squares.
        if (n > m) {
            std::swap(n, m);
        }
        
        // Calculate how many squares of size 'n x n' can be cut from the 'm' side.
        // This is equivalent to integer division: m / n.
        long long num_n_by_n_squares = m / n;
        
        // After cutting 'num_n_by_n_squares' squares of size 'n x n',
        // the remaining paper will have dimensions 'n x (m % n)'.
        // The total number of squares is the sum of the squares just cut
        // and the squares obtained from the remaining piece of paper.
        return num_n_by_n_squares + minSquares(n, m % n);
    }
};