# [Max Path Sum Between Two Leaves](https://www.geeksforgfg.org/problems/maximum-path-sum/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to find the maximum path sum between any two leaf nodes in a given binary tree. A "path" here means a sequence of nodes where each adjacent pair is connected by an edge. The path must start at one leaf node, traverse upwards through ancestor nodes, and then descend to another leaf node.

**Key Constraints/Conditions:**
1.  The path must connect exactly two *leaf* nodes.
2.  If no such path exists (e.g., the tree is empty, has only one node, or is a skewed tree with only one leaf), we should return -1.
3.  Node values can be negative.

## Intuition & Approach

This problem is a classic application of Depth First Search (DFS) or recursion on trees. The core idea is to process the tree from the bottom up, calculating relevant information at each node and using it to update a global maximum.

**What information do we need from children?**
For any given node `root`, if we want to form a path between two leaves *passing through* `root`, `root` must have both a left child and a right child. The path would then consist of:
1.  A path from a leaf in the left subtree up to `root->left`.
2.  The `root->data` itself.
3.  A path from a leaf in the right subtree up to `root->right`.

So, the recursive helper function `solve(Node* root)` needs to return the maximum path sum from *a leaf in its subtree* up to the `root` itself. This value is what its parent node would use to form a path.

**Detailed Approach:**

1.  **Global Variable for Maximum Sum**: We'll use a `long long max_sum` initialized to `LLONG_MIN` (a very small value) to store the overall maximum path sum found between any two leaves. This variable will be updated whenever a potential path between two leaves is identified.

2.  **Recursive Helper Function `solve(Node* root)`**:
    *   **Base Case**: If `root` is `NULL`, it cannot contribute to any path. We return `LLONG_MIN` to signify that no valid path from a leaf can be formed through this empty branch. This ensures that `std::max` operations involving this value will correctly ignore it unless no other option is available.
    *   **Leaf Node Case**: If `root` is a leaf node (i.e., `!root->left && !root->right`), the maximum path sum from a leaf in its subtree to itself is simply `root->data`. We return this value.
    *   **Recursive Calls**:
        *   Call `solve(root->left)` to get `left_sum`. This `left_sum` represents the maximum path sum from a leaf in the left subtree up to `root->left`.
        *   Call `solve(root->right)` to get `right_sum`. This `right_sum` represents the maximum path sum from a leaf in the right subtree up to `root->right`.
    *   **Update `max_sum` and Return Value**:
        *   **If `root` has both left and right children (`root->left && root->right`)**:
            *   This is the only scenario where `root` can be an intermediate node for a path between two leaves (one in its left subtree, one in its right subtree).
            *   The path sum would be `left_sum + right_sum + root->data`. We update our global `max_sum`: `max_sum = std::max(max_sum, left_sum + right_sum + root->data);`.
            *   For its parent, `root` can only extend one path upwards. So, it should return the maximum of `(left_sum + root->data)` and `(right_sum + root->data)`.
            *   `return std::max(left_sum, right_sum) + root->data;`
        *   **If `root` has only one child (either left or right)**:
            *   `root` cannot form a path between two leaves because it doesn't have two branches leading to leaves. Thus, `max_sum` is *not* updated here.
            *   `root` still needs to return the maximum path sum from a leaf in its subtree up to itself for its parent.
            *   If `root->left` exists: `return left_sum + root->data;`
            *   If `root->right` exists: `return right_sum + root->data;`
            *   The `LLONG_MIN` from the non-existent child's `solve` call ensures that `std::max` would correctly pick the path through the existing child if we were to use `std::max(left_sum, right_sum) + root->data;` here as well. The provided code handles this by explicitly checking which child exists.

