#include <climits> // Required for INT_MAX
#include <cmath>   // Required for std::abs
#include <algorithm> // Required for std::min

// The TreeNode struct definition provided in your attempt should be removed
// as the GeeksforGeeks judge uses a 'Node' struct.
// The 'Node' struct is typically provided by the online judge environment.
// For reference, it would look something like this (but you don't include it):
/*
struct Node {
    int data; // This is the common name for the value member in GFG's Node struct
    Node *left;
    Node *right;
    Node() : data(0), left(nullptr), right(nullptr) {}
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
    Node(int x, Node *left, Node *right) : data(x), left(left), right(right) {}
};
*/

class Solution {
public:
    /**
     * @brief Finds the minimum absolute difference between k and the value of any node in the BST.
     *
     * This function leverages the properties of a Binary Search Tree (BST) to efficiently
     * find the node whose value is closest to k. It performs a modified search similar
     * to finding a specific value in a BST.
     *
     * @param root The root of the Binary Search Tree (expected to be of type Node* by GFG).
     * @param k The integer value to find the closest node to.
     * @return The minimum absolute difference between k and any node's value in the BST.
     *
     * Time Complexity: O(H), where H is the height of the BST. In the worst case (skewed tree),
     *                  H can be N (number of nodes), making it O(N). In the best/average case
     *                  (balanced tree), H is log N, making it O(log N).
     * Space Complexity: O(1), as it uses an iterative approach with a few constant extra variables.
     */
    int minDiff(Node* root, int k) { // Changed TreeNode* to Node*
        int min_abs_diff = INT_MAX; // Initialize with the largest possible integer value

        // Traverse the BST iteratively
        while (root != nullptr) {
            // Calculate the absolute difference between the current node's value and k
            // CORRECTED: Changed root->val to root->data
            int current_diff = std::abs(root->data - k);

            // Update the minimum absolute difference found so far
            min_abs_diff = std::min(min_abs_diff, current_diff);

            // If the minimum difference becomes 0, it means k itself is present in the BST.
            // This is the smallest possible difference, so we can return immediately.
            if (min_abs_diff == 0) {
                return 0;
            }

            // Decide whether to move to the left or right subtree based on k's value
            // CORRECTED: Changed root->val to root->data
            if (k < root->data) {
                // If k is smaller than the current node's value,
                // the closest value (if not found yet) must be in the left subtree.
                root = root->left;
            } else { // k >= root->data (Note: if k == root->data, we would have returned 0 already)
                // If k is larger than or equal to the current node's value,
                // the closest value (if not found yet) must be in the right subtree.
                root = root->right;
            }
        }

        // After traversing the path to k (or where k would be), min_abs_diff holds the answer.
        return min_abs_diff;
    }
};