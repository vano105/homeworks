#include "common.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

void naiveVersion(std::vector<int> &array) {
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    auto [a, b, c] = generateUniqueIndices();
    int sum = array[a] + array[b] + array[c];
    array[a] = sum;
    array[b] = sum;
    array[c] = sum;
  }
}

void globalMutexVersion(std::vector<int> &array,
                        std::vector<std::mutex> &mutexes) {
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    auto [a, b, c] = generateUniqueIndices();
    std::lock_guard<std::mutex> lock(mutexes[0]);
    int sum = array[a] + array[b] + array[c];
    array[a] = sum;
    array[b] = sum;
    array[c] = sum;
  }
}

void orderedLockVersion(std::vector<int> &array,
                        std::vector<std::mutex> &mutexes) {
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    auto [a, b, c] = generateUniqueIndices();

    size_t indices[3] = {a, b, c};
    std::sort(std::begin(indices), std::end(indices));

    std::unique_lock<std::mutex> lock1(mutexes[indices[0]], std::defer_lock);
    std::unique_lock<std::mutex> lock2(mutexes[indices[1]], std::defer_lock);
    std::unique_lock<std::mutex> lock3(mutexes[indices[2]], std::defer_lock);
    std::lock(lock1, lock2, lock3);

    int sum = array[a] + array[b] + array[c];
    array[a] = sum;
    array[b] = sum;
    array[c] = sum;
  }
}

void trylockVersion(std::vector<int> &array, std::vector<std::mutex> &mutexes) {
  for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
    auto [a, b, c] = generateUniqueIndices();

    while (true) {
      std::unique_lock<std::mutex> lock1(mutexes[a], std::try_to_lock);
      std::unique_lock<std::mutex> lock2(mutexes[b], std::try_to_lock);
      std::unique_lock<std::mutex> lock3(mutexes[c], std::try_to_lock);

      if (lock1.owns_lock() && lock2.owns_lock() && lock3.owns_lock()) {
        int sum = array[a] + array[b] + array[c];
        array[a] = sum;
        array[b] = sum;
        array[c] = sum;
        break;
      }
    }
  }
}

template <typename func> void runTest(func &&f, const std::string &name) {
  std::vector<int> array(ARRAY_SIZE, 1);
  std::vector<std::mutex> mutexes(ARRAY_SIZE);
  std::vector<std::thread> threads(NUM_THREADS);

  auto start = std::chrono::high_resolution_clock::now();

  for (auto &thread : threads) {
    thread = std::thread(f, std::ref(array), std::ref(mutexes));
  }
  for (auto &thread : threads) {
    thread.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << name << " time: " << duration.count() << " ms\n";
}

int main() {
  runTest([](std::vector<int> &arr,
             std::vector<std::mutex> &) { naiveVersion(arr); },
          "Naive");
  runTest([](std::vector<int> &arr,
             std::vector<std::mutex> &mtxs) { globalMutexVersion(arr, mtxs); },
          "GlobalMutex");
  runTest([](std::vector<int> &arr,
             std::vector<std::mutex> &mtxs) { trylockVersion(arr, mtxs); },
          "TryLock");
  runTest([](std::vector<int> &arr,
             std::vector<std::mutex> &mtxs) { orderedLockVersion(arr, mtxs); },
          "OrderedMutexes");

  return 0;
}
