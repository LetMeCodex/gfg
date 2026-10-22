#include <vector>

class Solution {
public:
    // Standard merge sort based inversion counting
    long long countInversions(std::vector<int>& arr, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        long long count = countInversions(arr, left, mid) + countInversions(arr, mid + 1, right);
        
        std::vector<int> temp;
        int i = left, j = mid + 1;
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp.push_back(arr[i++]);
            } else {
                // Inversion found: arr[i] > arr[j]
                // Since left part is sorted, all elements from i to mid are > arr[j]
                count += (mid - i + 1);
                temp.push_back(arr[j++]);
            }
        }
        while (i <= mid) temp.push_back(arr[i++]);
        while (j <= right) temp.push_back(arr[j++]);
        
        for (int k = 0; k < temp.size(); ++k) arr[left + k] = temp[k];
        
        return count;
    }

    // Helper to collect in-order traversal
    void inorder(Node* root, std::vector<int>& arr) {
        if (!root) return;
        inorder(root->left, arr);
        arr.push_back(root->data);
        inorder(root->right, arr);
    }

    int pairsViolatingBST(Node* root) {
        std::vector<int> arr;
        inorder(root, arr);
        if (arr.empty()) return 0;
        return static_cast<int>(countInversions(arr, 0, arr.size() - 1));
    }
};