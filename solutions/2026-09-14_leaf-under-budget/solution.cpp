#include <vector>
#include <algorithm>

/*
Definition for a binary tree node.
struct Node {
    int data;
    Node *left;
    Node *right;
    Node(int val) {
        data = val;
        left = right = NULL;
    }
};
*/

class Solution {
public:
    // Helper function to perform DFS and collect leaf node levels
    void getLeafLevels(Node* root, int level, std::vector<int>& leafCosts) {
        if (!root) return;
        
        // Check if it's a leaf node
        if (!root->left && !root->right) {
            leafCosts.push_back(level);
            return;
        }
        
        // Recurse for children
        getLeafLevels(root->left, level + 1, leafCosts);
        getLeafLevels(root->right, level + 1, leafCosts);
    }

    // The problem statement implies the method should be named getCount
    // based on the compilation error.
    int getCount(Node* root, int k) {
        std::vector<int> leafCosts;
        
        // Traverse the tree to find all leaf nodes and their levels
        getLeafLevels(root, 1, leafCosts);
        
        // Sort costs to pick the cheapest leaves first (Greedy approach)
        std::sort(leafCosts.begin(), leafCosts.end());
        
        int count = 0;
        int currentCost = 0;
        
        for (int cost : leafCosts) {
            if (currentCost + cost <= k) {
                currentCost += cost;
                count++;
            } else {
                // Since costs are sorted, we cannot afford any more leaves
                break;
            }
        }
        
        return count;
    }
};