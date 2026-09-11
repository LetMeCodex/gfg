#include <vector>
#include <algorithm>

class Solution {
public:
    long long maxProduct(std::vector<int>& arr, int k) {
        int n = arr.size();
        std::sort(arr.begin(), arr.end());

        // If all numbers are negative and k is odd, the result must be negative.
        // To maximize a negative product, we pick the k largest elements (closest to 0).
        if (arr[n - 1] < 0 && k % 2 != 0) {
            long long prod = 1;
            for (int i = 0; i < k; ++i) {
                prod *= arr[n - 1 - i];
            }
            return prod;
        }

        // General case:
        long long max_prod = 1;
        int left = 0;
        int right = n - 1;

        // If k is odd, pick the largest positive element to make k even.
        if (k % 2 != 0) {
            max_prod *= arr[right];
            right--;
            k--;
        }

        // Now k is even, pick pairs from either end.
        while (k > 0) {
            long long left_prod = (long long)arr[left] * arr[left + 1];
            long long right_prod = (long long)arr[right] * arr[right - 1];

            if (left_prod > right_prod) {
                max_prod *= left_prod;
                left += 2;
            } else {
                max_prod *= right_prod;
                right -= 2;
            }
            k -= 2;
        }

        return max_prod;
    }
};