3.  **Main Function `maxPathSum(Node* root)`**:
    *   Initialize `max_sum = LLONG_MIN`.
    *   **Edge Case 1**: If the tree is empty (`!root`) or has only one node (`!root->left && !root->right`), there are no two leaves. Return -1 as per problem statement.
    *   Call `solve(root)` to start the DFS. The return value of `solve(root)` is not directly used for the final answer, as `max_sum` stores the global maximum.
    *   **Edge Case 2**: After the DFS, if `max_sum` is still `LLONG_MIN`, it means no node ever had both left and right children (e.g., a skewed tree). In this case, no path between two leaves exists. Return -1.
    *   Otherwise, return `(int)max_sum`. We use `long long` for `max_sum` to prevent overflow, as path sums can exceed `INT_MAX` even if individual node values are `int`. The final cast to `int` is done because the problem asks for an `int` return type.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   The `solve` function visits each node in the binary tree exactly once.
    *   At each node, a constant number of operations (comparisons, additions, `std::max` calls) are performed.
    *   Therefore, the total time complexity is directly proportional to the number of nodes, $N$.

*   **Space Complexity**: $O(H)$
    *   This complexity arises from the recursion stack used by the DFS.
    *   In the worst-case scenario (a skewed tree, resembling a linked list), the height $H$ can be equal to $N$ (the number of nodes).
    *   In the best-case scenario (a perfectly balanced tree), the height $H$ is $log N$.
    *   Thus, the space complexity is $O(H)$, where $H$ is the height of the binary tree.

## Solution Code

```cpp
#include <algorithm> // Required for std::max
#include <climits>   // Required for LLONG_MIN

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
    long long max_sum; // Global variable to store the maximum path sum found

    /**
     * Helper function to calculate the maximum path sum starting from a leaf
     * and going up to the current node.
     * 
     * @param root Current node
     * @return The maximum path sum from a leaf in the subtree to the current node.
     *         Returns LLONG_MIN if no leaf exists in the subtree (e.g., NULL node).
     */
    long long solve(Node* root) {
        // Base case: If the node is NULL, it cannot contribute to any path.
        // Return a very small value to ensure it's not chosen by std::max
        // unless absolutely necessary (e.g., no other valid path).
        if (!root) return LLONG_MIN;

        // Leaf node case: If it's a leaf, the max path from a leaf in its subtree
        // to itself is just its own data.
        if (!root->left && !root->right) {
            return root->data;
        }

        // Recursively calculate max path sums from leaves in left and right subtrees
        // up to their respective roots.
        long long left_sum = solve(root->left);
        long long right_sum = solve(root->right);

        // If both children exist, we can form a path between two leaves
        // passing through the current 'root' node.
        if (root->left && root->right) {
            // Update the global maximum path sum.
            // This path goes from a leaf in left subtree -> root -> leaf in right subtree.
            max_sum = std::max(max_sum, left_sum + right_sum + root->data);
            
            // For the parent of 'root', 'root' can only extend one path upwards.
            // Choose the path that yields a greater sum: through left child or right child.
            return std::max(left_sum, right_sum) + root->data;
        }

        // If only one child exists, we cannot form a path between two leaves at this node.
        // We still need to return the max path from a leaf in its subtree up to 'root'
        // for its parent.
        return (root->left) ? (left_sum + root->data) : (right_sum + root->data);
    }

public:
    /**
     * Finds the maximum path sum between any two leaf nodes.
     * Time Complexity: O(N) where N is the number of nodes.
     * Space Complexity: O(H) where H is the height of the tree for recursion stack.
     */
    int maxPathSum(Node* root) {
        max_sum = LLONG_MIN; // Initialize global max_sum to a very small value

        // Edge case: If tree is empty or has only one node, there are no two leaves.
        if (!root || (!root->left && !root->right)) return -1;

        // Start the recursive DFS. The return value of solve(root) is not directly
        // the answer, as max_sum tracks the global maximum.
        long long result = solve(root); // 'result' here is the max path from a leaf to root itself

        // If max_sum was never updated, it means there was no node with both left and right children.
        // This implies no path between two leaves exists (e.g., a skewed tree).
        if (max_sum == LLONG_MIN) return -1;

        // Cast to int as the problem asks for an int return type.
        return (int)max_sum;
    }
};
```