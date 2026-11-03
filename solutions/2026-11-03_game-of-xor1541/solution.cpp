#include <vector>
#include <numeric>

class Solution {
public:
    // Renamed the method to match the driver code's expectation.
    int subarrayXor(std::vector<int>& arr) {
        int n = arr.size();
        int result = 0;

        // The logic for determining which elements contribute to the final XOR sum
        // is based on the number of subarrays each element appears in.
        // An element arr[i] appears in (i + 1) * (n - i) subarrays.
        // The final XOR sum is the XOR of all subarray XORs.
        //
        // Consider the contribution of arr[i] to the final XOR sum.
        // arr[i] will be XORed into the final result if it appears in an odd number of subarray XORs.
        //
        // Let's analyze the parity of (i + 1) * (n - i):
        //
        // Case 1: n is odd.
        // If i is even: (i + 1) is odd, (n - i) is odd (odd - even = odd). So, (i + 1) * (n - i) is odd.
        // If i is odd: (i + 1) is even, (n - i) is even (odd - odd = even). So, (i + 1) * (n - i) is even.
        // Therefore, if n is odd, elements at even indices (0, 2, 4, ...) contribute to the final XOR sum.
        //
        // Case 2: n is even.
        // If i is even: (i + 1) is odd, (n - i) is even (even - even = even). So, (i + 1) * (n - i) is even.
        // If i is odd: (i + 1) is even, (n - i) is odd (even - odd = odd). So, (i + 1) * (n - i) is even.
        // Therefore, if n is even, no element contributes to the final XOR sum, and the result is 0.

        if (n % 2 == 0) {
            // If the array size is even, each element appears in an even number of subarrays.
            // Thus, their contribution to the total XOR sum cancels out.
            return 0;
        } else {
            // If the array size is odd, elements at even indices appear in an odd number of subarrays.
            // We XOR these elements to get the final result.
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 0) { // Elements at even indices contribute when n is odd
                    result ^= arr[i];
                }
            }
            return result;
        }
    }
};