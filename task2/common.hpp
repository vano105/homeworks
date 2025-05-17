#pragma once
#include <cstdlib>
#include <stdexcept>
#include <tuple>

const int ARRAY_SIZE = 100;
const int NUM_THREADS = 4;
const int OPERATIONS_PER_THREAD = 10000;

static std::tuple<std::size_t, std::size_t, std::size_t>
generateUniqueIndices() {
  if (ARRAY_SIZE < 2) {
    throw std::runtime_error("can't get 3 unique numbers from 2 numbers");
  }

  std::size_t i = rand() % ARRAY_SIZE;
  std::size_t j = rand() % ARRAY_SIZE;
  while (j == i) {
    j = rand() % ARRAY_SIZE;
  }
  std::size_t k = rand() % ARRAY_SIZE;
  while (k == i || k == j) {
    k = rand() % ARRAY_SIZE;
  }
  return {i, j, k};
}
