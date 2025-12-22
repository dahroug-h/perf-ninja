#include "const.h"
#include "solution.h"

#include <x86intrin.h>

std::vector<short> mandelbrot(int image_width, int image_height) {
  const auto data_width = image_width + 2;
  const auto data_height = image_height + 2;
  const auto diameter_y = kDiameterX / image_width * image_height;
  const auto min_x = kCenterX - kDiameterX / 2;
  const auto max_x = kCenterX + kDiameterX / 2;
  const auto min_y = kCenterY - diameter_y / 2;
  const auto max_y = kCenterY + diameter_y / 2;
  std::vector<short> result(data_width * data_height);

  // auto result_idx = 0;
  // for (auto py = 0; py < data_height; ++py) {
  //   for (auto px = 0; px < data_width; ++px) {
  //     const auto c_x = min_x + (max_x - min_x) * px / data_width;
  //     const auto c_y = min_y + (max_y - min_y) * py / data_height;
  //     auto z_x = 0.0;
  //     auto z_y = 0.0;
  //     auto iter_cnt = 0;
  //     for (; iter_cnt < kMaxIterations; ++iter_cnt) {
  //       const auto z_xx = z_x * z_x;
  //       const auto z_yy = z_y * z_y;
  //       if (z_xx + z_yy > kSquareBound) {
  //         break;
  //       }
  //       const auto z_xy = z_x * z_y;
  //       z_x = z_xx - z_yy + c_x;
  //       z_y = z_xy + z_xy + c_y;
  //     }
  //     result[result_idx++] = iter_cnt;
  //   }
  // }


  auto result_idx = 0;
  const __m256d square_bound_4 = _mm256_set_pd(kSquareBound, kSquareBound, kSquareBound, kSquareBound);  
  const __m256i max_iterations_4 = _mm256_set_epi64x(kMaxIterations, kMaxIterations, kMaxIterations, kMaxIterations);
  const __m256i iter_increment = _mm256_set_epi64x(1, 1, 1, 1);
  for (auto py = 0; py < data_height; ++py) {
    const auto c_y = min_y + (max_y - min_y) * py / data_height;
    __m256d c_y_v4 = _mm256_set_pd(c_y, c_y, c_y, c_y);
    const int width = (data_width / 4) * 4;
    auto px = 0;
    for (; px < width; px += 4) {
      __m256d c_x_v4;
      __m256d z_x_v4;
      __m256d z_y_v4;      
      __m256i iter_cnt_v4 = _mm256_setzero_si256();

      double tmp[4];
      for (int i = 0; i < 4; i++) {
        tmp[i] = min_x + (max_x - min_x) * (px + i) / data_width;
      }

      c_x_v4 = _mm256_set_pd(tmp[0], tmp[1], tmp[2], tmp[3]);
      z_x_v4 = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
      z_y_v4 = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);

      while (1) {
        __m256d z_xx_v4 = _mm256_mul_pd(z_x_v4, z_x_v4);
        __m256d z_yy_v4 = _mm256_mul_pd(z_y_v4, z_y_v4);
        __m256d s = _mm256_add_pd(z_xx_v4, z_yy_v4);
        __m256d cmp_mask = _mm256_cmp_pd(square_bound_4, s, _CMP_NGT_UQ);        
        int mask = _mm256_movemask_pd(cmp_mask);
        __m256i cmp_mask_int = _mm256_castpd_si256(cmp_mask);
        iter_cnt_v4 = _mm256_add_epi64(iter_cnt_v4, _mm256_add_epi64(iter_increment, cmp_mask_int));
        __m256i cmp_mask_2 = _mm256_cmpeq_epi64(iter_cnt_v4, max_iterations_4);
        int mask2 = _mm256_movemask_pd(_mm256_castsi256_pd(cmp_mask_2));
        if ((mask | mask2) == 0xf)
          break;

        __m256d z_xy_v4 = _mm256_mul_pd(z_x_v4, z_y_v4);
        z_x_v4 = _mm256_add_pd(_mm256_sub_pd(z_xx_v4, z_yy_v4), c_x_v4);
        z_y_v4 = _mm256_add_pd(_mm256_add_pd(z_xy_v4, z_xy_v4), c_y_v4);
      }      

      for (int i = 0; i < 4; i++) {
        result[result_idx] = iter_cnt_v4[3 - i];
        result_idx++;
      }
    }

    for (; px < data_width; px += 1) {
      const auto c_x = min_x + (max_x - min_x) * px / data_width;
      auto z_x = 0.0;
      auto z_y = 0.0;
      auto iter_cnt = 0;
      for (; iter_cnt < kMaxIterations; ++iter_cnt) {
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

  return result;
}
