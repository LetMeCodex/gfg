# Sort in Specific Order

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to sort an array of integers in a specific order. The requirements are:
1. All odd numbers should appear before all even numbers.
2. Among the odd numbers, they should be sorted in descending order.
3. Among the even numbers, they should be sorted in ascending order.

## Intuition & Approach

The problem can be broken down into two main steps:
1. **Partitioning**: We need to separate the odd numbers from the even numbers. The requirement is that all odd numbers must come before all even numbers. This immediately suggests a partitioning strategy.
2. **Sorting within Partitions**: Once the array is partitioned, we need to sort the odd numbers in descending order and the even numbers in ascending order.

**Detailed Approach:**

1.  **Partitioning using `std::partition`**: The `std::partition` algorithm in C++ is perfect for this. It rearranges the elements in a range `[first, last)` such that all elements for which a given predicate returns `true` precede all elements for which the predicate returns `false`.
    *   We want odd numbers to come first. So, our predicate should return `true` if a number is odd and `false` if it's even. The condition `x % 2 != 0` checks for oddness.
    *   `std::partition` returns an iterator to the first element of the second group (i.e., the first even number). Let's call this `partition_point`.

2.  **Sorting Odd Numbers**: The elements from the beginning of the array (`arr.begin()`) up to `partition_point` are the odd numbers. We need to sort these in descending order. The `std::sort` algorithm can be used with a custom comparator or `std::greater<int>()` for descending order.

3.  **Sorting Even Numbers**: The elements from `partition_point` to the end of the array (`arr.end()`) are the even numbers. We need to sort these in ascending order. The default behavior of `std::sort` is ascending order, so we can directly use `std::sort` on this sub-range.

**Example Walkthrough:**

Let's consider an example array: `arr = {5, 2, 8, 1, 9, 4, 7, 6}`

1.  **Partitioning**:
    *   `std::partition(arr.begin(), arr.end(), [](int x){ return x % 2 != 0; })`
    *   The predicate `x % 2 != 0` is true for 5, 1, 9, 7.
    *   The predicate is false for 2, 8, 4, 6.
    *   After partitioning, the array might look like: `{5, 1, 9, 7, 2, 8, 4, 6}`.
    *   `partition_point` will point to the element `2`.

2.  **Sorting Odd Numbers**:
    *   Sort `arr.begin()` to `partition_point` (i.e., `{5, 1, 9, 7}`) in descending order.
    *   This becomes `{9, 7, 5, 1}`.
    *   The array is now: `{9, 7, 5, 1, 2, 8, 4, 6}`.

3.  **Sorting Even Numbers**:
    *   Sort `partition_point` to `arr.end()` (i.e., `{2, 8, 4, 6}`) in ascending order.
    *   This becomes `{2, 4, 6, 8}`.
    *   The final array is: `{9, 7, 5, 1, 2, 4, 6, 8}`.

This approach directly addresses all the problem requirements.

## Complexity Analysis

-   **Time Complexity**: $O(N \log N)$
    *   The `std::partition` operation takes $O(N)$ time, where $N$ is the number of elements in the array.
    *   The `std::sort` operation on the odd numbers takes $O(k \log k)$ time, where $k$ is the number of odd elements.
    *   The `std::sort` operation on the even numbers takes $O((N-k) \log (N-k))$ time, where $N-k$ is the number of even elements.
    *   In the worst case, either $k$ or $N-k$ can be close to $N$. Therefore, the dominant factor is the sorting, leading to a total time complexity of $O(N \log N)$.

-   **Space Complexity**: $O(1)$
    *   The `std::partition` and `std::sort` algorithms in C++ typically use in-place sorting algorithms (like IntroSort, which is a hybrid of QuickSort, HeapSort, and InsertionSort). These algorithms have a space complexity of $O(\log N)$ for the recursion stack in the worst case for QuickSort, but often $O(1)$ auxiliary space is considered for practical purposes or if a non-recursive sorting algorithm is used. For `std::partition`, it's also $O(1)$ auxiliary space.
    *   Since we are modifying the input array in-place and not using any additional data structures that grow with the input size, the space complexity is $O(1)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm>
#include <functional> // Required for std::greater

class Solution {
public:
    /**
     * @brief Sorts an array such that odd numbers come first in descending order,
     *        followed by even numbers in ascending order.
     *
     * @param arr The input vector of integers to be sorted.
     * @return The sorted vector according to the specified order.
     */
    std::vector<int> sortIt(std::vector<int>& arr) {
        // Partition the array into odd numbers (true) and even numbers (false).
        // The std::partition algorithm rearranges elements such that all elements
        // for which the predicate returns true are placed before elements for which
        // it returns false. We want odd numbers first, so the predicate should
        // return true for odd numbers.
        auto partition_point = std::partition(arr.begin(), arr.end(), [](int x){
            return x % 2 != 0; // Predicate: returns true if x is odd
        });

        // After partitioning, the array is structured as:
        // [odd_numbers..., even_numbers...]
        // 'partition_point' is an iterator pointing to the first even number.

        // Sort the odd numbers (elements from arr.begin() up to partition_point)
        // in descending order. std::greater<int>() is used as the comparator
        // for descending sort.
        std::sort(arr.begin(), partition_point, std::greater<int>());

        // Sort the even numbers (elements from partition_point up to arr.end())
        // in ascending order. The default std::sort behavior is ascending.
        std::sort(partition_point, arr.end());

        // Return the modified array, which is now sorted according to the problem's criteria.
        return arr;
    }
};
```