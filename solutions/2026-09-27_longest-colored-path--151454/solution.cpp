#include <vector>
#include <string>
#include <algorithm> // For max_element, sort

using namespace std;

class Solution {
public:
    int n;
    vector<vector<int>> adj;
    string colors;

    // dp_R_down[u]: longest Red path starting at u and going into its subtree.
    // dp_B_down[u]: longest Blue path starting at u and going into its subtree.
    vector<int> dp_R_down;
    vector<int> dp_B_down;

    // dp_R_up[u]: longest Red path starting at u and going towards parent.
    // dp_B_up[u]: longest Blue path starting at u and going towards parent.
    vector<int> dp_R_up;
    vector<int> dp_B_up;

    // For each node u, store the 1st and 2nd longest (1 + dp_X_down[v]) for its children v.
    // This is used to calculate dp_X_up for children.
    // max1_X_child_len[u]: longest (1 + dp_X_down[v]) for child v of u.
    // max2_X_child_len[u]: second longest (1 + dp_X_down[v]) for child v of u.
    // child_X_with_max1[u]: the child v that gives max1_X_child_len[u].
    vector<int> max1_R_child_len;
    vector<int> max2_R_child_len;
    vector<int> child_R_with_max1;

    vector<int> max1_B_child_len;
    vector<int> max2_B_child_len;
    vector<int> child_B_with_max1;

    int overall_max_path_len;

    void dfs1(int u, int p) {
        dp_R_down[u] = (colors[u-1] == 'R' ? 1 : 0);
        dp_B_down[u] = (colors[u-1] == 'B' ? 1 : 0);

        // Store lengths of paths from children to find top two for diameter calculation
        // and for up-paths calculation in DFS2.
        int current_max1_R_child_len = 0, current_max2_R_child_len = 0;
        int current_child_R_with_max1 = 0; // Stores the child node ID
        int current_max1_B_child_len = 0, current_max2_B_child_len = 0;
        int current_child_B_with_max1 = 0; // Stores the child node ID

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);

