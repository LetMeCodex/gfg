# Largest with Same Parity Adjacent Swaps

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to rearrange an array of non-negative integers such that the resulting array is lexicographically largest, with the constraint that we can only swap adjacent elements if they have the same parity (both even or both odd).

## Intuition & Approach

The core constraint is that swaps are only allowed between adjacent elements of the same parity. This implies that elements of different parities can never cross each other. In other words, an even number can never move past an odd number, and vice-versa. This effectively partitions the array into contiguous blocks of numbers with the same parity.

For example, if the array is `[1, 4, 3, 2, 5]`, the parities are `[Odd, Even, Odd, Even, Odd]`. The blocks of same parity are:
- `[1]` (Odd)
- `[4, 2]` (Even)
- `[3, 5]` (Odd)

Within each of these blocks, we can perform any number of adjacent swaps. Since we want the lexicographically largest array, and we have complete freedom to rearrange elements *within* a same-parity block, the optimal strategy for each block is to sort its elements in descending order.

The overall strategy then becomes:
1. **Identify contiguous blocks of same-parity numbers.**
2. **For each identified block, sort its elements in descending order.**

We can iterate through the array and keep track of the starting index of the current block of same-parity numbers. When we encounter an element whose parity is different from the previous one, it signifies the end of the current block. At this point, we sort the elements from the start of the current block up to (but not including) the current element in descending order. We then update the start of the next block to be the current element's index.

After the loop finishes, there will be one last block of same-parity numbers from the last recorded `current_block_start_index` to the end of the array. This final block also needs to be sorted in descending order.

Let's trace with an example: `arr = [1, 4, 3, 2, 5]`

- `i = 0`: `arr[0] = 1` (Odd). `current_block_start_index = 0`.
- `i = 1`: `arr[1] = 4` (Even). Parity of `arr[0]` (Odd) != Parity of `arr[1]` (Even).
    - Block ends at index `i=0`. Sort `arr[0...0]` (which is just `[1]`) in descending order. Array remains `[1, 4, 3, 2, 5]`.
    - Update `current_block_start_index = i + 1 = 2`.
- `i = 2`: `arr[2] = 3` (Odd). Parity of `arr[1]` (Even) != Parity of `arr[2]` (Odd).
    - Block ends at index `i=1`. Sort `arr[2...1]` (this is incorrect, the block is from `current_block_start_index` to `i`). The block is `arr[2...2]`. This logic needs refinement.

Let's refine the loop condition and block identification:
We iterate from `i = 0` to `n-2` to compare `arr[i]` and `arr[i+1]`.

`arr = [1, 4, 3, 2, 5]`, `n = 5`

