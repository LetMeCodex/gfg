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