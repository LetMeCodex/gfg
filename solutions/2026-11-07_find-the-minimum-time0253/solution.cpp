#include <algorithm>

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
        // The problem can be solved by considering how many documents each scanner
        // scans. Let's say scanner 1 scans 'i' documents and scanner 2 scans 'n-i' documents.
        // The time taken would be max(i * s1, (n-i) * s2).
        // We want to minimize this value over all possible values of 'i' from 0 to n.

        // However, a simpler observation is that we can always assign documents
        // to the faster scanner as much as possible.
        // If we assign all 'n' documents to scanner 1, the time is n * s1.
        // If we assign all 'n' documents to scanner 2, the time is n * s2.
        // The minimum time will be at least the minimum of these two.

        // Consider the case where we use both scanners.
        // If scanner 1 scans 'i' documents and scanner 2 scans 'n-i' documents,
        // the total time is determined by the scanner that finishes last.
        // This time is max(i * s1, (n-i) * s2).

        // Let's analyze the function f(i) = max(i * s1, (n-i) * s2).
        // We are looking for min(f(i)) for i in [0, n].
        // The function i * s1 is increasing with i.
        // The function (n-i) * s2 is decreasing with i.
        // The minimum of max(A, B) often occurs when A is close to B.
        // So, we are looking for an 'i' such that i * s1 is approximately equal to (n-i) * s2.
        // i * s1 = n * s2 - i * s2
        // i * (s1 + s2) = n * s2
        // i = (n * s2) / (s1 + s2)

        // Since 'i' must be an integer, we can check values of 'i' around this ideal value.
        // However, a more direct approach is to realize that the optimal strategy
        // will involve assigning documents such that the completion times are as close as possible.
        // The minimum time will be achieved when one scanner is busy for some time 'T',
        // and the other scanner is also busy for at most time 'T'.

        // If scanner 1 scans 'k' documents, it takes k * s1 time.
        // If scanner 2 scans 'n-k' documents, it takes (n-k) * s2 time.
        // The total time is max(k * s1, (n-k) * s2).

        // We can iterate through all possible number of documents assigned to scanner 1 (from 0 to n).
        // For each 'i', calculate the time taken if scanner 1 scans 'i' documents and scanner 2 scans 'n-i' documents.
        // The time taken is max(i * s1, (n-i) * s2).
        // We want to find the minimum of these values.

        long long min_total_time = -1;

        for (int i = 0; i <= n; ++i) {
            long long time_scanner1 = (long long)i * s1;
            long long time_scanner2 = (long long)(n - i) * s2;
            long long current_total_time = std::max(time_scanner1, time_scanner2);

            if (min_total_time == -1 || current_total_time < min_total_time) {
                min_total_time = current_total_time;
            }
        }

        return min_total_time;
    }
};