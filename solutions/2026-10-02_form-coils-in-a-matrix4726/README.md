# [Coils in a Matrix](https://www.geeksforgeeks.org/problems/form-coils-in-a-matrix4726/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to construct two "coils" of numbers within a square matrix of size $4n \times 4n$. The numbers in the matrix are filled sequentially from 1 to $(4n)^2$. The two coils are formed by traversing the matrix in a spiral pattern, but with specific starting points and directions.

Coil 1 starts from the top-left corner $(0,0)$ and spirals inwards clockwise.
Coil 2 starts from the bottom-right corner $(4n-1, 4n-1)$ and spirals inwards counter-clockwise.

The output should be a vector of two vectors, where the first inner vector contains the numbers of Coil 1 in the order they are visited, and the second inner vector contains the numbers of Coil 2 in the order they are visited.

## Intuition & Approach

The core of this problem lies in understanding how to traverse a matrix in a spiral pattern and how to adapt this traversal for two distinct spirals starting from opposite corners.

The matrix size is $N \times N$, where $N = 4n$. This means the matrix is divided into $n$ concentric layers or "rings". For $n=1$, we have a $4 \times 4$ matrix and one layer. For $n=2$, we have an $8 \times 8$ matrix and two layers.

We can iterate through these layers from the outermost to the innermost. For each layer $k$ (where $k$ ranges from $0$ to $n-1$), we define the boundaries of the current square sub-matrix that forms this layer.
The top-left corner of layer $k$ is at $(2k, 2k)$, and the bottom-right corner is at $(N-1-2k, N-1-2k)$. Let's denote these as `start_r`, `start_c`, `end_r`, and `end_c` respectively.

**Coil 1 (Clockwise from Top-Left):**
For each layer $k$, Coil 1 traverses the outer boundary of the current layer clockwise.
1.  **Down:** From `(start_r, start_c)` to `(end_r, start_c)`.
2.  **Right:** From `(end_r, start_c + 1)` to `(end_r, end_c - 1)`. We skip `(end_r, start_c)` as it's covered by the previous segment, and `(end_r, end_c)` belongs to Coil 2's boundary.
3.  **Up:** From `(end_r - 1, end_c - 1)` to `(start_r + 1, end_c - 1)`. We skip `(end_r, end_c - 1)` as it's covered by the previous segment, and `(start_r, end_c - 1)` is part of Coil 2's boundary.
4.  **Left:** From `(start_r + 1, end_c - 2)` to `(start_r + 1, start_c + 2)`. This segment connects to the inner layer. It's only traversed if we are not on the innermost layer ($k < n-1$). We skip `(start_r + 1, end_c - 1)` as it's covered by the previous segment, and `(start_r + 1, start_c + 1)` is the starting point of the next layer's Coil 2 traversal.

**Coil 2 (Counter-Clockwise from Bottom-Right):**
For each layer $k$, Coil 2 traverses the outer boundary of the current layer counter-clockwise.
1.  **Up:** From `(end_r, end_c)` to `(start_r, end_c)`.
2.  **Left:** From `(start_r, end_c - 1)` to `(start_r, start_c + 1)`. We skip `(start_r, end_c)` as it's covered by the previous segment, and `(start_r, start_c)` belongs to Coil 1's boundary.
3.  **Down:** From `(start_r + 1, start_c + 1)` to `(end_r - 1, start_c + 1)`. We skip `(start_r, start_c + 1)` as it's covered by the previous segment, and `(end_r, start_c + 1)` is part of Coil 1's boundary.
4.  **Right:** From `(end_r - 1, start_c + 2)` to `(end_r - 1, end_c - 2)`. This segment connects to the inner layer. It's only traversed if we are not on the innermost layer ($k < n-1$). We skip `(end_r - 1, start_c + 1)` as it's covered by the previous segment, and `(end_r - 1, end_c - 1)` is the starting point of the next layer's Coil 1 traversal.

The numbers are calculated using the formula `row * N + col + 1` to convert 0-indexed row and column to 1-indexed matrix values.

The key to solving this in two attempts was realizing the precise boundaries and the segments that connect to the inner layers. The initial attempt might have had off-by-one errors in loop bounds or incorrect handling of the connecting segments for the inner layers. Careful tracing of the traversal for a small `n` (like `n=1` or `n=2`) helps identify these edge cases.

## Complexity Analysis

-   **Time Complexity**: $O(n^2)$
    The matrix size is $N \times N$, where $N = 4n$. The total number of elements in the matrix is $(4n)^2 = 16n^2$.
    Each element in the matrix is visited and added to one of the coils exactly once. The loops iterate through segments of the matrix boundaries. The total number of operations is proportional to the total number of elements in the matrix. Therefore, the time complexity is $O(N^2)$, which is $O((4n)^2) = O(16n^2) = O(n^2)$.

