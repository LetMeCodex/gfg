# [Generate Usernames](https://www.geeksforgeeks.org/problems/new-registration-system--150419/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to process a list of desired usernames (referred to as "first names") and generate a list of actual assigned usernames according to specific rules. The rules are:

1.  **Order Matters**: Usernames must be assigned in the order they appear in the input list.
2.  **Unchanged if Available**: If a desired username (e.g., "foo") has not been assigned to any previous user, it should be assigned as is.
3.  **Suffix if Taken**: If a desired username (e.g., "foo") has already been assigned to a previous user, we must append the smallest positive integer (starting from 1) to form a new username (e.g., "foo1", "foo2", etc.).
4.  **Suffix Restriction**: This newly generated suffixed username (e.g., "foo1") must satisfy two conditions:
    *   It must not have been assigned to any previous user.
    *   It must *not* be present in the *initial list of usernames* (the original input `arr`). This means if "foo1" was one of the original desired names, we cannot use it as a generated suffix for "foo".

We need to return the list of final assigned usernames.

## Intuition & Approach

The core of this problem lies in efficiently tracking which usernames are available and what the next appropriate suffix should be for a given base name. We need to manage three distinct sets of information:

1.  **Original Reserved Names (`initial_reserved`)**: We need to know all unique names that were present in the *initial input list*. This is crucial for rule 4, which states that a *suffixed* username cannot be one of these original names. An `std::unordered_set<std::string>` is perfect for this, allowing $O(L)$ average-time lookups (where $L$ is string length).

2.  **Currently Assigned Usernames (`assigned_usernames`)**: As we process the input names one by one, we need to keep track of all usernames that have *already been successfully assigned* to previous users. This is vital for rules 2 and 3, ensuring we don't assign the same username twice. Another `std::unordered_set<std::string>` serves this purpose well.

3.  **Next Suffix Counter (`suffix_counts`)**: For each base name (e.g., "foo"), we need to know what the *next integer suffix to try* is. If "foo" was requested, and "foo" is taken, we try "foo1". If "foo1" is also taken, we try "foo2", and so on. An `std::unordered_map<std::string, int>` can store this, mapping a base name to its next available suffix number. For example, `suffix_counts["foo"] = 3` would mean the next attempt for "foo" should be "foo3".

**Algorithm Steps:**

1.  **Pre-processing `initial_reserved`**: Iterate through the input `arr` once and populate `initial_reserved` with all unique names. This takes $O(N \cdot L_{avg})$ time.
2.  **Initialize Data Structures**: Create empty `assigned_usernames` (an `unordered_set`) and `suffix_counts` (an `unordered_map`).
3.  **Iterate and Assign**: Loop through each `current_name` in the input `arr` in order:
    *   Retrieve the current suffix counter `k` for `current_name` from `suffix_counts`. If `current_name` is not yet in the map, `k` will default to 0 (meaning we first try the original name without a suffix).
    *   Enter a `while(true)` loop to find the smallest valid username:
        *   **Generate Candidate**:
            *   If `k` is 0, the `candidate_username` is `current_name` itself.
            *   If `k` is greater than 0, the `candidate_username` is `current_name` + `std::to_string(k)`.
        *   **Check Availability (Rule 2 & 3)**: Check if `candidate_username` is *already present* in `assigned_usernames`.
            *   If it *is* present, this candidate is taken. Increment `k` and continue to the next iteration of the `while` loop.
        *   **Check Suffix Restriction (Rule 4)**: If the candidate is *not* in `assigned_usernames`, we then apply the suffix restriction:
            *   If `k` is 0 (we are trying the original name), this name is valid because it's not assigned yet. The problem statement implies original names can be used even if they are in `initial_reserved`, as long as they haven't been assigned *yet*.
            *   If `k` is greater than 0 (we are trying a suffixed name), this name is valid *only if* it is *not present* in `initial_reserved`.
            *   If both conditions (not in `assigned_usernames` AND satisfies suffix restriction) are met, we have found our valid username.
        *   **Assign and Update**:
            *   Add `candidate_username` to our `result` vector.
            *   Insert `candidate_username` into `assigned_usernames`.
            *   Update `suffix_counts[current_name]` to `k + 1` (preparing for the next time this base name is requested).
            *   `break` out of the `while` loop to process the next `current_name` from the input `arr`.
        *   If the candidate was not valid (either already assigned or a forbidden suffixed name), increment `k` and try the next suffix.

This approach ensures that we always find the smallest valid username by systematically trying suffixes (or no suffix) and checking against all necessary conditions.

## Complexity Analysis

Let $N$ be the number of names in the input `arr`, and $L_{max}$ be the maximum length of a username.

