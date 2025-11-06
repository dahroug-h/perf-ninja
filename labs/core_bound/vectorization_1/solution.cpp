#include "solution.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>

namespace {
using score_t = int16_t;
using column_t = std::array<score_t, sequence_size_v + 1>;

/*
 * Initialise score values.
 */
constexpr score_t gap_open{-11};
constexpr score_t gap_extension{-1};
constexpr score_t match{6};
constexpr score_t mismatch{-4};

struct init_columns_t {
  column_t score_column;
  column_t horizontal_gap_column;
  score_t last_vertical_gap;
};

inline init_columns_t create_init_columns() {
  column_t score_column{};
  column_t horizontal_gap_column{};
  score_t last_vertical_gap{};

  /*
   * Initialise the first column of the matrix.
   */
  horizontal_gap_column[0] = gap_open;
  last_vertical_gap = gap_open;

  for (size_t i = 1; i < score_column.size(); ++i) {
    score_column[i] = last_vertical_gap;
    horizontal_gap_column[i] = last_vertical_gap + gap_open;
    last_vertical_gap += gap_extension;
  }


  return init_columns_t {
    .score_column = score_column,
    .horizontal_gap_column = horizontal_gap_column,
    .last_vertical_gap = last_vertical_gap,
  };
}

}

// The alignment algorithm which computes the alignment of the given sequence
// pairs.
result_t compute_alignment(std::vector<sequence_t> const &sequences1,
                           std::vector<sequence_t> const &sequences2) {
  result_t result{};

  const auto init_columns = create_init_columns();

  for (size_t sequence_idx = 0; sequence_idx < sequences1.size();
       ++sequence_idx) {

    sequence_t const &sequence1 = sequences1[sequence_idx];
    sequence_t const &sequence2 = sequences2[sequence_idx];

    /*
     * Setup the matrix.
     * Note we can compute the entire matrix with just one column in memory,
     * since we are only interested in the last value of the last column in the
     * score matrix.
     */
    column_t score_column = init_columns.score_column;
    column_t horizontal_gap_column = init_columns.horizontal_gap_column;
    score_t last_vertical_gap = init_columns.last_vertical_gap;

    /*
     * Compute the main recursion to fill the matrix.
     */
    for (unsigned col = 1; col <= sequence2.size(); ++col) {
      score_t last_diagonal_score =
          score_column[0]; // Cache last diagonal score to compute this cell.
      score_column[0] = horizontal_gap_column[0];
      last_vertical_gap = horizontal_gap_column[0] + gap_open;
      horizontal_gap_column[0] += gap_extension;

      for (unsigned row = 1; row <= sequence1.size(); ++row) {
        // Compute next score from diagonal direction with match/mismatch.
        score_t best_cell_score =
            last_diagonal_score +
            (sequence1[row - 1] == sequence2[col - 1] ? match : mismatch);
        // Determine best score from diagonal, vertical, or horizontal
        // direction.
        best_cell_score = std::max(best_cell_score, last_vertical_gap);
        best_cell_score = std::max(best_cell_score, horizontal_gap_column[row]);
        // Cache next diagonal value and store optimum in score_column.
        last_diagonal_score = score_column[row];
        score_column[row] = best_cell_score;
        // Compute the next values for vertical and horizontal gap.
        best_cell_score += gap_open;
        last_vertical_gap += gap_extension;
        horizontal_gap_column[row] += gap_extension;
        // Store optimum between gap open and gap extension.
        last_vertical_gap = std::max(last_vertical_gap, best_cell_score);
        horizontal_gap_column[row] =
            std::max(horizontal_gap_column[row], best_cell_score);
      }
    }

    // Report the best score.
    result[sequence_idx] = score_column.back();
  }

  return result;
}
