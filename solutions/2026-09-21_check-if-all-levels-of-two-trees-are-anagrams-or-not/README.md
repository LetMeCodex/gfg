# [Check Level Anagrams in Binary Trees](https://www.geeksforgeeks.org/problems/check-if-all-levels-of-two-trees-are-anagrams-or-not/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to determine if two binary trees are "level anagrams". This means that for every level in the trees, the multiset of node values at that level in the first tree must be identical to the multiset of node values at the corresponding level in the second tree.

## Intuition & Approach

The core idea to solve this problem is to traverse both trees level by level and compare the node values at each corresponding level. A Breadth-First Search (BFS) is the natural choice for level-order traversal.

We can use two queues, one for each tree, to perform simultaneous BFS.

1.  **Initialization**:
    *   Push the root nodes of both trees into their respective queues.

2.  **Level-by-Level Comparison**:
    *   While both queues are not empty:
        *   Get the number of nodes at the current level for both trees (i.e., the current size of each queue).
        *   **Crucial Check**: If the number of nodes at the current level differs between the two trees, they cannot be level anagrams. Return `false` immediately.
        *   Create two frequency maps (e.g., `std::map` or `std::unordered_map`) to store the counts of node values for the current level of each tree.
        *   Iterate through all nodes at the current level for the first tree:
            *   Dequeue a node.
            *   Increment the count of its `data` in the first frequency map.
            *   Enqueue its left and right children (if they exist) for the next level.
        *   Iterate through all nodes at the current level for the second tree:
            *   Dequeue a node.
            *   Increment the count of its `data` in the second frequency map.
            *   Enqueue its left and right children (if they exist) for the next level.
        *   **Anagram Check**: After processing all nodes at the current level for both trees, compare the two frequency maps. If they are not identical (meaning the multisets of node values are different), return `false`.

3.  **Final Check**:
    *   If the loop completes without returning `false`, it means all levels processed so far were anagrams.
    *   The final condition to check is whether both trees have been fully traversed. If one queue is empty and the other is not, it implies one tree has more levels than the other, so they are not level anagrams. Therefore, return `q1.empty() && q2.empty()`.

The provided solution uses `std::map` for frequency counting. While `std::unordered_map` could offer better average time complexity, `std::map` is generally sufficient and guarantees ordered iteration, which is not strictly necessary here but doesn't hurt. The problem constraints (tree size up to $10^5$) make `std::map` a reasonable choice.

The initial thought process might involve just comparing the number of nodes at each level. However, this is insufficient. For example, two levels could have 3 nodes each, but the values could be `{1, 2, 3}` in one tree and `{1, 1, 4}` in the other. They have the same number of nodes but are not anagrams. Hence, frequency maps are essential.

The "3 attempt(s)" might indicate that an initial approach might have missed the frequency map comparison or the final check for equal number of levels. For instance, one might forget to check if both queues are empty at the end, or might incorrectly assume that if the number of nodes at each level matches, they are anagrams.

## Complexity Analysis

*   **Time Complexity**: $O(N \log K)$ or $O(N)$
    *   Let $N$ be the total number of nodes in the larger tree.
    *   We visit each node exactly once during the BFS traversal.
    *   For each level, we perform frequency map operations.
    *   If using `std::map`: Each insertion/lookup takes $O(\log K)$ time, where $K$ is the number of distinct node values at that level. In the worst case, $K$ can be up to the number of nodes at that level. Summing this over all levels, the total time complexity is roughly $O(N \log N)$ in the worst case if all nodes are distinct and at different levels, or more precisely, $O(\sum_{level} \text{nodes\_at\_level} \times \log(\text{distinct\_values\_at\_level}))$. A tighter bound considering the total number of nodes $N$ and the maximum number of distinct values $V$ across the entire tree would be $O(N \log V)$. If $V$ is bounded by $N$, it's $O(N \log N)$.
    *   If using `std::unordered_map`: The average time complexity for insertion/lookup is $O(1)$. Thus, the average time complexity for the entire traversal becomes $O(N)$. The worst-case for `unordered_map` can be $O(N^2)$ due to hash collisions, but this is rare in practice.
    *   Given the constraints and typical competitive programming scenarios, $O(N)$ with `unordered_map` or $O(N \log N)$ with `map` is expected. The provided solution uses `std::map`, so it's closer to $O(N \log N)$.

