# [Minimum Time with 2 Scanners](https://www.geeksforgeeks.org/problems/find-the-minimum-time0253/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to find the minimum time required to scan `n` documents using two scanners. Scanner 1 takes `s1` time to scan one document, and scanner 2 takes `s2` time to scan one document. We can distribute the `n` documents between the two scanners in any way.

## Intuition & Approach

The core idea is to figure out how to distribute the `n` documents between the two scanners to minimize the overall time. Let's say scanner 1 scans `i` documents and scanner 2 scans `n-i` documents.

The time taken by scanner 1 to scan `i` documents is `i * s1`.
The time taken by scanner 2 to scan `n-i` documents is `(n-i) * s2`.

Since both scanners can work in parallel, the total time taken to scan all `n` documents will be determined by the scanner that finishes *last*. Therefore, for a given distribution of `i` documents to scanner 1 and `n-i` to scanner 2, the total time is `max(i * s1, (n-i) * s2)`.

Our goal is to find the minimum value of this `max` function over all possible values of `i`, where `i` can range from `0` (scanner 1 scans 0 documents, scanner 2 scans all `n`) to `n` (scanner 1 scans all `n` documents, scanner 2 scans 0).

This leads to a straightforward approach: iterate through all possible values of `i` from `0` to `n`. For each `i`, calculate the time taken by scanner 1 (`i * s1`) and scanner 2 (`(n-i) * s2`). The total time for this distribution is the maximum of these two values. We keep track of the minimum total time encountered so far and update it whenever we find a smaller time.

**Example:**
If `n = 5`, `s1 = 2`, `s2 = 3`:
- `i = 0`: Scanner 1 scans 0, Scanner 2 scans 5. Time = `max(0*2, 5*3) = max(0, 15) = 15`.
- `i = 1`: Scanner 1 scans 1, Scanner 2 scans 4. Time = `max(1*2, 4*3) = max(2, 12) = 12`.
- `i = 2`: Scanner 1 scans 2, Scanner 2 scans 3. Time = `max(2*2, 3*3) = max(4, 9) = 9`.
- `i = 3`: Scanner 1 scans 3, Scanner 2 scans 2. Time = `max(3*2, 2*3) = max(6, 6) = 6`.
- `i = 4`: Scanner 1 scans 4, Scanner 2 scans 1. Time = `max(4*2, 1*3) = max(8, 3) = 8`.
- `i = 5`: Scanner 1 scans 5, Scanner 2 scans 0. Time = `max(5*2, 0*3) = max(10, 0) = 10`.

The minimum time is 6.

The problem statement mentions "Easy" difficulty, and this brute-force iteration over all possible distributions is efficient enough for the given constraints. We need to use `long long` for time calculations to avoid potential integer overflow, especially when `n`, `s1`, or `s2` are large.

## Complexity Analysis

- **Time Complexity**: $O(n)$
  We iterate through all possible values of `i` from `0` to `n`. For each `i`, we perform a constant number of arithmetic operations and a `max` comparison. Therefore, the time complexity is directly proportional to `n`.

- **Space Complexity**: $O(1)$
  We only use a few variables to store the current minimum time and intermediate calculations. The space used does not depend on the input size `n`.

## Solution Code

```cpp
#include <algorithm> // Required for std::max

class Solution {
public:
    /**
     * @brief Finds the minimum time required to scan all n documents using two scanners.
     *
     * @param n The number of documents to scan.
     * @param s1 The time taken by scanner 1 to scan one document.
     * @param s2 The time taken by scanner 2 to scan one document.
     * @return The minimum time required to scan all n documents.
     */
    long long minTime(int n, int s1, int s2) {
        // Initialize min_total_time to a value that will be easily replaced by the first calculated time.
        // Using -1 as a sentinel value or a very large number would also work.
        long long min_total_time = -1;

        // Iterate through all possible distributions of documents.
        // 'i' represents the number of documents scanned by scanner 1.
        // Consequently, 'n - i' documents are scanned by scanner 2.
        for (int i = 0; i <= n; ++i) {
            // Calculate the time taken by scanner 1.
            // Cast to long long to prevent potential overflow if n * s1 is large.
            long long time_scanner1 = (long long)i * s1;

            // Calculate the time taken by scanner 2.
            // Cast to long long to prevent potential overflow if (n - i) * s2 is large.
            long long time_scanner2 = (long long)(n - i) * s2;

            // The total time for this distribution is the maximum of the two scanner times,
            // as both scanners work in parallel and the process finishes when the last scanner is done.
            long long current_total_time = std::max(time_scanner1, time_scanner2);

            // Update the minimum total time if the current distribution results in a shorter time.
            // The first iteration will always set min_total_time because it's initialized to -1.
            if (min_total_time == -1 || current_total_time < min_total_time) {
                min_total_time = current_total_time;
            }
        }

        // Return the overall minimum time found.
        return min_total_time;
    }
};
```