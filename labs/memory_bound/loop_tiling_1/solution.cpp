#include "solution.hpp"

// Cache-oblivious recursive transpose
void transposeRecursive(MatrixOfDoubles &in, MatrixOfDoubles &out,
                       int rowStart, int rowEnd, int colStart, int colEnd) {
  int rowSize = rowEnd - rowStart;
  int colSize = colEnd - colStart;
  
  // Base case: if the block is small enough, transpose directly
  const int BASE_CASE = 16;
  if (rowSize <= BASE_CASE && colSize <= BASE_CASE) {
    for (int i = rowStart; i < rowEnd; i++) {
      for (int j = colStart; j < colEnd; j++) {
        out[i][j] = in[j][i];
      }
    }
    return;
  }
  
  // Recursive case: divide the matrix
  if (rowSize >= colSize) {
    // Split along rows
    int rowMid = rowStart + rowSize / 2;
    transposeRecursive(in, out, rowStart, rowMid, colStart, colEnd);
    transposeRecursive(in, out, rowMid, rowEnd, colStart, colEnd);
  } else {
    // Split along columns
    int colMid = colStart + colSize / 2;
    transposeRecursive(in, out, rowStart, rowEnd, colStart, colMid);
    transposeRecursive(in, out, rowStart, rowEnd, colMid, colEnd);
  }
}

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  transposeRecursive(in, out, 0, size, 0, size);
  return out[0][size - 1];
}
