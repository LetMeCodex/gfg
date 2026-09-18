# [Minimum Absolute Difference In BST](https://www.geeksforgeeks.org/problems/minimum-absolute-difference-in-bst-1665139652/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 4 attempt(s)

## Problem Summary
Given a Binary Search Tree (BST), the task is to find the minimum absolute difference between the values of any two nodes in the tree. Since it is a BST, the difference between any two nodes is minimized when we compare nodes that are adjacent in the sorted order.

## Intuition & Approach
The key property of a Binary Search Tree is that its **inorder traversal** produces values in non-decreasing (sorted) order. 

1. **Inorder Traversal**: By performing an inorder traversal (Left -> Root -> Right), we visit the nodes in ascending order.
2. **Tracking the Previous Node**: As we traverse, we maintain a pointer `prev` to the node visited immediately before the current node.
3. **Calculating Difference**: For every node we visit, we calculate the difference `root->data - prev->data`. Since the traversal is sorted, this difference will always be non-negative.
4. **Updating Minimum**: We keep track of the global minimum difference found so far using a variable `minDiff`.
5. **Efficiency**: This approach allows us to find the result in a single pass without needing to store all node values in an auxiliary array, saving space.

## Complexity Analysis
- **Time Complexity**: $O(N)$, where $N$ is the number of nodes in the BST. We visit each node exactly once during the inorder traversal.
- **Space Complexity**: $O(H)$, where $H$ is the height of the tree. This space is consumed by the recursion stack. In the worst case (a skewed tree), $H = N$; in a balanced tree, $H = \log N$.

## Solution Code

```cpp
#include <algorithm>
#include <climits>

/*
// Definition for a binary tree node.
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
    int minDiff;
    Node* prev;

    void inorder(Node* root) {
        if (!root) {
            return;
        }

        // Traverse left subtree
        inorder(root->left);

        // Process current node: calculate difference with the previous node in inorder traversal
        if (prev != nullptr) {
            minDiff = std::min(minDiff, root->data - prev->data);
        }
        prev = root;

        // Traverse right subtree
        inorder(root->right);
    }

    int absDiff(Node* root) {
        minDiff = INT_MAX;
        prev = nullptr;
        inorder(root);
        return minDiff;
    }
};
```