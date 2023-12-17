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

// std::vector<std::vector<int>> rows;
// std::vector<std::vector<int>> cols;
std::vector<std::vector<int>> a;
std::vector<std::vector<int>> b;




// std::vector<std::vector<int>> generate_random_matrix(int n) {
//     std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             matrix[i][j] = rand() % 100;  // Generate random values between 0 and 99
//         }
//     }
//     return matrix;
// }


// std::vector<int> generate_random_vector(int n) {
//     std::vector<int> a(n);
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, 99);
//     for (int i = 0; i < n; i++) {
//         a[i] = dis(gen);  // Generate random values between 0 and 99
//     }
//     return a;
// }

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



// behavior collector_actor(stateful_actor<point> *self) {

//   // int N = 4000;
//   // a = generate_random_matrix(N);
//   // b = generate_random_matrix(N);

//   // // Get all rows from matrix a
//   for (auto &row : a) {
//     rows.push_back(row);
//   }

//   // get all columns from matrix b
//   for (size_t i = 0; i < b.size(); ++i) {
//     std::vector<int> col;
//     col.resize(b.size());
//     for (size_t j = 0; j < b.size(); ++j) {
//       col[j] = b[j][i];
//     }
//     cols.push_back(col);
//   }

//   self->state.c.resize(N);
//   for (auto &row : self->state.c)
//     row.resize(N);

//   auto row_it = rows.begin();
//   auto col_it = cols.begin();
  
//   for (int i = 0; i < N; ++i) {
//     self->send(self->spawn(point_actor), i);
//   }


//   return {
//     [=](int val) {
//     }
//   };

// }


behavior loop_actor(event_based_actor *self, int a) {
  if (a == 0) {
    a++;
    aout(self) << "Spawning actor: id: " << a << std::endl;
    for (int i = 0; i < 2; ++i) {
      auto test = self->spawn(loop_actor, a+i);
      self->send(test, "");
    }
  } 
  return { 
    [=](const int row) {
      aout(self) << "Recieved: id: " << a << std::endl;
    }, 
    [=](std::string& test) {
      aout(self) << "Looping: id: " << a << std::endl;
      // while(true) {} 
      auto start = std::chrono::high_resolution_clock::now();
      auto end = start;

      while (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 10) { // 10 seconds
          // Simulate computation
          end = std::chrono::high_resolution_clock::now();
      }
    }
  };
}




behavior generate_random_matrix(event_based_actor *self) {
  return {
    [=](int n) {
      aout(self) << "Here\n";
      std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, 99);
      for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
              matrix[i][j] = dis(gen);  // Generate random values between 0 and 99
          }
      }
      aout(self) << "Done" << std::endl;
      self->quit();
      return;
    }
  };
}

// int rows = 50000;
// int cols = 50000;
// std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));

behavior generate_random_vector(event_based_actor *self) {
  return {
    [=](int n, int id) {
      std::vector<int> local_vec(n);
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, 99);
      for (int i = 0; i < n; i++) {
          local_vec[i] = dis(gen);  // Generate random values between 0 and 99
      }
      // matrix[id] = local_vec;
      return local_vec;
    }
  };

}


struct some_struct {
  int rows = 25000;
  int cols = 25000;
  std::vector<std::vector<int>> matrix;
  int size = 0;
};

behavior parent_actor(stateful_actor<some_struct> *self) {
  return {
    [=](const int size) {
        self->state.matrix.resize(size);
        for (int i = 0; i < size; ++i) {
          auto test = self->spawn(generate_random_vector);
          self->send(test, size, i);
        }
    },
    [=](const std::vector<int>& vec) {
      // aout(self) << "Recieved: id: " << std::endl;
      self->state.matrix[self->state.size] = vec;
      self->state.size++;
    }
  };
}


// behavior populate_actor(event_based_actor *self) {
//   return {
//     [=](const std::vector<int>& vec) {
//       // for (int i = rank * rows_per_process; i < (rank + 1) * rows_per_process; ++i) {
//       //   for (int j = 0; j < cols; ++j) {
//       //       matrix[i][j] = dis(gen); // Replace this with your matrix generation logic
//       //   }
//     }
//     }
//   };
// }


long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

behavior fib_actor(event_based_actor *self) {
  return {
    [=](const int i) {
      aout(self) << "F(" << i << ") = " << fibonacci(i) << std::endl;
    }
  };
}







void caf_main(actor_system& system) {
  scoped_actor self{system};
  aout(self) << " ######## Starting Test ######## \n";
  int size = 60000;
  aout(self) << "\t timing generating matrix of size: " << size << "\n";
  // auto test1 = self->spawn(generate_random_matrix);
  // self->send(test1, size);
  // auto parent = self->spawn(loop_actor, 0);

  // std::this_thread::sleep_for(std::chrono::seconds(5));
  int i = 44; 
  
  for (i = 0; i < 100000; ++i) {
    self->send(self->spawn(fib_actor), 32);
  }
  



  // aout(self) << "\t timing generating matrix of size: " << size << "\n";
  // auto test2 = self->spawn(parent_actor);
  // self->send(test2, size);

  // auto test3 = self->spawn(parent_actor);
  // self->send(test3, size);
}

CAF_MAIN(id_block::test)