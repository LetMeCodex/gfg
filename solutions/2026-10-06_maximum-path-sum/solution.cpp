#include <algorithm>
#include <climits>

/**
 * Definition for a binary tree node.
 * struct Node {
 *     int data;
 *     Node *left;
 *     Node *right;
 *     Node(int val) {
 *         data = val;
 *         left = right = NULL;
 *     }
 * };
 */

class Solution {
    long long max_sum;

    /**
     * Helper function to calculate the maximum path sum starting from a leaf
     * and going up to the current node.
     * 
     * @param root Current node
     * @return The maximum path sum from a leaf in the subtree to the current node.
     *         Returns a very small value if no leaf exists in the subtree.
     */
    long long solve(Node* root) {
        if (!root) return LLONG_MIN;

        // Leaf node case
        if (!root->left && !root->right) {
            return root->data;
        }

        long long left_sum = solve(root->left);
        long long right_sum = solve(root->right);

        // If both children exist, we can form a path between two leaves
        if (root->left && root->right) {
            max_sum = std::max(max_sum, left_sum + right_sum + root->data);
            return std::max(left_sum, right_sum) + root->data;
        }

        // If only one child exists, we cannot form a path between two leaves at this node
        return (root->left) ? (left_sum + root->data) : (right_sum + root->data);
    }

public:
    /**
     * Finds the maximum path sum between any two leaf nodes.
     * Time Complexity: O(N) where N is the number of nodes.
     * Space Complexity: O(H) where H is the height of the tree for recursion stack.
     */
    int maxPathSum(Node* root) {
        max_sum = LLONG_MIN;
        
        // Edge case: If tree is empty or has only one node
        if (!root || (!root->left && !root->right)) return -1;

        long long result = solve(root);

        // If max_sum was never updated, it means there was no path between two leaves
        // (e.g., a skewed tree where only one leaf exists)
        if (max_sum == LLONG_MIN) return -1;

        return (int)max_sum;
    }
};