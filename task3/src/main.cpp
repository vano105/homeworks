#include "hash_table/hash_table.hpp"
#include <cassert>
#include <iostream>
#include <random>

void test_concurrent_put() {
  HashMap<int, Value, 100> table;
  const int num_keys = 1000;
  const int num_threads = 4;
  std::vector<std::thread> threads;

  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&table, i, num_threads, num_keys]() {
      int start = i * (num_keys / num_threads) + 1;
      int end = (i + 1) * (num_keys / num_threads);
      for (int key = start; key <= end; ++key) {
        table.put(key, {"value", key});
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  for (int key = 1; key <= num_keys; ++key) {
    Value value;
    assert((table.get(key, value) != std::nullopt) && value.num == key);
  }
  std::cout << "[OK] Все ключи добавлены\n";
}

void test_random_operations() {
  HashMap<int, Value, 100> table;
  const int M = 1000;
  const int num_threads = 4;
  const int operations_per_thread = 10000;
  std::vector<std::thread> threads;

  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&table, M]() {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> op_dist(0, 2);
      std::uniform_int_distribution<> key_dist(1, M);

      for (int j = 0; j < operations_per_thread; ++j) {
        int key = key_dist(gen);
        int op = op_dist(gen);
        switch (op) {
        case 0: // Put
          table.put(key, {"value", key});
          break;
        case 1: // Remove
          table.remove(key);
          break;
        case 2: // Check
          Value value;
          table.get(key, value);
          break;
        }
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }
  std::cout << "[OK] Тест случайных операций завершен\n";
}

int main() {
  test_concurrent_put();
  test_random_operations();
  return 0;
}
