#include "solution.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iostream>

// The alignment algorithm which computes the alignment of the given sequence
// pairs.

result_t compute_alignment(std::vector<sequence_t> const &sequences1,
                           std::vector<sequence_t> const &sequences2) {
  using score_t = int16_t;
  using column_t = std::array<score_t, sequence_size_v + 1>;

  result_t result{};

  // We’ll process up to LANES sequences in parallel.
  static constexpr unsigned LANES = 8;  // 8×int16_t fits nicely in NEON 128-bit

  const size_t total_sequences = sequences1.size();
  assert(total_sequences == sequences2.size());

  for (size_t base = 0; base < total_sequences; base += LANES) {
    const unsigned lanes =
        static_cast<unsigned>(std::min<size_t>(LANES, total_sequences - base));

    // Per-lane pointers to the actual sequences.
    const sequence_t *seq1_ptrs[LANES]{};
    const sequence_t *seq2_ptrs[LANES]{};

    for (unsigned lane = 0; lane < lanes; ++lane) {
      seq1_ptrs[lane] = &sequences1[base + lane];
      seq2_ptrs[lane] = &sequences2[base + lane];
    }

    // DP state per lane:
    //   score_column[lane][row]
    //   horizontal_gap_column[lane][row]
    alignas(16) score_t score_column[LANES][sequence_size_v + 1];
    alignas(16) score_t horizontal_gap_column[LANES][sequence_size_v + 1];
    score_t last_vertical_gap[LANES];
    score_t last_diagonal_score[LANES];

    // Common scoring parameters.
    const score_t gap_open{-11};
    const score_t gap_extension{-1};
    const score_t match{6};
    const score_t mismatch{-4};

    /*
     * Initialise the first column of the matrix for all lanes.
     *
     * Original scalar logic:
     *   horizontal_gap_column[0] = gap_open;
     *   last_vertical_gap = gap_open;
     *   for i = 1..N:
     *     score_column[i] = gap_open + (i-1)*gap_extension;
     *     horizontal_gap_column[i] = score_column[i] + gap_open;
     */
    for (unsigned lane = 0; lane < lanes; ++lane) {
      score_column[lane][0] = 0;             // top-left cell is usually 0
      horizontal_gap_column[lane][0] = gap_open;
      last_vertical_gap[lane] = gap_open;    // will be reset at first column anyway

      for (size_t i = 1; i < sequence_size_v + 1; ++i) {
        score_t v = gap_open + gap_extension * static_cast<score_t>(i - 1);
        score_column[lane][i] = v;
        horizontal_gap_column[lane][i] = static_cast<score_t>(v + gap_open);
      }
    }

    /*
     * Compute the main recursion to fill the matrix.
     *
     * Outer loop over columns,
     *   inner loop over rows,
     *     innermost loop over lanes (SIMD-friendly).
     */
    for (unsigned col = 1; col <= sequence_size_v; ++col) {
      // --- Handle row 0 for all lanes (top cell of this column).
      for (unsigned lane = 0; lane < lanes; ++lane) {
        // Cache last diagonal score for row=0
        last_diagonal_score[lane] = score_column[lane][0];

        score_column[lane][0] = horizontal_gap_column[lane][0];
        last_vertical_gap[lane] =
            static_cast<score_t>(horizontal_gap_column[lane][0] + gap_open);
        horizontal_gap_column[lane][0] =
            static_cast<score_t>(horizontal_gap_column[lane][0] + gap_extension);
      }

      // --- Main DP over rows.
      for (unsigned row = 1; row <= sequence_size_v; ++row) {
        // Innermost loop over lanes: good vectorization candidate.
        for (unsigned lane = 0; lane < lanes; ++lane) {
          const sequence_t &sequence1 = *seq1_ptrs[lane];
          const sequence_t &sequence2 = *seq2_ptrs[lane];

          const score_t s1 = sequence1[row - 1];
          const score_t s2 = sequence2[col - 1];

          // Compute next score from diagonal direction with match/mismatch.
          score_t best_cell_score =
              static_cast<score_t>(last_diagonal_score[lane] +
                  (s1 == s2 ? match : mismatch));

          // Determine best score from diagonal, vertical, or horizontal direction.
          best_cell_score = std::max(best_cell_score, last_vertical_gap[lane]);
          best_cell_score = std::max(best_cell_score,
                                     horizontal_gap_column[lane][row]);

          // Cache next diagonal value and store optimum in score_column.
          const score_t prev_score = score_column[lane][row];
          last_diagonal_score[lane] = prev_score;
          score_column[lane][row] = best_cell_score;

          // Compute the next values for vertical and horizontal gaps.
          const score_t gap_opened =
              static_cast<score_t>(best_cell_score + gap_open);

          last_vertical_gap[lane] =
              static_cast<score_t>(last_vertical_gap[lane] + gap_extension);
          horizontal_gap_column[lane][row] =
              static_cast<score_t>(horizontal_gap_column[lane][row] +
                                   gap_extension);

          // Store optimum between gap open and gap extension.
          last_vertical_gap[lane] =
              std::max(last_vertical_gap[lane], gap_opened);
          horizontal_gap_column[lane][row] =
              std::max(horizontal_gap_column[lane][row], gap_opened);
        }
      }
    }

    // Write back results for each lane in this block.
    for (unsigned lane = 0; lane < lanes; ++lane) {
      result[base + lane] = score_column[lane][sequence_size_v];
    }
  }

  return result;
}