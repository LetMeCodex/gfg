# [Count Pairs Violating BST Property](https://www.geeksforg eeks.org/problems/pairs-violating-bst-property--212515/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

Given the `root` of a binary tree, we need to find the number of pairs `(i, j)` such that `i` appears before `j` in the in-order traversal of the tree, but `data[i] > data[j]`. This essentially means we need to count the number of "inversions" in the in-order traversal of the given binary tree.

A Binary Search Tree (BST) property dictates that its in-order traversal should yield a sorted list of elements. If the given tree is not a BST, its in-order traversal will not be sorted, and the pairs `(i, j)` where `i` comes before `j` but `data[i] > data[j]` are precisely the pairs that "violate" this sorted property, hence "violating BST property".

## Intuition & Approach

### Intuition

The core of the problem lies in understanding what "pairs violating BST property" means in the context of a general binary tree.
1.  **BST Property and In-order Traversal**: A fundamental property of a Binary Search Tree (BST) is that an in-order traversal of its nodes yields a sequence of elements in non-decreasing (sorted) order.
2.  **Violation**: If a given binary tree is *not* a BST, its in-order traversal will *not* be sorted. A "violation" occurs when we encounter two elements `A` and `B` such that `A` appears before `B` in the in-order traversal, but `A > B`.
3.  **Inversions**: This definition (`A` appears before `B`, but `A > B`) is precisely the definition of an "inversion" in a sequence or array.

Therefore, the problem can be rephrased as:
1.  Perform an in-order traversal of the given binary tree to obtain a linear sequence (an array) of its node values.
2.  Count the number of inversions in this resulting array.

### Approach

The solution involves two main steps:

1.  **In-order Traversal**:
    *   We implement a standard recursive `inorder` traversal function.
    *   This function takes the current `Node* root` and a `std::vector<int>& arr` as arguments.
    *   It recursively visits the left child, then adds the current node's `data` to `arr`, and finally recursively visits the right child.
    *   After this step, `arr` will contain all node values of the tree in their in-order sequence.

2.  **Counting Inversions using Merge Sort**:
    *   The most efficient way to count inversions in an array is by modifying the Merge Sort algorithm. Merge Sort naturally divides the array into halves, sorts them, and then merges them. The key insight for counting inversions comes during the merge step.
    *   **`countInversions(std::vector<int>& arr, int left, int right)` function**:
        *   **Base Case**: If `left >= right`, the subarray has 0 or 1 element, so there are no inversions. Return `0`.
        *   **Divide**: Find the middle index `mid = left + (right - left) / 2`.
        *   **Conquer**: Recursively call `countInversions` for the left half (`arr[left...mid]`) and the right half (`arr[mid+1...right]`). Sum their results.
        *   **Combine (Merge and Count)**: This is where inversions are counted between the two sorted halves.
            *   Initialize two pointers: `i` for the left half (starting at `left`) and `j` for the right half (starting at `mid + 1`).
            *   Create a temporary `std::vector<int> temp` to store the merged sorted elements.
            *   While `i <= mid` and `j <= right`:
                *   If `arr[i] <= arr[j]`: The element `arr[i]` is not greater than `arr[j]`. Add `arr[i]` to `temp` and increment `i`.
                *   If `arr[i] > arr[j]`: An inversion is found! `arr[i]` is greater than `arr[j]`. Since both `arr[left...mid]` and `arr[mid+1...right]` are already sorted (from recursive calls), it implies that `arr[i]` (and all subsequent elements in the left half, i.e., `arr[i+1]` to `arr[mid]`) are also greater than `arr[j]`. The number of such elements is `(mid - i + 1)`. Add this count to the total inversions. Then, add `arr[j]` to `temp` and increment `j`.
            *   After one of the halves is exhausted, copy any remaining elements from the other half to `temp`.
            *   Finally, copy the sorted elements from `temp` back into the original `arr` segment `[left...right]`.
            *   Return the total count of inversions found in this merge step plus the counts from the recursive calls.

The `pairsViolatingBST` function orchestrates these two steps: it first calls `inorder` to populate the array and then calls `countInversions` on this array.

## Complexity Analysis

*   **Time Complexity**:
    *   **In-order Traversal**: We visit each of the `N` nodes exactly once. This takes $O(N)$ time.
    *   **Counting Inversions (Merge Sort)**: The merge sort algorithm, including the inversion counting modification, has a time complexity of $O(N \log N)$ for an array of size `N`.
    *   **Total Time Complexity**: $O(N) + O(N \log N) = O(N \log N)$.

*   **Space Complexity**:
    *   **`arr` vector**: Stores all `N` node values, requiring $O(N)$ space.
    *   **`temp` vector**: Used during the merge step of merge sort, requiring $O(N)$ space.
    *   **Recursion Stack**:
        *   For `inorder` traversal: In the worst case (a skewed tree), the recursion depth can be `N`, requiring $O(N)$ stack space. In the best case (a balanced tree), it's $O(\log N)$.
        *   For `countInversions`: The recursion depth for merge sort is $O(\log N)$.
    *   **Total Space Complexity**: Dominated by the `arr` and `temp` vectors, and potentially the `inorder` recursion stack in the worst case. Thus, $O(N)$.

## Solution Code

```cpp
#include <vector>

// Definition for a binary tree node (assuming it's provided by GfG)
// struct Node {
//     int data;
//     Node *left;
//     Node *right;
// };

class Solution {
public:
    // Standard merge sort based inversion counting
    // This function sorts the subarray arr[left...right] and returns the number of inversions
    long long countInversions(std::vector<int>& arr, int left, int right) {
        // Base case: If the subarray has 0 or 1 element, no inversions
        if (left >= right) return 0;

        int mid = left + (right - left) / 2;
        long long count = 0;

        // Recursively count inversions in left and right halves
        count += countInversions(arr, left, mid);
        count += countInversions(arr, mid + 1, right);
        
        // Merge the two sorted halves and count inversions between them
        std::vector<int> temp;
        int i = left;      // Pointer for the left half
        int j = mid + 1;   // Pointer for the right half

        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                // No inversion: element from left half is smaller or equal
                temp.push_back(arr[i++]);
            } else {
                // Inversion found: arr[i] > arr[j]
                // Since the left part (arr[left...mid]) is sorted,
                // all elements from arr[i] to arr[mid] are greater than arr[j].
                // The number of such elements is (mid - i + 1).
                count += (mid - i + 1);
                temp.push_back(arr[j++]);
            }
        }

        // Copy any remaining elements from the left half
        while (i <= mid) {
            temp.push_back(arr[i++]);
        }
        // Copy any remaining elements from the right half
        while (j <= right) {
            temp.push_back(arr[j++]);
        }
        
        // Copy the sorted elements from temp back to the original array segment
        for (int k = 0; k < temp.size(); ++k) {
            arr[left + k] = temp[k];
        }
        
        return count;
    }

    // Helper function to collect in-order traversal of the binary tree
    void inorder(Node* root, std::vector<int>& arr) {
        if (!root) return;
        inorder(root->left, arr);
        arr.push_back(root->data);
        inorder(root->right, arr);
    }

    // Main function to find pairs violating BST property
    int pairsViolatingBST(Node* root) {
        std::vector<int> arr;
        // Step 1: Get the in-order traversal of the tree
        inorder(root, arr);
        
        // Handle empty tree case
        if (arr.empty()) return 0;

        // Step 2: Count inversions in the in-order traversal array
        // The result is long long, cast to int as per problem return type
        return static_cast<int>(countInversions(arr, 0, arr.size() - 1));
    }
};
```