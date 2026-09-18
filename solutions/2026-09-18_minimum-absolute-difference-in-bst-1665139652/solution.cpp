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