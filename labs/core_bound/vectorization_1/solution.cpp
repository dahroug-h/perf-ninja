#include "solution.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>

// The alignment algorithm which computes the alignment of the given sequence
// pairs.
// result_t compute_alignment(std::vector<sequence_t> const &sequences1,
//                            std::vector<sequence_t> const &sequences2) {
//   result_t result{};

//   for (size_t sequence_idx = 0; sequence_idx < sequences1.size();
//        ++sequence_idx) {
//     using score_t = int16_t;
//     using column_t = std::array<score_t, sequence_size_v + 1>;

//     sequence_t const &sequence1 = sequences1[sequence_idx];
//     sequence_t const &sequence2 = sequences2[sequence_idx];

//     /*
//      * Initialise score values.
//      */
//     score_t gap_open{-11};
//     score_t gap_extension{-1};
//     score_t match{6};
//     score_t mismatch{-4};

//     /*
//      * Setup the matrix.
//      * Note we can compute the entire matrix with just one column in memory,
//      * since we are only interested in the last value of the last column in the
//      * score matrix.
//      */
//     column_t score_column{};
//     column_t horizontal_gap_column{};
//     score_t last_vertical_gap{};

//     /*
//      * Initialise the first column of the matrix.
//      */
//     // horizontal_gap_column[0] = gap_open;
//     // last_vertical_gap = gap_open;

//     // for (size_t i = 1; i < score_column.size(); ++i) {

//     //   score_column[i] = last_vertical_gap;
//     //   horizontal_gap_column[i] = last_vertical_gap + gap_open;

//     //   last_vertical_gap += gap_extension;
//     // }



//     horizontal_gap_column[0] = gap_open;
//     last_vertical_gap = gap_open;

//     for (size_t i = 1; i < score_column.size(); ++i) {
//       score_t v = gap_open + gap_extension * (i - 1);
//       score_column[i] = v;
//       horizontal_gap_column[i] = v + gap_open;
//     }

    


    

//     /*
//      * Compute the main recursion to fill the matrix.
//      */
//     for (unsigned col = 1; col <= sequence2.size(); ++col) {
//       score_t last_diagonal_score = score_column[0]; // Cache last diagonal score to compute this cell.

//       score_column[0] = horizontal_gap_column[0];
//       last_vertical_gap = horizontal_gap_column[0] + gap_open;
//       horizontal_gap_column[0] += gap_extension;
      
//       score_t seq2_col_m1 = sequence2[col - 1];

//       for (unsigned row = 1; row <= sequence1.size(); ++row) {
//         // Compute next score from diagonal direction with match/mismatch.
//         score_t best_cell_score = last_diagonal_score + (sequence1[row - 1] == seq2_col_m1 ? match : mismatch);
//         last_diagonal_score = score_column[row];
          
//         // Determine best score from diagonal, vertical, or horizontal
//         // direction.

//         best_cell_score = std::max(best_cell_score, last_vertical_gap);
//         best_cell_score = std::max(best_cell_score, horizontal_gap_column[row]);

//         score_column[row] = best_cell_score;


//         best_cell_score += gap_open;
//         last_vertical_gap += gap_extension;
//         last_vertical_gap = std::max(last_vertical_gap, best_cell_score);

//         horizontal_gap_column[row] += gap_extension;
//         horizontal_gap_column[row] = std::max(horizontal_gap_column[row], best_cell_score);



//       }
//     }

//     // Report the best score.
//     result[sequence_idx] = score_column.back();
//   }

//   return result;
// }


constexpr unsigned BATCH = 4; // or 8

result_t compute_alignment(std::vector<sequence_t> const &sequences1,
                           std::vector<sequence_t> const &sequences2) {
  result_t result{};

  const size_t num_seqs = sequences1.size();
  assert(num_seqs == sequences2.size());

  for (size_t base = 0; base < num_seqs; base += BATCH) {
    const unsigned batch = static_cast<unsigned>(
        std::min<size_t>(BATCH, num_seqs - base));

    // For now, assume all sequences in this batch have same lengths
    // (vectorization lab usually fixes sequence_size_v).
    const unsigned seq_len1 = sequence_size_v; // sequence1 length
    const unsigned seq_len2 = sequence_size_v; // sequence2 length

    // Per-row DP state, but now *per lane* in [0, batch)
    using score_t = int16_t;

    // score_column[row][lane], horiz_gap[row][lane]
    std::array<std::array<score_t, BATCH>, sequence_size_v + 1> score_column{};
    std::array<std::array<score_t, BATCH>, sequence_size_v + 1> horizontal_gap{};
    std::array<score_t, BATCH> last_vertical_gap{};

    // (1) initialisation for each lane k in this batch
    for (unsigned lane = 0; lane < batch; ++lane) {
      score_column[0][lane] = 0;
      horizontal_gap[0][lane] = -11; // gap_open
      last_vertical_gap[lane] = -11;
      // fill first column (same as scalar code, but over rows & lanes)
      score_t gap_open      = -11;
      score_t gap_extension = -1;
      score_t lv = gap_open;
      for (unsigned row = 1; row < score_column.size(); ++row) {
        score_column[row][lane] = lv;
        horizontal_gap[row][lane] = lv + gap_open;
        lv += gap_extension;
      }
    }

    // (2) main DP over columns, but now inside each row we loop lanes
    for (unsigned col = 1; col <= seq_len2; ++col) {
      // For each lane, we have its own last_diagonal_score
      std::array<score_t, BATCH> last_diagonal_score;

      // init per lane from row=0
      for (unsigned lane = 0; lane < batch; ++lane) {
        last_diagonal_score[lane] = score_column[0][lane];
        score_column[0][lane]     = horizontal_gap[0][lane];
        last_vertical_gap[lane]   = horizontal_gap[0][lane] - 11; // + gap_open
        horizontal_gap[0][lane]  += -1; // gap_extension
      }

      // Now sweep rows
      for (unsigned row = 1; row <= seq_len1; ++row) {
        for (unsigned lane = 0; lane < batch; ++lane) {
          auto const& s1 = sequences1[base + lane];
          auto const& s2 = sequences2[base + lane];

          score_t gap_open      = -11;
          score_t gap_extension = -1;
          score_t match         = 6;
          score_t mismatch      = -4;

          score_t c1 = s1[row - 1];
          score_t c2 = s2[col - 1];

          // diagonal
          score_t best = last_diagonal_score[lane] +
                         (c1 == c2 ? match : mismatch);

          // prepare next diagonal: current column's previous value
          score_t diag_next = score_column[row][lane];

          // vertical & horizontal gaps
          best = std::max(best, last_vertical_gap[lane]);
          best = std::max(best, horizontal_gap[row][lane]);

          score_column[row][lane] = best;

          // update gaps
          score_t open = static_cast<score_t>(best + gap_open);

          last_vertical_gap[lane] =
              std::max<score_t>(last_vertical_gap[lane] + gap_extension, open);

          horizontal_gap[row][lane] =
              std::max<score_t>(horizontal_gap[row][lane] + gap_extension, open);

          last_diagonal_score[lane] = diag_next;
        }
      }
    }

    // write back results
    for (unsigned lane = 0; lane < batch; ++lane) {
      result[base + lane] = score_column.back()[lane];
    }
  }

  return result;
}