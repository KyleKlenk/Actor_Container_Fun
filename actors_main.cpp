#include <iostream>
#include "caf/all.hpp"
#include <array>
#include <chrono>
#include <random>
#include <list>
using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(test, first_custom_type_id)
  CAF_ADD_TYPE_ID(test, (std::array<int, 5>))
  CAF_ADD_TYPE_ID(test, (std::vector<int>))
CAF_END_TYPE_ID_BLOCK(test)

std::vector<std::vector<int>> rows;
std::vector<std::vector<int>> cols;
std::vector<std::vector<int>> a;
std::vector<std::vector<int>> b;




std::vector<std::vector<int>> generate_random_matrix(int n) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 100;  // Generate random values between 0 and 99
        }
    }
    return matrix;
}


std::vector<int> generate_random_vector(int n) {
    std::vector<int> a(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 0; i < n; i++) {
        a[i] = dis(gen);  // Generate random values between 0 and 99
    }
    return a;
}

std::vector<int> vector_add(const std::vector<int>& a, const std::vector<int>& b) {
    int n = a.size();
    std::vector<int> c(n);
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
    return c;
}


int dot_product(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size())
        throw std::invalid_argument("Vectors must be the same size");

    int product = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        product += a[i] * b[i];
    }

    return product;
}



behavior point_actor(event_based_actor *self) {
  return {
    [=](const int row) {
      auto sender_addr = caf::actor_cast<caf::actor>(self->current_sender());
      for (size_t column = 0; column < b.size(); ++column) {
        auto val = dot_product(a[row], b[column]);
        self->send(sender_addr, val);
      }
    }
  };
}

struct point {
  int vectors_recieved = 0;
  std::vector<std::vector<int>> c;
  std::vector<std::vector<int>> vecs;
  std::vector<int> vec;
  std::vector<int> vec2;
};



behavior collector_actor(stateful_actor<point> *self) {

  int N = 4000;
  a = generate_random_matrix(N);
  b = generate_random_matrix(N);

  // Get all rows from matrix a
  for (auto &row : a) {
    rows.push_back(row);
  }

  // get all columns from matrix b
  for (size_t i = 0; i < b.size(); ++i) {
    std::vector<int> col;
    col.resize(b.size());
    for (size_t j = 0; j < b.size(); ++j) {
      col[j] = b[j][i];
    }
    cols.push_back(col);
  }

  self->state.c.resize(N);
  for (auto &row : self->state.c)
    row.resize(N);

  auto row_it = rows.begin();
  auto col_it = cols.begin();
  
  for (int i = 0; i < N; ++i) {
    auto act = self->spawn(point_actor);
    self->send(act, i);
  }


  return {
    [=](int val) {
    }
  };

}






void caf_main(actor_system& system) {
  scoped_actor self{system};
  self->spawn(collector_actor);




}

CAF_MAIN(id_block::test)