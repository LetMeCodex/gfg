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