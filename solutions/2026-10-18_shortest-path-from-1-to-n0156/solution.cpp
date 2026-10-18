class Solution {
public:
    int minimumStep(int n) {
        // Base case: if n is 1, we are already at the destination, 0 steps needed.
        if (n == 1) {
            return 0;
        }

        int steps = 0;
        // We work backwards from n to 1.
        // Each step in reverse (n/3 or n-1) corresponds to a step in forward (i*3 or i+1).
        // We want to reach 1 from n in minimum steps.
        // The greedy strategy is to divide by 3 whenever possible, as it reduces n much faster.
        // If n is not divisible by 3, we must subtract 1 until it becomes divisible by 3,
        // or until it becomes 1.
        while (n > 1) {
            if (n % 3 == 0) {
                // If n is divisible by 3, it's always optimal to divide by 3.
                // This corresponds to a 'current_num * 3' operation in forward.
                n /= 3;
            } else {
                // If n is not divisible by 3, we must subtract 1.
                // This corresponds to a 'current_num + 1' operation in forward.
                // Example: if n = 4 (n % 3 == 1), we do 4 -> 3.
                // Example: if n = 5 (n % 3 == 2), we do 5 -> 4. Then in next iteration, 4 -> 3.
                // This ensures we reach a multiple of 3 or 1.
                n -= 1;
            }
            steps++;
        }
        
        return steps; 
    }
};