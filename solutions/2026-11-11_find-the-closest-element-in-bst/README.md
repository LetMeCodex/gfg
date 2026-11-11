Here's a high-quality GitHub solution writeup for the "Closest in BST" problem:

---

# [Closest in BST](https://www.geeksforgfg.org/problems/find-the-closest-element-in-bst/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to find the minimum absolute difference between a given target integer `k` and the value of any node present in a Binary Search Tree (BST). We need to return this minimum difference. For example, if `k = 12` and the BST contains `10, 15, 5`, the closest node to `12` is `10` (difference `|10-12|=2`) or `15` (difference `|15-12|=3`). The minimum difference would be `2`.

## Intuition & Approach

The core idea behind solving this problem efficiently lies in leveraging the fundamental property of a Binary Search Tree (BST): for any given node, all values in its left subtree are strictly smaller than the node's value, and all values in its right subtree are strictly greater.

This property allows us to perform a targeted search similar to finding an element in a BST. Instead of just looking for `k`, we are looking for the node whose value is *closest* to `k`.

Here's the step-by-step approach:

1.  **Initialization**:
    *   Initialize a variable `min_abs_diff` to `INT_MAX` (the largest possible integer value, from `<climits>`). This variable will store the minimum absolute difference found so far.

2.  **Iterative Traversal**:
    *   We traverse the BST iteratively using a `while` loop, starting from the `root` and continuing as long as the current node (`root`) is not `nullptr`. This iterative approach avoids the overhead of recursion's call stack, leading to $O(1)$ space complexity.

3.  **Calculate Difference**:
    *   In each step, calculate the absolute difference between the current node's value (`root->data`) and the target `k`: `current_diff = std::abs(root->data - k)` (using `std::abs` from `<cmath>`).

4.  **Update Minimum**:
    *   Update `min_abs_diff` with the smaller of its current value and `current_diff`: `min_abs_diff = std::min(min_abs_diff, current_diff)` (using `std::min` from `<algorithm>`).

5.  **Early Exit Condition**:
    *   If `min_abs_diff` becomes `0` at any point, it means `k` itself is present in the BST. Since `0` is the smallest possible absolute difference, we can immediately return `0` as the answer, avoiding further unnecessary traversal.

6.  **Decide Next Move**:
    *   Based on the comparison of `k` with the current node's value (`root->data`), decide whether to move to the left or right subtree:
        *   If `k < root->data`: The target `k` is smaller than the current node's value. According to BST properties, the closest value (if not found yet) must reside in the left subtree. So, we move `root = root->left`.
        *   If `k > root->data`: The target `k` is larger than the current node's value. The closest value (if not found yet) must reside in the right subtree. So, we move `root = root->right`.
        *   (Note: If `k == root->data`, the `min_abs_diff` would have become 0, and we would have already returned in step 5.)

7.  **Return Result**:
    *   Once the `while` loop finishes (meaning `root` becomes `nullptr`, indicating we've reached the end of a path), `min_abs_diff` will hold the minimum absolute difference found along the path to `k` (or where `k` would have been inserted). Return `min_abs_diff`.

**Important Note on Node Structure**:
The GeeksforGeeks platform typically uses a `Node` struct with a member named `data` (e.g., `Node->data`) for the value, not `val` or `value`. The provided solution code correctly uses `root->data`.

## Complexity Analysis

*   **Time Complexity**: $O(H)$, where $H$ is the height of the BST.
    *   The algorithm traverses a single path from the root down to a leaf (or where the target `k` would be inserted). The length of this path is at most the height of the BST.
    *   In the worst-case scenario (a skewed tree, resembling a linked list), the height $H$ can be equal to $N$ (the number of nodes in the BST). In this case, the time complexity becomes $O(N)$.
    *   In the best or average-case scenario (a balanced BST), the height $H$ is approximately $\log N$. Thus, the time complexity would be $O(\log N)$.
    *   At each node along this path, we perform a constant number of operations (comparison, subtraction, absolute value, min update, pointer assignment).

*   **Space Complexity**: $O(1)$.
    *   The solution uses an iterative approach, which only requires a few constant extra variables (`min_abs_diff`, `current_diff`, and the `root` pointer itself). It does not use any auxiliary data structures (like a recursion stack or a queue) that grow with the input size.

## Solution Code

```cpp
#include <climits>   // Required for INT_MAX
#include <cmath>     // Required for std::abs
#include <algorithm> // Required for std::min

// The 'Node' struct is typically provided by the online judge environment.
// You do not need to include its definition in your submission.
// For reference, it commonly looks like this on GeeksforGeeks:
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
    int minDiff(Node* root, int k) {
        int min_abs_diff = INT_MAX; // Initialize with the largest possible integer value

        // Traverse the BST iteratively
        while (root != nullptr) {
            // Calculate the absolute difference between the current node's value and k
            int current_diff = std::abs(root->data - k);

            // Update the minimum absolute difference found so far
            min_abs_diff = std::min(min_abs_diff, current_diff);

            // If the minimum difference becomes 0, it means k itself is present in the BST.
            // This is the smallest possible difference, so we can return immediately.
            if (min_abs_diff == 0) {
                return 0;
            }

            // Decide whether to move to the left or right subtree based on k's value
            if (k < root->data) {
                // If k is smaller than the current node's value,
                // the closest value (if not found yet) must be in the left subtree.
                root = root->left;
            } else { // k >= root->data (Note: if k == root->data, we would have returned 0 already)
                // If k is larger than the current node's value,
                // the closest value (if not found yet) must be in the right subtree.
                root = root->right;
            }
        }

        // After traversing the path to k (or where k would be), min_abs_diff holds the answer.
        return min_abs_diff;
    }
};
```