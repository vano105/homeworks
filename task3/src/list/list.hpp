#pragma once
#include <mutex>
#include <optional>

template <typename K, typename V> struct Node {
  K key;
  V value;
  Node *next;
  std::mutex mtx;

  Node(K k, V v) : key(k), value(v), next(nullptr) {}
  ~Node() { delete next; }
};

template <typename K, typename V> class List {
  Node<K, V> *head;
  std::mutex headMtx;

public:
  List() : head(nullptr) {}
  ~List() { delete head; }

  void insert(K key, V value);
  bool remove(K key);
  std::optional<V> find(K key, V &out);
};
