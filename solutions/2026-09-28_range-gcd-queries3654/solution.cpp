#include <vector>
// #include <numeric> // No need to include this if the judge provides gcd or if we're not using std::gcd directly.

// Remove the custom gcd function definition.
// The judge's environment will either provide one or you can assume std::gcd is available.
// For competitive programming, it's safer to assume the judge provides it if it's a common utility.
// If not, and std::gcd is not C++17, you'd typically implement it inside the class or as a static helper.
// Given the error, it's definitely provided by the judge.

class Solution {
private:
    std::vector<int> tree;
    // std::vector<int> arr_ref; // No need to store a reference copy of arr, as updates only affect the segment tree.
    int n;

    // Forward declaration of gcd if it's not a global function provided by the judge
    // or if we want to use std::gcd.
    // For this problem, we'll assume the judge's global gcd is available.
    // If not, you'd put a static helper like:
    // static int gcd(int a, int b) { ... }
    // or use std::gcd if C++17 is guaranteed.

    // Assuming the judge provides 'gcd' function globally.
    // If not, and you want to be self-contained, you can define it as a static member:
    static int gcd(int a, int b) {
        while (b) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }


    void build(int node, int start, int end, const std::vector<int>& arr) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = start + (end - start) / 2; // Safer way to calculate mid to prevent overflow
            build(2 * node, start, mid, arr);
            build(2 * node + 1, mid + 1, end, arr);
            tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
        }
    }

    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            // arr_ref[idx] = val; // No longer needed if arr_ref is removed
        } else {
            int mid = start + (end - start) / 2;
            if (start <= idx && idx <= mid) {
                update(2 * node, start, mid, idx, val);
            } else {
                update(2 * node + 1, mid + 1, end, idx, val);
            }
            tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
        }
    }

    int query(int node, int start, int end, int l, int r) {
        // If the current segment is completely outside the query range
        if (r < start || end < l) {
            return 0; // Identity element for GCD is 0 (gcd(x, 0) = x)
        }
        // If the current segment is completely inside the query range
        if (l <= start && end <= r) {
            return tree[node];
        }
        // If the current segment partially overlaps, recurse
        int mid = start + (end - start) / 2;
        int p1 = query(2 * node, start, mid, l, r);
        int p2 = query(2 * node + 1, mid + 1, end, l, r);
        return gcd(p1, p2);
    }

public:
    // Renamed the method from query_gcd to processQueries as expected by the judge.
    std::vector<int> processQueries(std::vector<int>& arr, std::vector<std::vector<int>>& queries) {
        // arr_ref = arr; // No longer needed
        n = arr.size();
        tree.resize(4 * n); // A segment tree typically needs 4*N space

        // Pass arr to build function
        build(1, 0, n - 1, arr);

        std::vector<int> results;
        for (const auto& query_item : queries) {
            if (query_item[0] == 0) { // Type 0 query: Range GCD
                int l = query_item[1];
                int r = query_item[2];
                results.push_back(query(1, 0, n - 1, l, r));
            } else { // Type 1 query: Update
                int index = query_item[1];
                int value = query_item[2];
                update(1, 0, n - 1, index, value);
            }
        }
        return results;
    }
};