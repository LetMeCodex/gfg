#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    long long minTime(vector<int>& duration, vector<vector<int>>& dependencies) {
        int n = duration.size();
        vector<vector<int>> adj(n);
        vector<int> in_degree(n, 0);

        for (const auto& dep : dependencies) {
            int u = dep[0];
            int v = dep[1];
            adj[u].push_back(v);
            in_degree[v]++;
        }

        queue<int> q;
        // completion_time[i] stores the maximum time taken to complete module i,
        // considering all its prerequisite modules and their durations.
        vector<long long> completion_time(n, 0);

        for (int i = 0; i < n; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
                // For modules with no dependencies, their completion time is just their own duration.
                completion_time[i] = duration[i];
            }
        }

        int count = 0; // To count the number of modules processed (for cycle detection)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            count++;

            for (int v : adj[u]) {
                // The time to complete module v, if it were to start after module u,
                // would be completion_time[u] + duration[v].
                // We need to take the maximum of all such paths leading to v,
                // because v can only start after ALL its prerequisites are done.
                completion_time[v] = max(completion_time[v], completion_time[u] + duration[v]);
                
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        // If count is not equal to n, it means not all modules could be processed,
        // indicating a cycle in the dependency graph.
        if (count != n) {
            return -1; // Cycle detected
        }

        // The minimum time to complete the entire project is the maximum
        // completion time among all modules, as the project is finished
        // only when the last module is completed.
        long long max_completion_time = 0;
        for (long long time : completion_time) {
            max_completion_time = max(max_completion_time, time);
        }

        return max_completion_time;
    }
};