            // Update dp_R_down[u] and dp_B_down[u]
            if (colors[u-1] == 'R' && colors[v-1] == 'R') {
                // Path from u down to v and into v's Red subtree
                // Length is 1 (for u) + dp_R_down[v]
                dp_R_down[u] = max(dp_R_down[u], 1 + dp_R_down[v]);
                
                // Update top two for R children, for diameter calculation
                if (1 + dp_R_down[v] > current_max1_R_child_len) {
                    current_max2_R_child_len = current_max1_R_child_len;
                    current_max1_R_child_len = 1 + dp_R_down[v];
                    current_child_R_with_max1 = v;
                } else if (1 + dp_R_down[v] > current_max2_R_child_len) {
                    current_max2_R_child_len = 1 + dp_R_down[v];
                }
            }
            if (colors[u-1] == 'B' && colors[v-1] == 'B') {
                // Path from u down to v and into v's Blue subtree
                // Length is 1 (for u) + dp_B_down[v]
                dp_B_down[u] = max(dp_B_down[u], 1 + dp_B_down[v]);
                
                // Update top two for B children, for diameter calculation
                if (1 + dp_B_down[v] > current_max1_B_child_len) {
                    current_max2_B_child_len = current_max1_B_child_len;
                    current_max1_B_child_len = 1 + dp_B_down[v];
                    current_child_B_with_max1 = v;
                } else if (1 + dp_B_down[v] > current_max2_B_child_len) {
                    current_max2_B_child_len = 1 + dp_B_down[v];
                }
            }
        }

        max1_R_child_len[u] = current_max1_R_child_len;
        max2_R_child_len[u] = current_max2_R_child_len;
        child_R_with_max1[u] = current_child_R_with_max1;

        max1_B_child_len[u] = current_max1_B_child_len;
        max2_B_child_len[u] = current_max2_B_child_len;
        child_B_with_max1[u] = current_child_B_with_max1;

        // Update overall_max_path_len for single-color paths (diameter)
        // A single-color path can pass through u, using two branches from children,
        // or just one branch (which is covered by dp_X_down[u] itself).
        if (colors[u-1] == 'R') {
            if (current_max1_R_child_len > 0 && current_max2_R_child_len > 0) {
                // Path is (child1_path -> u -> child2_path)
                // Length = (1 + dp_R_down[v1]) + (1 + dp_R_down[v2]) - 1 (for u)
                // = current_max1_R_child_len + current_max2_R_child_len - 1
                overall_max_path_len = max(overall_max_path_len, current_max1_R_child_len + current_max2_R_child_len - 1);
            }
            // The case where path only goes down one branch (or no branches) is covered by dp_R_down[u]
            overall_max_path_len = max(overall_max_path_len, dp_R_down[u]);
        }
        if (colors[u-1] == 'B') {
            if (current_max1_B_child_len > 0 && current_max2_B_child_len > 0) {
                overall_max_path_len = max(overall_max_path_len, current_max1_B_child_len + current_max2_B_child_len - 1);
            }
            overall_max_path_len = max(overall_max_path_len, dp_B_down[u]);
        }
    }

    void dfs2(int u, int p) {
        // Initialize dp_X_up[u] to 1 if u is of that color, representing path of just u.
        dp_R_up[u] = (colors[u-1] == 'R' ? 1 : 0);
        dp_B_up[u] = (colors[u-1] == 'B' ? 1 : 0);

        if (p != 0) { // If u is not root
            if (colors[u-1] == 'R' && colors[p-1] == 'R') {
                // Calculate longest Red path from p that does NOT go to u.
                // This path can go up from p (dp_R_up[p]) or down to another child of p.
                int val_from_p = dp_R_up[p]; 
                if (child_R_with_max1[p] == u) { // If u was the child that gave max1_R_child_len[p]
                    val_from_p = max(val_from_p, max2_R_child_len[p]);
                } else {
                    val_from_p = max(val_from_p, max1_R_child_len[p]);
                }
                if (val_from_p > 0) { // Only extend if parent path exists (val_from_p includes p itself)
                    dp_R_up[u] = max(dp_R_up[u], 1 + val_from_p);
                }
            }
            if (colors[u-1] == 'B' && colors[p-1] == 'B') {
                // Calculate longest Blue path from p that does NOT go to u.
                int val_from_p = dp_B_up[p];
                if (child_B_with_max1[p] == u) {
                    val_from_p = max(val_from_p, max2_B_child_len[p]);
                } else {
                    val_from_p = max(val_from_p, max1_B_child_len[p]);
                }
                if (val_from_p > 0) { // Only extend if parent path exists
                    dp_B_up[u] = max(dp_B_up[u], 1 + val_from_p);
                }
            }
        }
        
        // Update overall_max_path_len for mixed paths (R...R -> B...B)
        // A mixed path must involve an edge (u, v) where one is the last Red and other is first Blue.
        // We iterate over all neighbors v of u (which could be parent or children).
        // For DFS2, v is a child of u. The parent is p.
        // The logic below iterates over children v.
        for (int v : adj[u]) {
            if (v == p) continue; // v is a child of u

            // Case 1: Path is (Red path ending at u) -> (Blue path starting at v)
            // This means u is Red, v is Blue.
            // Longest Red path from u not going to v:
            // This path can go up from u (dp_R_up[u]) or down to another child of u (max_R_child_len_excluding_v).
            int len_R_from_u_away_from_v = 0;
            if (colors[u-1] == 'R') {
                len_R_from_u_away_from_v = dp_R_up[u]; // Path going up from u
                if (child_R_with_max1[u] == v) { // If v was the child that gave max1_R_child_len[u]
                    len_R_from_u_away_from_v = max(len_R_from_u_away_from_v, max2_R_child_len[u]);
                } else {
                    len_R_from_u_away_from_v = max(len_R_from_u_away_from_v, max1_R_child_len[u]);
                }
            }

            // Longest Blue path from v not going to u:
            // This path must go down from v into its subtree. This is dp_B_down[v].
            int len_B_from_v_away_from_u = 0;
            if (colors[v-1] == 'B') {
                len_B_from_v_away_from_u = dp_B_down[v]; // Path going down from v
            }

            if (colors[u-1] == 'R' && colors[v-1] == 'B') {
                // Only update if both parts exist (i.e., u is Red and v is Blue)
                if (len_R_from_u_away_from_v > 0 && len_B_from_v_away_from_u > 0) {
                    overall_max_path_len = max(overall_max_path_len, len_R_from_u_away_from_v + len_B_from_v_away_from_u);
                }
            }

            // Case 2: Path is (Red path ending at v) -> (Blue path starting at u)
            // This means v is Red, u is Blue.
            // Longest Red path from v not going to u:
            // This path must go down from v into its subtree. This is dp_R_down[v].
            int len_R_from_v_away_from_u = 0;
            if (colors[v-1] == 'R') {
                len_R_from_v_away_from_u = dp_R_down[v]; // Path going down from v
            }

            // Longest Blue path from u not going to v:
            // This path can go up from u (dp_B_up[u]) or down to another child of u (max_B_child_len_excluding_v).
            int len_B_from_u_away_from_v = 0;
            if (colors[u-1] == 'B') {
                len_B_from_u_away_from_v = dp_B_up[u]; // Path going up from u
                if (child_B_with_max1[u] == v) {
                    len_B_from_u_away_from_v = max(len_B_from_u_away_from_v, max2_B_child_len[u]);
                } else {
                    len_B_from_u_away_from_v = max(len_B_from_u_away_from_v, max1_B_child_len[u]);
                }
            }

            if (colors[u-1] == 'B' && colors[v-1] == 'R') {
                // Only update if both parts exist
                if (len_R_from_v_away_from_u > 0 && len_B_from_u_away_from_v > 0) {
                    overall_max_path_len = max(overall_max_path_len, len_R_from_v_away_from_u + len_B_from_u_away_from_v);
                }
            }
        }

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs2(v, u);
        }
    }

    // Renamed function and adjusted parameters
    int longestPath(string s, vector<vector<int>>& edges) {
        n = s.length(); // Initialize n from string length
        colors = s;
        adj.assign(n + 1, vector<int>()); // Use assign to clear and resize
        dp_R_down.assign(n + 1, 0);
        dp_B_down.assign(n + 1, 0);
        dp_R_up.assign(n + 1, 0);
        dp_B_up.assign(n + 1, 0);
        max1_R_child_len.assign(n + 1, 0);
        max2_R_child_len.assign(n + 1, 0);
        child_R_with_max1.assign(n + 1, 0);
        max1_B_child_len.assign(n + 1, 0);
        max2_B_child_len.assign(n + 1, 0);
        child_B_with_max1.assign(n + 1, 0);

        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }

        overall_max_path_len = 0;
        
        // A single node is a path of length 1. If n >= 1, there's always a path of length 1.
        if (n > 0) { // Constraints state N >= 1, so n will always be > 0.
            overall_max_path_len = 1;
        }
        
        // Start DFS from node 1, parent 0 (dummy). Assuming graph is connected.
        // The problem states "undirected acyclic graph (tree)", so it's connected.
        dfs1(1, 0); 
        dfs2(1, 0);

        return overall_max_path_len;
    }
};