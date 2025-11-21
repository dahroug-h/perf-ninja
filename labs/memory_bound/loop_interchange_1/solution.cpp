
#include "solution.h"
#include <memory>
#include <string_view>

// Make zero matrix
void zero(Matrix &result) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result[i][j] = 0;
    }
  }
}

// Make identity matrix
void identity(Matrix &result) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result[i][j] = 0;
    }
    result[i][i] = 1;
  }
}

// Multiply two square matrices
void multiply(Matrix &result, const Matrix &a, const Matrix &b) {
  zero(result);

  constexpr int kBlockNum = 5;
  constexpr int kBlockSize = N / kBlockNum;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < kBlockSize; k++) {
        result[i][0 * kBlockSize + k] += a[i][j] * b[j][0 * kBlockSize + k];
        result[i][1 * kBlockSize + k] += a[i][j] * b[j][1 * kBlockSize + k];
        result[i][2 * kBlockSize + k] += a[i][j] * b[j][2 * kBlockSize + k];
        result[i][3 * kBlockSize + k] += a[i][j] * b[j][3 * kBlockSize + k];
        result[i][4 * kBlockSize + k] += a[i][j] * b[j][4 * kBlockSize + k];
      }
    }
  }
}

void multiply_transposed(Matrix &result, const Matrix &a, const Matrix &b_tr) {
  zero(result);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        // result[i][j] += a[i][k] * b[k][j];
        result[i][j] += a[i][k] * b_tr[j][k];
      }
    }
  }
}

// Compute integer power of a given square matrix
Matrix power(const Matrix &input, const uint32_t k) {
  // Temporary products
  std::unique_ptr<Matrix> productCurrent(new Matrix());
  std::unique_ptr<Matrix> productNext(new Matrix());

  // Temporary elements = a^(2^integer)
  std::unique_ptr<Matrix> elementCurrent(new Matrix());
  // std::unique_ptr<Matrix> elementCurrentTr(new Matrix());
  std::unique_ptr<Matrix> elementNext(new Matrix());

  // Initial values
  identity(*productCurrent);
  *elementCurrent = input;

  // Use binary representation of k to be O(log(k))
  for (auto i = k; i > 0; i /= 2) {
    if (i % 2 != 0) {
      // Multiply the product by element
      multiply(*productNext, *productCurrent, *elementCurrent);
      std::swap(productNext, productCurrent);

      // Exit early to skip next squaring
      if (i == 1)
        break;
    }

    // Square an element
    multiply(*elementNext, *elementCurrent, *elementCurrent);
    std::swap(elementNext, elementCurrent);
  }

  return std::move(*productCurrent);
}
