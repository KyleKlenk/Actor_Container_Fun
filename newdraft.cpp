#include "caf/all.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace std::chrono;
using namespace caf;

struct matrix;
struct tuple;
struct pairing;

CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)

  CAF_ADD_TYPE_ID(custom_types_1, (vector<vector<int>>) )
  CAF_ADD_TYPE_ID(custom_types_1, (std::tuple<int, int>) )
  CAF_ADD_TYPE_ID(custom_types_1, (std::tuple<int, int, int>) )

CAF_END_TYPE_ID_BLOCK(custom_types_1)

behavior columnActor(event_based_actor* self) {
  return {
    [=](char seq1, char seq2, int row, int above, int matchingScore,
        int misMatchScore) -> std::tuple<int, int> {
      int match = (seq1 == seq2 ? matchingScore : misMatchScore);
      int score = std::max({match, above, 0});
      return make_tuple(score, row);
    },
  };
}

behavior rowActor(event_based_actor* self) {
  return {
    [=](char seq1, char seq2, int row, int col, int above, int left,
        int diagonal, int matchingScore,
        int misMatchScore) -> std::tuple<int, int, int> {
      int match = diagonal + (seq1 == seq2 ? matchingScore : misMatchScore);
      int score = std::max({match, left, above, 0});
      return make_tuple(score, row, col);
      ;
    },
  };
}

struct manager_state {
  string seq1
    = "AGCTCTACCAAGCTCTACCAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCT"
      "CTACCTCACAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACA"
      "GCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACGAGCTCTACCAGCAGCTCTA"
      "AGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACCAGCAGCTCTACCAGCAGCTCTAAGCTCT"
      "ACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACTAGAGCTCTACCAGCAGCTCTAAGCTCTACCTCAC"
      "TGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCT"
      "CACTGAATGCTCTACCTCACCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTC"
      "TACCTCACCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCT"
      "ACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTACCAGCAGCT"
      "CTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTAAGCTCTACCTCACTGAATG"
      "CCCTCACTGAATGCTCTACCTCAC"
      "TGAATGCTCACTGAATGC",
    seq2
    = "AGCTCTACCAAGCTCTACCAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCT"
      "CTACCTCACAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACA"
      "GCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACGAGCTCTACCAGCAGCTCTA"
      "AGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACCAGCAGCTCTACCAGCAGCTCTAAGCTCT"
      "ACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACTAGAGCTCTACCAGCAGCTCTAAGCTCTACCTCAC"
      "TGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCT"
      "CACTGAATGCTCTACCTCACCTCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTC"
      "TACCTCACCTACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCT"
      "ACCAGCAGCTCTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTACCAGCAGCT"
      "CTAAGCTCTACCTCACTGAATGCCCTCACTGAATGCTCTACCTCACAGCTCTAAGCTCTACCTCACTGAATG"
      "CCCTCACTGAATGCTCTACCTCAC"
      "TGAATGCTCACTGAATGC";
  int m = static_cast<int>(seq1.length()), n = static_cast<int>(seq2.length()),
      matchingScore = 2, misMatchScore = -1, gapScore = -2, maxScore = 0,
      maxScoreIndexRow = 0, maxScoreIndexCol = 0;
  vector<vector<int>> scoreMatrix;
  actor buddy1; // Address Actor
  actor buddy2; // Calculator
};
behavior managerActor(stateful_actor<manager_state>* self) {
  self->state.buddy1 = self->spawn(columnActor);
  self->state.buddy2 = self->spawn(rowActor);
  auto start_time = high_resolution_clock::now();

  self->state.scoreMatrix
    = vector<vector<int>>(self->state.m + 1, vector<int>(self->state.n + 1, 0));
  self->send(self->state.buddy1, self->state.seq1[0], self->state.seq2[0], 1, 0,
             self->state.matchingScore, self->state.misMatchScore);

  return {
    [=](const std::tuple<int, int> address) {
      auto [score, row] = address;
      self->state.scoreMatrix[row][1] = score;
      if (self->state.maxScore < score) {
        self->state.maxScore = score;
        self->state.maxScoreIndexRow = row;
        self->state.maxScoreIndexCol = 1;
      }
      int above = score + self->state.gapScore;
      if (row != self->state.m) {
        self->send(self->state.buddy1, self->state.seq1[row],
                   self->state.seq2[0], row + 1, above,
                   self->state.matchingScore, self->state.misMatchScore);
        int left = score + self->state.gapScore;
        int above = self->state.scoreMatrix[row - 1][2] + self->state.gapScore;
        int diagonal = self->state.scoreMatrix[row - 1][1];
        self->send(self->state.buddy2, self->state.seq1[row - 1],
                   self->state.seq2[1], row, 2, above, left, diagonal,
                   self->state.matchingScore, self->state.misMatchScore);
      } else {
        int left = score + self->state.gapScore;
        int above = self->state.scoreMatrix[row - 1][2] + self->state.gapScore;
        int diagonal = self->state.scoreMatrix[row - 1][1];
        self->send(self->state.buddy2, self->state.seq1[row - 1],
                   self->state.seq2[1], row, 2, above, left, diagonal,
                   self->state.matchingScore, self->state.misMatchScore);
      }
    },
    [=](const std::tuple<int, int, int> address) {
      auto [score, row, col] = address;
      self->state.scoreMatrix[row][col] = score;
      if (self->state.maxScore < score) {
        self->state.maxScore = score;
        self->state.maxScoreIndexRow = row;
        self->state.maxScoreIndexCol = col;
      }
      if (row == self->state.m & col == self->state.n) {
        // score Matrix is Done then Do traceback
        auto end_time = high_resolution_clock::now();
        // aout(self) << "m: " << self->state.m << endl;
        // aout(self) << "n: " << self->state.n << endl;
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        aout(self) << "Final Max Score:" << self->state.maxScore << endl;
        aout(self) << "Final run time: " << duration << endl;
        // aout(self) << "Final Score Matrix:" << endl;

        // for (const auto& row : self->state.scoreMatrix) {
        //   for (int cell : row) {
        //     aout(self) << cell << " ";
        //   }
        //   aout(self) << endl;
        // }
      } else {
        if (col == self->state.n) {
        } else {
          int left = score + self->state.gapScore;
          int above = self->state.scoreMatrix[row - 1][col + 1]
                      + self->state.gapScore;
          int diagonal = self->state.scoreMatrix[row - 1][col];
          self->send(self->state.buddy2, self->state.seq1[row - 1],
                     self->state.seq2[col], row, col + 1, above, left, diagonal,
                     self->state.matchingScore, self->state.misMatchScore);
        }
      }
    },
  };
}

void caf_main(actor_system& sys) {
  auto manager_actor = sys.spawn(managerActor);

  // Initial message to start calculations can be sent from here
}

CAF_MAIN(id_block::custom_types_1)