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