-   **Space Complexity**: $O(n^2)$
    We are storing the elements of Coil 1 and Coil 2 in two vectors. The total number of elements stored is equal to the total number of elements in the matrix, which is $N^2 = (4n)^2 = 16n^2$. Thus, the space complexity is $O(n^2)$.

## Solution Code

```cpp
#include <vector>
#include <numeric> // Not strictly needed, but good practice for competitive programming headers

class Solution {
public:
    // Renamed the function from 'coils' to 'formCoils' to match the driver code's expectation.
    std::vector<std::vector<int>> formCoils(int n) {
        int N = 4 * n; // Dimension of the square matrix
        std::vector<int> coil1_elements;
        std::vector<int> coil2_elements;

        // Iterate through 'n' layers of the coil
        // Each layer 'k' (0-indexed) defines a square boundary for the current segment of the spiral.
        // The problem implies that for n=1, the matrix is 4x4, and there's 1 layer (k=0).
        // For n=2, the matrix is 8x8, and there are 2 layers (k=0, k=1).
        for (int k = 0; k < n; ++k) {
            // Define the boundaries for the current layer 'k'
            // The outermost cells of layer 'k' are at row/column indices 2*k and N-1-2*k.
            int start_r = 2 * k;
            int start_c = 2 * k;
            int end_r = N - 1 - 2 * k;
            int end_c = N - 1 - 2 * k;

            // --- Coil 1 construction ---
            // Coil 1 starts from (0,0) and spirals inward.
            // For layer k, it starts at (start_r, start_c) and moves clockwise.
            
            // Segment 1: Down along the leftmost column of the current layer
            // From (start_r, start_c) to (end_r, start_c)
            for (int r = start_r; r <= end_r; ++r) {
                coil1_elements.push_back(r * N + start_c + 1);
            }

            // Segment 2: Right along the bottommost row of the current layer
            // From (end_r, start_c + 1) to (end_r, end_c - 1)
            // (end_r, start_c) was already added. (end_r, end_c) belongs to Coil 2.
            for (int c = start_c + 1; c <= end_c - 1; ++c) {
                coil1_elements.push_back(end_r * N + c + 1);
            }

            // Segment 3: Up along the second-to-rightmost column of the current layer
            // From (end_r - 1, end_c - 1) down to (start_r + 1, end_c - 1)
            // (end_r, end_c-1) was added in segment 2. (start_r, end_c-1) is not part of this coil.
            for (int r = end_r - 1; r >= start_r + 1; --r) {
                coil1_elements.push_back(r * N + (end_c - 1) + 1);
            }

            // Segment 4: Left along the second-to-topmost row, connecting to the inner layer.
            // This segment only exists for outer layers (k < n-1).
            // It connects from (start_r + 1, end_c - 2) to (start_r + 1, start_c + 2).
            // (start_r + 1, end_c - 1) was added in segment 3.
            // The next layer starts at (start_r + 2, start_c + 2). This segment ends one cell above that.
            if (k < n - 1) {
                for (int c = end_c - 2; c >= start_c + 2; --c) {
                    coil1_elements.push_back((start_r + 1) * N + c + 1);
                }
            }

            // --- Coil 2 construction ---
            // Coil 2 starts from (N-1, N-1) and spirals inward.
            // For layer k, it starts at (end_r, end_c) and moves counter-clockwise.
            
            // Segment 1: Up along the rightmost column of the current layer
            // From (end_r, end_c) down to (start_r, end_c)
            for (int r = end_r; r >= start_r; --r) {
                coil2_elements.push_back(r * N + end_c + 1);
            }

            // Segment 2: Left along the topmost row of the current layer
            // From (start_r, end_c - 1) down to (start_r, start_c + 1)
            // (start_r, end_c) was already added. (start_r, start_c) belongs to Coil 1.
            for (int c = end_c - 1; c >= start_c + 1; --c) {
                coil2_elements.push_back(start_r * N + c + 1);
            }

            // Segment 3: Down along the second-to-leftmost column of the current layer
            // From (start_r + 1, start_c + 1) to (end_r - 1, start_c + 1)
            // (start_r, start_c+1) was added in segment 2. (end_r, start_c+1) is not part of this coil.
            for (int r = start_r + 1; r <= end_r - 1; ++r) {
                coil2_elements.push_back(r * N + (start_c + 1) + 1);
            }

            // Segment 4: Right along the second-to-bottommost row, connecting to the inner layer.
            // This segment only exists for outer layers (k < n-1).
            // It connects from (end_r - 1, start_c + 2) to (end_r - 1, end_c - 2).
            // (end_r - 1, start_c + 1) was added in segment 3.
            // The next layer ends at (end_r - 2, end_c - 2). This segment ends one cell above that.
            if (k < n - 1) {
                for (int c = start_c + 2; c <= end_c - 2; ++c) {
                    coil2_elements.push_back((end_r - 1) * N + c + 1);
                }
            }
        }

        return {coil1_elements, coil2_elements};
    }
};
```