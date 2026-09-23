#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /**
     * The cost to form a pyramid with peak at i and height x is:
     * Total sum of arr - (sum of pyramid elements).
     * The pyramid elements are 1, 2, ..., x, ..., 1, which sum to x^2.
     * We need to find the max x for each i such that arr[i-k] >= x-k and arr[i+k] >= x-k.
     */
    long long formPyramid(vector<int>& arr) {
        int n = arr.size();
        long long total_sum = 0;
        for (int x : arr) total_sum += x;

        // Precompute L[i]: max height of a non-decreasing sequence ending at i
        // L[i] = min(arr[i], L[i-1] + 1)
        vector<int> L(n);
        L[0] = 1;
        for (int i = 1; i < n; ++i) {
            L[i] = min(arr[i], L[i - 1] + 1);
        }

        // Precompute R[i]: max height of a non-increasing sequence starting at i
        // R[i] = min(arr[i], R[i+1] + 1)
        vector<int> R(n);
        R[n - 1] = 1;
        for (int i = n - 2; i >= 0; --i) {
            R[i] = min(arr[i], R[i + 1] + 1);
        }

        // The max height x at peak i is min(L[i], R[i])
        long long max_pyramid_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long x = min(L[i], R[i]);
            max_pyramid_sum = max(max_pyramid_sum, x * x);
        }

        return total_sum - max_pyramid_sum;
    }
};