- `current_block_start_index = 0`
- `i = 0`: `arr[0] = 1` (Odd), `arr[1] = 4` (Even). Parities differ.
    - Block is from `current_block_start_index` (0) to `i` (0).
    - Sort `arr.begin() + 0` to `arr.begin() + 0 + 1` (i.e., `arr[0]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 1`.
- `i = 1`: `arr[1] = 4` (Even), `arr[2] = 3` (Odd). Parities differ.
    - Block is from `current_block_start_index` (1) to `i` (1).
    - Sort `arr.begin() + 1` to `arr.begin() + 1 + 1` (i.e., `arr[1]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 2`.
- `i = 2`: `arr[2] = 3` (Odd), `arr[3] = 2` (Even). Parities differ.
    - Block is from `current_block_start_index` (2) to `i` (2).
    - Sort `arr.begin() + 2` to `arr.begin() + 2 + 1` (i.e., `arr[2]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 3`.
- `i = 3`: `arr[3] = 2` (Even), `arr[4] = 5` (Odd). Parities differ.
    - Block is from `current_block_start_index` (3) to `i` (3).
    - Sort `arr.begin() + 3` to `arr.begin() + 3 + 1` (i.e., `arr[3]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 4`.

Loop ends.
Now, sort the last block from `current_block_start_index` (4) to `arr.end()`.
- Sort `arr.begin() + 4` to `arr.end()` (i.e., `arr[4]`). Array: `[1, 4, 3, 2, 5]`.

This trace seems to be missing the actual sorting of blocks. The issue is how `current_block_start_index` and `i` are used for sorting.

Let's re-think the block identification and sorting:
We iterate through the array. `current_block_start_index` marks the beginning of the current contiguous block of numbers with the same parity.
When `arr[i]` and `arr[i+1]` have different parities, it means the block of same-parity numbers ends at `arr[i]`. The elements in this block are from `arr[current_block_start_index]` to `arr[i]`. We should sort this range. After sorting, the next block starts at `arr[i+1]`, so we update `current_block_start_index = i + 1`.

Example: `arr = [1, 4, 3, 2, 5]`

- `current_block_start_index = 0`
- `i = 0`: `arr[0]=1` (Odd), `arr[1]=4` (Even). Parities differ.
    - Block is from index `current_block_start_index` (0) to `i` (0).
    - Sort `arr.begin() + 0` to `arr.begin() + 0 + 1` (i.e., `arr[0]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 1`.
- `i = 1`: `arr[1]=4` (Even), `arr[2]=3` (Odd). Parities differ.
    - Block is from index `current_block_start_index` (1) to `i` (1).
    - Sort `arr.begin() + 1` to `arr.begin() + 1 + 1` (i.e., `arr[1]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 2`.
- `i = 2`: `arr[2]=3` (Odd), `arr[3]=2` (Even). Parities differ.
    - Block is from index `current_block_start_index` (2) to `i` (2).
    - Sort `arr.begin() + 2` to `arr.begin() + 2 + 1` (i.e., `arr[2]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 3`.
- `i = 3`: `arr[3]=2` (Even), `arr[4]=5` (Odd). Parities differ.
    - Block is from index `current_block_start_index` (3) to `i` (3).
    - Sort `arr.begin() + 3` to `arr.begin() + 3 + 1` (i.e., `arr[3]`). Array: `[1, 4, 3, 2, 5]`.
    - `current_block_start_index = i + 1 = 4`.

Loop ends.
Sort the last block from `current_block_start_index` (4) to `arr.end()`.
- Sort `arr.begin() + 4` to `arr.end()` (i.e., `arr[4]`). Array: `[1, 4, 3, 2, 5]`.

This still doesn't seem right. The problem is that the `current_block_start_index` is being updated too aggressively.

Let's consider the example `[1, 2, 3, 4, 5]`.
Parities: `[O, E, O, E, O]`
Blocks: `[1]`, `[2]`, `[3]`, `[4]`, `[5]`
Sorted: `[1, 2, 3, 4, 5]` (already sorted descending within each block of size 1)

Example: `[5, 2, 3, 4, 1]`
Parities: `[O, E, O, E, O]`
Blocks: `[5]`, `[2]`, `[3]`, `[4]`, `[1]`
Sorted: `[5, 2, 3, 4, 1]`

Example: `[1, 3, 2, 4]`
Parities: `[O, O, E, E]`
Blocks: `[1, 3]`, `[2, 4]`
Sort `[1, 3]` -> `[3, 1]`
Sort `[2, 4]` -> `[4, 2]`
Result: `[3, 1, 4, 2]`

Let's trace `[1, 3, 2, 4]` with the code's logic:
`arr = [1, 3, 2, 4]`, `n = 4`

- `current_block_start_index = 0`
- `i = 0`: `arr[0]=1` (Odd), `arr[1]=3` (Odd). Parities same.
- `i = 1`: `arr[1]=3` (Odd), `arr[2]=2` (Even). Parities differ.
    - Block is from `current_block_start_index` (0) to `i` (1).
    - Sort `arr.begin() + 0` to `arr.begin() + 1 + 1` (i.e., `arr[0]` to `arr[1]`).
    - `std::sort(arr.begin(), arr.begin() + 2, std::greater<int>())`.
    - `arr` becomes `[3, 1, 2, 4]`.
    - `current_block_start_index = i + 1 = 2`.
- `i = 2`: `arr[2]=2` (Even), `arr[3]=4` (Even). Parities same.

Loop ends.
Sort the last block from `current_block_start_index` (2) to `arr.end()`.
- Sort `arr.begin() + 2` to `arr.end()` (i.e., `arr[2]` to `arr[3]`).
- `std::sort(arr.begin() + 2, arr.end(), std::greater<int>())`.
- `arr` becomes `[3, 1, 4, 2]`.

This matches the expected output. The logic seems correct now. The key is that `current_block_start_index` marks the beginning of the *current* block, and when a parity change is detected at `i+1` (comparing `arr[i]` and `arr[i+1]`), the block to be sorted is from `current_block_start_index` up to `i`.

The code uses `arr.begin() + i + 1` as the end iterator for `std::sort`. This is correct because `std::sort` sorts the range `[first, last)`, meaning `last` is exclusive. So, `arr.begin() + i + 1` points one element past `arr[i]`, effectively sorting up to and including `arr[i]`.

The final sort handles the last block correctly.

## Complexity Analysis

- **Time Complexity**: $O(N \log N)$
    - The array is iterated through once to identify blocks. This takes $O(N)$ time.
    - Inside the loop, `std::sort` is called on sub-arrays. In the worst case, each element might be part of a block that is sorted. If we have $k$ blocks, and the lengths of these blocks are $l_1, l_2, \dots, l_k$, where $\sum l_i = N$. The total time for sorting would be $\sum O(l_i \log l_i)$.
    - In the worst case, we might have many small blocks, or one large block. For example, if parities alternate `[O, E, O, E, ...]`, each block has size 1, and sorting takes $O(1)$ for each. Total $O(N)$.
    - If all elements have the same parity, we have one block of size $N$, and sorting takes $O(N \log N)$.
    - If we have alternating parities like `[O, O, ..., O, E, E, ..., E]`, we have two blocks. The sorting time would be $O(N_1 \log N_1 + N_2 \log N_2)$, where $N_1+N_2=N$. This is still bounded by $O(N \log N)$.
    - Therefore, the overall time complexity is dominated by the sorting operations, resulting in $O(N \log N)$.

- **Space Complexity**: $O(1)$
    - The solution modifies the input array in-place.
    - No auxiliary data structures are used that grow with the input size.
    - The `std::sort` function might use some auxiliary space depending on its implementation (e.g., for recursion stack in quicksort or temporary arrays in mergesort), but this is typically considered part of the algorithm's overhead and not counted as explicit extra space if it's within reasonable bounds or if the problem statement implies in-place modification. For standard library implementations, it's often $O(\log N)$ or $O(N)$ in worst-case scenarios for some sorting algorithms, but for practical purposes and typical competitive programming contexts, $O(1)$ auxiliary space is often assumed for in-place sorts. Given the problem constraints and typical solutions, $O(1)$ is the most appropriate answer.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::sort
#include <functional> // Required for std::greater<int>

class Solution {
public:
    /**
     * @brief Rearranges the array to be lexicographically largest by swapping
     *        adjacent elements of the same parity.
     * 
     * @param arr The input vector of non-negative integers.
     * @return std::vector<int> The lexicographically largest array achievable
     *         under the given constraints.
     */
    std::vector<int> lexicographicallyLargest(std::vector<int>& arr) {
        int n = arr.size();
        
        // Handle edge cases for small arrays.
        // An array with 0 or 1 element cannot have adjacent swaps,
        // and is already lexicographically largest.
        if (n <= 1) {
            return arr;
        }

        // current_block_start_index marks the beginning of the current contiguous
        // block of numbers with the same parity.
        int current_block_start_index = 0;

        // Iterate through the array to find boundaries of same-parity blocks.
        // We iterate up to n-1 because we compare arr[i] with arr[i+1].
        for (int i = 0; i < n - 1; ++i) {
            // Check if arr[i] and arr[i+1] have different parities.
            // (arr[i] % 2) gives 0 for even, 1 for odd (since arr[i] >= 0).
            if ((arr[i] % 2) != (arr[i+1] % 2)) {
                // If parities are different, it means the current block of
                // same-parity numbers ends at arr[i].
                // To make the array lexicographically largest, we sort this block
                // in descending order.
                // std::sort operates on the range [first, last).
                // The range to sort is from current_block_start_index up to i (inclusive).
                // The end iterator is arr.begin() + i + 1, which points one past arr[i].
                std::sort(arr.begin() + current_block_start_index, 
                          arr.begin() + i + 1, 
                          std::greater<int>());
                
                // The next block of same-parity numbers starts from arr[i+1].
                current_block_start_index = i + 1;
            }
        }

        // After the loop, there will always be one last block of same-parity numbers
        // from current_block_start_index to the end of the array (n-1).
        // Sort this final block in descending order as well.
        // The range is from current_block_start_index to the end of the array.
        // arr.end() points one past the last element.
        std::sort(arr.begin() + current_block_start_index, 
                  arr.end(), 
                  std::greater<int>());

        // Return the modified array.
        return arr;
    }
};
```