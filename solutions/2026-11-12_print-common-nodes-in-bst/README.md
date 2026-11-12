# Common Nodes in Two BSTs

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 4 attempt(s)

## Problem Summary

Given two Binary Search Trees (BSTs), the task is to find and print all the common nodes present in both BSTs. The output should be a sorted list of these common nodes.

## Intuition & Approach

The core idea behind solving this problem efficiently lies in the properties of Binary Search Trees. An in-order traversal of a BST visits the nodes in ascending sorted order. If we can obtain sorted lists of all nodes from both BSTs, finding the common elements becomes a standard problem of finding intersections in two sorted arrays.

Here's the step-by-step approach:

1.  **In-order Traversal**: We'll perform an in-order traversal on the first BST (`r1`) and store all its node values in a vector, let's call it `v1`. Since it's a BST, `v1` will be sorted in ascending order.
2.  **In-order Traversal (Second BST)**: Similarly, we'll perform an in-order traversal on the second BST (`r2`) and store all its node values in another vector, `v2`. `v2` will also be sorted in ascending order.
3.  **Two-Pointer Intersection**: Now that we have two sorted vectors (`v1` and `v2`), we can find their common elements using a two-pointer approach.
    *   Initialize two pointers, `i` for `v1` and `j` for `v2`, both starting at index 0.
    *   Iterate while both `i` and `j` are within the bounds of their respective vectors:
        *   If `v1[i]` is less than `v2[j]`, it means `v1[i]` is not present in `v2` at the current position or any subsequent position (since `v2` is sorted). So, increment `i`.
        *   If `v1[i]` is greater than `v2[j]`, it means `v2[j]` is not present in `v1` at the current position or any subsequent position. So, increment `j`.
        *   If `v1[i]` is equal to `v2[j]`, we've found a common node. Add `v1[i]` (or `v2[j]`) to our result list. Then, increment both `i` and `j` to look for the next potential common element.
4.  **Return Result**: The result list will contain all common nodes in sorted order.

The provided solution implements this exact strategy. The `inorderTraversal` helper function recursively traverses the BST and populates a vector. The `findCommon` function orchestrates the traversals and the two-pointer intersection logic.

**Note on Node Definition**: The GeeksforGeeks judge environment typically provides the `Node` structure definition. The commented-out `struct Node` in the solution is there for illustrative purposes and should not be uncommented to avoid redefinition errors.

## Complexity Analysis

*   **Time Complexity**: $O(N + M)$, where $N$ is the number of nodes in the first BST and $M$ is the number of nodes in the second BST.
    *   The in-order traversal of the first BST takes $O(N)$ time.
    *   The in-order traversal of the second BST takes $O(M)$ time.
    *   The two-pointer intersection of two sorted vectors of size $N$ and $M$ takes $O(\min(N, M))$ time in the worst case, but since we iterate through both vectors, it's effectively $O(N+M)$ as we advance at least one pointer in each step.
    *   Therefore, the dominant factor is the traversals, resulting in $O(N + M)$.

*   **Space Complexity**: $O(N + M)$
    *   We use two vectors, `v1` and `v2`, to store the node values from each BST. In the worst case, these vectors will store all $N$ and $M$ nodes, respectively.
    *   The recursion depth for the in-order traversal can be $O(H_1)$ and $O(H_2)$, where $H_1$ and $H_2$ are the heights of the respective BSTs. In the worst case (a skewed tree), this can be $O(N)$ and $O(M)$. However, the space for storing the node values in vectors is usually considered the dominant factor for space complexity in this approach.
    *   The `result` vector can store up to $\min(N, M)$ common elements.
    *   Thus, the total space complexity is $O(N + M)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm>

// Definition for a binary tree node.
// The judge uses 'Node' instead of 'TreeNode'.
// It seems the driver code also defines a 'Node' struct/class.
// To avoid redefinition errors, we should assume the Node definition
// is provided by the driver and not redefine it here.
// If the driver *doesn't* provide it, then this definition would be needed.
// For this specific error, the driver *is* providing it, causing a redefinition.
/*
struct Node {
    int data;
    Node *left;
    Node *right;
    Node() : data(0), left(nullptr), right(nullptr) {}
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
    Node(int x, Node *left, Node *right) : data(x), left(left), right(right) {}
};
*/

class Solution {
public:
    // Helper function to perform in-order traversal and collect node values.
    // In-order traversal of a BST yields node values in sorted ascending order.
    // The judge uses 'Node' instead of 'TreeNode'.
    void inorderTraversal(Node* root, std::vector<int>& nodes) {
        if (!root) {
            return;
        }
        inorderTraversal(root->left, nodes);
        nodes.push_back(root->data);
        inorderTraversal(root->right, nodes);
    }

    // Main function to find common nodes in two BSTs.
    // The function name and parameter types must match the GFG driver code.
    // The judge uses 'Node*' for tree roots.
    std::vector<int> findCommon(Node* r1, Node* r2) {
        std::vector<int> v1; // Stores in-order traversal of BST1
        std::vector<int> v2; // Stores in-order traversal of BST2

        // Populate v1 with nodes from r1 in sorted order
        inorderTraversal(r1, v1);
        // Populate v2 with nodes from r2 in sorted order
        inorderTraversal(r2, v2);

        std::vector<int> result; // Stores the common nodes
        int i = 0; // Pointer for v1
        int j = 0; // Pointer for v2

        // Use a two-pointer approach to find common elements in two sorted vectors
        while (i < v1.size() && j < v2.size()) {
            if (v1[i] < v2[j]) {
                // Current element in v1 is smaller, move to the next in v1
                i++;
            } else if (v1[i] > v2[j]) {
                // Current element in v2 is smaller, move to the next in v2
                j++;
            } else { // v1[i] == v2[j]
                // Found a common element
                result.push_back(v1[i]);
                // Move to the next elements in both vectors
                i++;
                j++;
            }
        }

        return result;
    }
};
```