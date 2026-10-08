class Solution {
public:
    int minOperation(int n) { // Renamed from minOperations to minOperation to match the driver code.
        int operations = 0;
        // We work backward from n to 0.
        // The inverse operations are:
        // 1. Divide by 2 (if n is even)
        // 2. Subtract 1
        while (n > 0) {
            if (n % 2 == 0) {
                // If n is even, it's always optimal to divide by 2.
                // This is because dividing by 2 reduces the number much faster
                // than subtracting 1 (which would make it odd, forcing another subtract 1
                // to make it even again, taking 2 steps to reach n-2).
                n /= 2;
            } else {
                // If n is odd, we must subtract 1.
                // An odd number cannot be reached by doubling an integer.
                // The only way to reach an odd number 'n' is from 'n-1' by adding 1.
                // (This also handles the base case n=1, where n-1=0).
                n -= 1;
            }
            operations++;
        }
        return operations;
    }
};