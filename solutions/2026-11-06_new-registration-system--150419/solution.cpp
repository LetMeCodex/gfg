#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

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