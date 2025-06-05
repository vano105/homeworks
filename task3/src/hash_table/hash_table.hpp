#pragma once
#include "../list/list.hpp"
#include <string>
#include <thread>
#include <vector>

struct Value {
  std::string str;
  int num;

  bool operator==(const Value &other) const {
    return str == other.str && num == other.num;
  }
};

namespace std {
template <> struct hash<Value> {
  size_t operator()(const Value &v) const {
    return hash<string>{}(v.str) ^ (hash<int>{}(v.num) << 1);
  }
};
} // namespace std

template <typename K, typename V, size_t BUCKETS = 16> class HashMap {
  std::vector<List<K, V>> table;
  std::hash<K> hasher;
  std::atomic<size_t> total_count{0};

public:
  HashMap() : table(BUCKETS) {}

  void put(K key, V value) {
    size_t idx = hasher(key) % BUCKETS;
    table[idx].insert(key, value);
    total_count++;
  }

  bool remove(K key) {
    size_t idx = hasher(key) % BUCKETS;
    if (table[idx].remove(key)) {
      total_count--;
      return true;
    }
    return false;
  }

  std::optional<V> get(K key, V &value) {
    size_t idx = hasher(key) % BUCKETS;
    return table[idx].find(key, value);
  }

  size_t size() const { return total_count.load(); }

  void clear() {
    for (auto &bucket : table) {
      List<K, V> new_list;
      bucket = std::move(new_list);
    }
    total_count = 0;
  }
};
