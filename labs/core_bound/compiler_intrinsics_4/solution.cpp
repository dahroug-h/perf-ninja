#include "const.h"
#include "solution.h"
#include <immintrin.h>
#include <iostream>

short OnePoint(const double c_x, const double c_y) {
    auto z_x = 0.0;
    auto z_y = 0.0;
    auto iter_cnt = 0;
    for (; iter_cnt < kMaxIterations; ++iter_cnt) {
      const auto z_xx = z_x * z_x;
      const auto z_yy = z_y * z_y;
      if (z_xx + z_yy > kSquareBound) {
        return iter_cnt;
      }
      const auto z_xy = z_x * z_y;
      z_x = z_xx - z_yy + c_x;
      z_y = z_xy + z_xy + c_y;
    }
    return kMaxIterations;
}

std::vector<short> mandelbrot1(int image_width, int image_height) {
  const auto data_width = image_width + 2;
  const auto data_height = image_height + 2;
  const auto diameter_y = kDiameterX / image_width * image_height;
  const auto min_x = kCenterX - kDiameterX / 2;
  const auto max_x = kCenterX + kDiameterX / 2;
  const auto min_y = kCenterY - diameter_y / 2;
  const auto max_y = kCenterY + diameter_y / 2;
  std::vector<short> result(data_width * data_height);
  auto result_idx = 0;
  for (auto py = 0; py < data_height; ++py) {
    for (auto px = 0; px < data_width; ++px) {
      const auto c_x = min_x + (max_x - min_x) * px / data_width;
      const auto c_y = min_y + (max_y - min_y) * py / data_height;
      auto z_x = 0.0;
      auto z_y = 0.0;
      auto iter_cnt = 0;
      for (; iter_cnt < kMaxIterations; ++iter_cnt) {
        // if ((result_idx == 0 || result_idx == 640) && (iter_cnt == 1 || iter_cnt == 116)) 
        //   printf("\t%d iter=%d: z=(%.10f, %.10f), c=(%.10f, %.10f) p=(%d, %d)\n", result_idx, iter_cnt, z_x, z_y, c_x, c_y, px, py);
        const auto z_xx = z_x * z_x;
        const auto z_yy = z_y * z_y;
        if (z_xx + z_yy > kSquareBound) {
          break;
        }
        const auto z_xy = z_x * z_y;
        z_x = z_xx - z_yy + c_x;
        z_y = z_xy + z_xy + c_y;
      }
      result[result_idx++] = iter_cnt;
    }
  }
  // std::cout << image_width << ", " << image_height << ": " << result[640] << " " << result[641] << " " << result[642] << " " << result[643] << "\n";
  return result;
}

std::vector<short> mandelbrot2(int image_width, int image_height) {
  // std::cout << image_width << " " << image_height << "\n";
  const auto data_width = image_width + 2;
  const auto data_height = image_height + 2;
  const auto diameter_y = kDiameterX / image_width * image_height;
  const auto min_x = kCenterX - kDiameterX / 2;
  const auto max_x = kCenterX + kDiameterX / 2;
  const auto min_y = kCenterY - diameter_y / 2;
  const auto max_y = kCenterY + diameter_y / 2;
  std::vector<short> result(data_width * data_height);
  auto result_idx = 0;
  auto dx = (max_x - min_x) / data_width;
  auto dy = (max_y - min_y) / data_height;
  for (auto py = 0; py < data_height; ++py) {
    __m256d c_y = _mm256_set1_pd(min_y + dy * py);
    auto px = 0;
    for (; px + 4 <= data_width; px += 4) {
      //   const auto c_x = min_x + dx * px;
      __m256d c_x = _mm256_add_pd(_mm256_set1_pd(min_x), _mm256_mul_pd(_mm256_set1_pd(dx), _mm256_set_pd(px + 3, px + 2, px + 1, px)));
      //   auto z_x = 0.0;
      //   auto z_y = 0.0;
      __m256d z_x = _mm256_set1_pd(0.0);
      __m256d z_y = _mm256_set1_pd(0.0);
      auto iter_cnt = 1;
      __m256d square_bound = _mm256_set1_pd(kSquareBound);
      result_idx = py * data_width + px;
      result[result_idx] = result[result_idx + 1] = result[result_idx + 2] = result[result_idx + 3] = kMaxIterations;

      int all_cmp = 0;
      for (; iter_cnt <= kMaxIterations; ++iter_cnt) {
        // const auto z_xx = z_x * z_x;
        __m256d z_xx = _mm256_mul_pd(z_x, z_x);
        // const auto z_yy = z_y * z_y;
        __m256d z_yy = _mm256_mul_pd(z_y, z_y);

        // if (z_xx + z_yy > kSquareBound) {
        //   break;
        // }
        int res_cmp = _mm256_movemask_pd(_mm256_cmp_pd(_mm256_add_pd(z_xx, z_yy), square_bound, _CMP_GT_OS));
        all_cmp |= res_cmp;
        if (all_cmp == 15) {
          break;
        }
        result[result_idx] = ((all_cmp & 1) == 0 ? iter_cnt : result[result_idx]);
        result[result_idx + 1] = ((all_cmp & (1 << 1)) == 0 ? iter_cnt : result[result_idx + 1]);
        result[result_idx + 2] = ((all_cmp & (1 << 2)) == 0 ? iter_cnt : result[result_idx + 2]);
        result[result_idx + 3] = ((all_cmp & (1 << 3)) == 0 ? iter_cnt : result[result_idx + 3]);

        // const auto z_xy = z_x * z_y;
        __m256d z_xy = _mm256_mul_pd(z_x, z_y);
        // z_x = z_xx - z_yy + c_x;
        z_x = _mm256_add_pd(_mm256_sub_pd(z_xx, z_yy), c_x);
        // z_y = z_xy + z_xy + c_y;
        z_y = _mm256_add_pd(_mm256_add_pd(z_xy, z_xy), c_y);
      }
      result_idx += 4;
    }

    while (px < data_width) {
      const auto c_x = min_x + (max_x - min_x) * px / data_width;
      const auto c_y = min_y + (max_y - min_y) * py / data_height;
      result[result_idx++] = OnePoint(c_x, c_y);
      px++;
    }
  }
  return result;
}

std::vector<short> mandelbrot(int image_width, int image_height) {
  return mandelbrot2(image_width, image_height);
}
