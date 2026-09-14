# [Visit Leaves with Budget](https://www.geeksforgeeks.org/problems/leaf-under-budget/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary
Given a binary tree and a budget `k`, we need to find the maximum number of leaf nodes that can be visited. The cost to visit a leaf node is equal to its level (where the root is at level 1). We want to maximize the count of leaves visited such that the total cost does not exceed `k`.

## Intuition & Approach
To maximize the number of leaves visited within a fixed budget, we should always prioritize visiting the "cheapest" leaves first. This is a classic **Greedy** problem.

1.  **Traversal**: We perform a Depth First Search (DFS) to traverse the binary tree. During the traversal, we keep track of the current depth (level).
2.  **Collection**: Whenever we encounter a leaf node (a node with no left or right children), we store its level in a list (`leafCosts`).
3.  **Sorting**: Once we have collected the levels of all leaf nodes, we sort the `leafCosts` vector in ascending order.
4.  **Greedy Selection**: We iterate through the sorted list and keep adding the costs to a running total. We increment our count as long as the total cost remains less than or equal to `k`. Once adding the next leaf exceeds the budget, we stop.

## Complexity Analysis
- **Time Complexity**: $O(N \log N)$, where $N$ is the number of nodes in the tree. The DFS traversal takes $O(N)$ time, and sorting the leaf nodes takes $O(L \log L)$ where $L$ is the number of leaves ($L \le N$).
- **Space Complexity**: $O(N)$ in the worst case (a skewed tree) for the recursion stack and the vector storing leaf levels.

## Solution Code

```cpp
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
```