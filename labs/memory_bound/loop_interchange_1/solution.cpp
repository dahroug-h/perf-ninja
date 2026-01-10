
#include "solution.h"
#include <memory>
#include <string_view>
#include <immintrin.h>

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
void multiply1(Matrix &result, const Matrix &a, const Matrix &b) {
  zero(result);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void multiply2(Matrix &result, const Matrix &a, const Matrix &b) {
  zero(result);

  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) {
      for (int j = 0; j < N; j++) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void multiply3(Matrix &result, const Matrix &a, const Matrix &b) {
  zero(result);

  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) {
      __m256d aik = _mm256_set1_ps(a[i][k]);
      const float* dt2 = b[k].data();
      float* res = result[i].data();
      for (int j = 0; j < N; j += 8) {
        // result[i][j] += a[i][k] * b[k][j];
        __m256d new_res = _mm256_add_ps(_mm256_loadu_ps(res), _mm256_mul_ps(aik, _mm256_loadu_ps(dt2)));
        _mm256_storeu_ps(res, new_res);
        res += 8;
        dt2 += 8;
      }
    }
  }
}

void multiply(Matrix &result, const Matrix &a, const Matrix &b) {
  return multiply2(result, a, b);
}

// Compute integer power of a given square matrix
Matrix power(const Matrix &input, const uint32_t k) {
  // Temporary products
  std::unique_ptr<Matrix> productCurrent(new Matrix());
  std::unique_ptr<Matrix> productNext(new Matrix());

  // Temporary elements = a^(2^integer)
  std::unique_ptr<Matrix> elementCurrent(new Matrix());
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