-   **Time Complexity**: $O(N \cdot (L_{max} + \log N))$ on average.
    *   **Populating `initial_reserved`**: This involves iterating through $N$ strings and inserting them into an `unordered_set`. Each insertion takes $O(L_{avg})$ time on average, where $L_{avg}$ is the average length of a name. So, $O(N \cdot L_{avg})$.
    *   **Main Loop**: We iterate $N$ times, once for each `current_name`.
    *   **Inside the Loop**: For each `current_name`, we might enter a `while` loop to find a suitable suffix.
        *   String concatenation (`std::to_string(k)` and `+`): The length of `k` is $O(\log k)$. The maximum value of `k` can be up to $N$ (if all $N$ input names are the same). So, generating a candidate string takes $O(L_{max} + \log N)$.
        *   `unordered_set::count` and `unordered_set::insert`: These operations take $O(\text{length of string})$ on average. The length of the candidate string is $O(L_{max} + \log N)$.
        *   `unordered_map` access/insert: These also take $O(\text{length of key})$ on average, which is $O(L_{max})$.
    *   **Total Operations**: Each `current_name` will eventually find a valid username. The total number of times a `candidate_username` is generated and checked across all $N$ input names is bounded. For each distinct base name, the `k` counter increments. The sum of all `k` values (representing the number of attempts for each base name) that lead to a successful assignment is at most $N$. Therefore, the total number of `unordered_set` and `unordered_map` operations (insertions and lookups) is proportional to $N$.
    *   Combining these, the average time complexity is $O(N \cdot (L_{max} + \log N))$. In the worst-case scenario for hash collisions, `unordered_set`/`map` operations could degrade to $O(L \cdot N)$, leading to $O(N^2 \cdot (L_{max} + \log N))$, but this is rare in practice and not expected for typical competitive programming problems.

-   **Space Complexity**: $O(N \cdot (L_{max} + \log N))$.
    *   `result`: Stores $N$ strings. Each string can have a maximum length of $L_{max}$ (base name) + $O(\log N)$ (for the suffix). So, $O(N \cdot (L_{max} + \log N))$.
    *   `initial_reserved`: Stores up to $N$ unique strings, each up to $L_{max}$ length. So, $O(N \cdot L_{max})$.
    *   `assigned_usernames`: Stores up to $N$ unique strings, each up to $L_{max} + O(\log N)$ length. So, $O(N \cdot (L_{max} + \log N))$.
    *   `suffix_counts`: Stores up to $D$ entries (where $D \le N$ is the number of distinct base names). Each key is a string up to $L_{max}$ length. So, $O(D \cdot L_{max})$.
    *   The dominant term is $O(N \cdot (L_{max} + \log N))$.

## Solution Code

```cpp
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm> // For std::to_string in some compilers, though usually in <string>

class Solution {
public:
    std::vector<std::string> generateUsernames(std::vector<std::string>& arr) {
        std::vector<std::string> result;
        // Pre-allocate memory for the result vector to avoid reallocations,
        // which can improve performance for large inputs.
        result.reserve(arr.size());

        // initial_reserved: A set containing all unique usernames present in the input array 'arr'.
        // These names are considered "reserved" and cannot be used as *newly generated* suffixed usernames.
        // However, an original name (without suffix) can be assigned if it hasn't been assigned yet.
        std::unordered_set<std::string> initial_reserved;
        for (const std::string& name : arr) {
            initial_reserved.insert(name);
        }

        // assigned_usernames: A set containing all usernames that have been successfully assigned
        // to users *so far* during the processing. This prevents assigning the same username twice.
        std::unordered_set<std::string> assigned_usernames;

        // suffix_counts: A map to keep track of the next integer suffix to try for each base name.
        // For example, if suffix_counts["foo"] is 3, it means the next attempt for a username
        // based on "foo" should start by trying "foo3".
        std::unordered_map<std::string, int> suffix_counts;

        // Iterate through each first name in the input array 'arr' in order.
        for (const std::string& current_name : arr) {
            // Get the current suffix counter for this base name.
            // If current_name is not yet in suffix_counts, it defaults to 0.
            int k = suffix_counts[current_name];
            std::string candidate_username;

            // Loop to find the smallest available suffix (or no suffix)
            while (true) {
                // Construct the candidate username:
                // If k is 0, try the original name.
                // If k > 0, append the integer suffix.
                if (k == 0) {
                    candidate_username = current_name;
                } else {
                    candidate_username = current_name + std::to_string(k);
                }

                // Check if the candidate username is already assigned to a previous user.
                if (assigned_usernames.count(candidate_username) == 0) {
                    // The candidate is not currently assigned. Now check reservation rules.

                    // If k == 0, we are trying the original name. The problem states:
                    // "If a username has not already been assigned to any previous user, keep it unchanged."
                    // This implies the original name can be used even if it's in initial_reserved,
                    // as long as it's not *already assigned*.
                    // If k > 0, we are generating a suffixed name. This newly generated name
                    // *must not* be in the initial_reserved set.
                    if (k == 0 || initial_reserved.count(candidate_username) == 0) {
                        // This candidate username is valid and available.
                        result.push_back(candidate_username);
                        assigned_usernames.insert(candidate_username);
                        suffix_counts[current_name] = k + 1; // Prepare for the next request for this base name
                        break; // Found a username for current_name, move to the next input name.
                    }
                }
                
                // If the candidate username was either:
                // 1. Already in assigned_usernames (meaning it was used by a previous user), OR
                // 2. A suffixed name (k > 0) that is present in initial_reserved (globally forbidden for new suffixes),
                // then this candidate is not available. Increment k and try the next suffix.
                k++;
            }
        }

        return result;
    }
};
```