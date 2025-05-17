#include "list.hpp"
#include <memory>

template <typename K, typename V> void List<K, V>::insert(K key, V value) {
  Node<K, V> *newNode = new Node<K, V>(key, value);
  if (!head) {
    head = newNode;
    return;
  }

  Node<K, V> *curr = nullptr;
  Node<K, V> *prev = nullptr;

  {
    std::lock_guard<std::mutex> hLock(headMtx);
    curr = head;
    if (curr)
      curr->mtx.lock();
  }

  while (curr) {
    if (prev)
      prev->mtx.unlock();
    prev = curr;
    curr = curr->next;
    if (curr)
      curr->mtx.lock();
  }

  {
    std::lock_guard<std::mutex> hLock(headMtx);
    if (!prev) {
      newNode->next = head;
      head = newNode;
    } else {
      newNode->next = curr;
      prev->next = newNode;
    }
  }

  if (prev)
    prev->mtx.unlock();
  if (curr)
    curr->mtx.unlock();
}

template <typename K, typename V> bool List<K, V>::remove(K key) {
  if (!head)
    return false;
  Node<K, V> *prev = nullptr;
  Node<K, V> *curr = nullptr;

  {
    std::lock_guard<std::mutex> hLock(headMtx);
    curr = head;
    if (curr)
      curr->mtx.lock();
  }

  while (curr) {
    if (curr->key == key) {
      if (prev) {
        prev->mtx.lock();
        prev->next = curr->next;
        prev->mtx.unlock();
      } else {
        std::lock_guard<std::mutex> hLock(headMtx);
        head = curr->next;
      }

      curr->mtx.unlock();
      delete curr;
      return true;
    }

    Node<K, V> *oldPrev = prev;
    prev = curr;
    curr = curr->next;

    if (oldPrev)
      oldPrev->mtx.unlock();
    if (curr)
      curr->mtx.lock();
  }

  if (prev)
    prev->mtx.unlock();
  return false;
}

template <typename K, typename V>
std::optional<V> List<K, V>::find(K key, V &out) {
  Node<K, V> *prev = nullptr;
  Node<K, V> *curr = nullptr;

  {
    std::lock_guard<std::mutex> hLock(headMtx);
    curr = head;
    if (curr)
      curr->mtx.lock();
  }

  while (curr) {
    if (curr->key == key) {
      V value = curr->value;
      curr->mtx.unlock();
      if (prev)
        prev->mtx.unlock();
      return value;
    }

    Node<K, V> *oldPrev = prev;
    prev = curr;
    curr = curr->next;

    if (oldPrev)
      oldPrev->mtx.unlock();
    if (curr)
      curr->mtx.lock();
  }

  if (prev)
    prev->mtx.unlock();
  return std::nullopt;
}