*   **Space Complexity**: $O(W)$ or $O(N)$
    *   $W$ is the maximum width of the binary tree. In the worst case (a complete binary tree), the width can be up to $N/2$.
    *   The queues store nodes for the current and next level. In the worst case, a queue can hold up to $O(W)$ nodes.
    *   The frequency maps store the counts of node values for a single level. In the worst case, a map can store up to $O(W)$ distinct values.
    *   Therefore, the space complexity is dominated by the queues and maps, which is $O(W)$. In the worst-case scenario for width (a complete binary tree), this becomes $O(N)$.

## Solution Code

```cpp
#include <queue> // Required for std::queue
#include <map>   // Required for std::map

// Definition for a binary tree node.
// This structure is typically provided by the platform (e.g., GeeksforGeeks).
// DO NOT redefine it here.
/*
struct Node {
    int data;
    Node *left;
    Node *right;
};
*/

class Solution {
public:
    // The GeeksforGeeks driver code expects a function named 'areAnagrams'.
    bool areAnagrams(Node* root1, Node* root2) {
        // According to constraints, 1 <= size of binary tree, so root1 and root2 will never be null initially.
        // However, if one tree is empty and the other is not, they cannot be level anagrams.
        // The problem statement implies non-empty roots, but a robust solution might check for null roots.
        // Given the constraints "1 <= size of binary tree", root1 and root2 will always be valid Node pointers.

        std::queue<Node*> q1;
        std::queue<Node*> q2;

        q1.push(root1);
        q2.push(root2);

        // Perform a level-order traversal (BFS) on both trees simultaneously.
        while (!q1.empty() && !q2.empty()) {
            int size1 = q1.size();
            int size2 = q2.size();

            // If the number of nodes at the current level is different,
            // they cannot be anagrams. Return false immediately.
            if (size1 != size2) {
                return false;
            }

            // Use frequency maps to store node values for the current level.
            // std::map is used for ordered key storage and O(log K) operations.
            // std::unordered_map could be used for O(1) average operations if performance is critical,
            // but std::map is generally sufficient for N=10^5.
            std::map<int, int> freq1;
            std::map<int, int> freq2;

            // Process all nodes at the current level for root1.
            for (int i = 0; i < size1; ++i) {
                Node* curr1 = q1.front();
                q1.pop();
                freq1[curr1->data]++; // Increment frequency of current node's data.

                // Enqueue children for the next level.
                if (curr1->left) {
                    q1.push(curr1->left);
                }
                if (curr1->right) {
                    q1.push(curr1->right);
                }
            }

            // Process all nodes at the current level for root2.
            for (int i = 0; i < size2; ++i) {
                Node* curr2 = q2.front();
                q2.pop();
                freq2[curr2->data]++; // Increment frequency of current node's data.

                // Enqueue children for the next level.
                if (curr2->left) {
                    q2.push(curr2->left);
                }
                if (curr2->right) {
                    q2.push(curr2->right);
                }
            }

            // After processing all nodes for the current level, compare their frequency maps.
            // If they are not identical, the levels are not anagrams.
            if (freq1 != freq2) {
                return false;
            }
        }

        // After the loop, if we haven't returned false, it means all levels processed
        // so far were anagrams. The final check is to ensure both trees have the
        // exact same number of levels. If one queue is empty and the other is not,
        // it implies one tree is deeper than the other, making them non-anagrams by level.
        return q1.empty() && q2.empty();
    }
};
```