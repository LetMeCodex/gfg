#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

struct Box {
    long long h, w, l;
};

bool compareBoxes(const Box& a, const Box& b) {
    return (a.w * a.l) > (b.w * b.l);
}

class Solution {
public:
    long long maxHeight(vector<int>& height, vector<int>& width, vector<int>& length) {
        vector<Box> all_orientations;
        int n = height.size();

        for (int i = 0; i < n; ++i) {
            long long h = height[i];
            long long w = width[i];
            long long l = length[i];

            // Orientation 1: h as height, w and l as base
            all_orientations.push_back({h, min(w, l), max(w, l)});
            // Orientation 2: w as height, h and l as base
            all_orientations.push_back({w, min(h, l), max(h, l)});
            // Orientation 3: l as height, h and w as base
            all_orientations.push_back({l, min(h, w), max(h, w)});
        }

        // Sort boxes by base area in descending order
        sort(all_orientations.begin(), all_orientations.end(), compareBoxes);

        int num_orientations = all_orientations.size();
        vector<long long> dp(num_orientations);
        long long max_h = 0;

        for (int i = 0; i < num_orientations; ++i) {
            dp[i] = all_orientations[i].h; // Initialize with the height of the current box
            for (int j = 0; j < i; ++j) {
                // Check if box i can be placed on top of box j
                if (all_orientations[i].w < all_orientations[j].w && all_orientations[i].l < all_orientations[j].l) {
                    dp[i] = max(dp[i], dp[j] + all_orientations[i].h);
                }
            }
            max_h = max(max_h, dp[i]);
        }

        return max_h;
    }
};