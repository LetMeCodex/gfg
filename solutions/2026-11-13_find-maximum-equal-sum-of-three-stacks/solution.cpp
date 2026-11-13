#include <vector>
#include <numeric> // For std::accumulate (optional, direct loop used)
#include <algorithm> // For std::max (optional, if-else chain used)

class Solution {
public:
    int maxEqualSum(std::vector<int>& s1, std::vector<int>& s2, std::vector<int>& s3) {
        // Calculate initial sums for each stack.
        // Using long long for sums to prevent potential overflow, although for the given constraints
        // (max sum 5 * 10^5 * 10^3 = 5 * 10^8), a standard int would also suffice.
        // It's a good habit in competitive programming.
        long long sum1 = 0;
        for (int x : s1) {
            sum1 += x;
        }

        long long sum2 = 0;
        for (int x : s2) {
            sum2 += x;
        }

        long long sum3 = 0;
        for (int x : s3) {
            sum3 += x;
        }
        
        // Pointers to keep track of the current top element of each stack.
        // These act as indices into the input vectors.
        int ptr1 = 0;
        int ptr2 = 0;
        int ptr3 = 0;

        // The main loop continues as long as the sums of the three stacks are not equal.
        while (!(sum1 == sum2 && sum2 == sum3)) {
            // Impossibility Check:
            // If any stack pointer reaches the end of its vector, it means that stack has become empty.
            // If the sums are still not equal at this point, it's impossible to make them equal
            // to a positive value. The only possible equal sum would be 0.
            // The problem asks for the maximum equal sum. If we can't achieve a positive
            // equal sum, 0 is the answer.
            // This condition correctly handles cases where one stack becomes empty while others
            // still have positive sums (e.g., s1=[4,7], s2=[10], s3=[1,2,3] from example).
            if (ptr1 == s1.size() || ptr2 == s2.size() || ptr3 == s3.size()) {
                return 0;
            }

            // Greedily remove the top element from the stack with the largest current sum.
            // This strategy aims to reduce the largest sum, bringing it closer to the others,
            // thus maximizing the potential equal sum.
            if (sum1 >= sum2 && sum1 >= sum3) {
                sum1 -= s1[ptr1];
                ptr1++;
            } else if (sum2 >= sum1 && sum2 >= sum3) {
                sum2 -= s2[ptr2];
                ptr2++;
            } else { // sum3 must be the largest
                sum3 -= s3[ptr3];
                ptr3++;
            }
        }

        // Once the loop terminates, all three sums are equal.
        // Return this common sum, cast back to int as per the function signature.
        return static_cast<int>(sum1);
    }
};