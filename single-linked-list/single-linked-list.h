#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template<typename Type>
class SingleLinkedList {
  struct Node {
    Node() = default;
    Node(const Type &val, Node *next)
        : value(val), next_node(next) {
    }
    Type value;
    Node *next_node = nullptr;
  };

  template<typename ValueType>
  class BasicIterator {
    friend class SingleLinkedList;

    explicit BasicIterator(Node *node) {
      node_ = node;
    }

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType *;
    using reference = ValueType &;

    BasicIterator() = default;

    BasicIterator(const BasicIterator<Type> &other) noexcept {
      node_ = other.node_;
    }

    BasicIterator &operator=(const BasicIterator &rhs) = default;

    [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept {
      return node_ == rhs.node_;
    }

    [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept {
      return node_ != rhs.node_;
    }

    [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept {
      return node_ == rhs.node_;
    }

    [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept {
      return node_ != rhs.node_;
    }

    BasicIterator &operator++() noexcept {
      assert(node_ != nullptr);
      node_ = node_->next_node;
      return *this;
    }

    BasicIterator operator++(int) noexcept {
      auto old_value(*this);
      ++(*this);
      return old_value;
    }

    [[nodiscard]] reference operator*() const noexcept {
      assert(node_ != nullptr);
      return node_->value;
    }

    [[nodiscard]] pointer operator->() const noexcept {
      assert(node_ != nullptr);
      return &node_->value;
    }

   private:
    Node *node_ = nullptr;
  };

 public:
  SingleLinkedList() {};

  SingleLinkedList(std::initializer_list<Type> values) {
    SingleLinkedList linked_list;
    FillLinkedList_(linked_list, values.begin(), values.end());
    swap(linked_list);
  }

  SingleLinkedList(const SingleLinkedList &other) {
    SingleLinkedList linked_list;
    FillLinkedList_(linked_list, other.begin(), other.end());
    swap(linked_list);
  }

  SingleLinkedList &operator=(const SingleLinkedList &rhs) {
    if (this == &rhs) {
      return *this;
    }
    SingleLinkedList copy(rhs);
    swap(copy);
    return *this;
  }

  ~SingleLinkedList() {
    Clear();
  }

  using value_type = Type;
  using reference = value_type &;
  using const_reference = const value_type &;

  using Iterator = BasicIterator<Type>;
  using ConstIterator = BasicIterator<const Type>;

  [[nodiscard]] Iterator begin() noexcept {
    return Iterator(head_.next_node);
  }

  [[nodiscard]] Iterator end() noexcept {
    return Iterator();
  }

  [[nodiscard]] ConstIterator begin() const noexcept {
    return ConstIterator(head_.next_node);
  }

  [[nodiscard]] ConstIterator end() const noexcept {
    return ConstIterator();
  }

  [[nodiscard]] ConstIterator cbegin() const noexcept {
    return ConstIterator(head_.next_node);
  }

  [[nodiscard]] ConstIterator cend() const noexcept {
    return ConstIterator();
  }

  [[nodiscard]] Iterator before_begin() noexcept {
    return Iterator(&head_);
  }

  [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
    return ConstIterator(const_cast<Node *>(&head_));
  }

  [[nodiscard]] ConstIterator before_begin() const noexcept {
    return ConstIterator(head_);
  }

  void swap(SingleLinkedList &other) noexcept {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
  }

  [[nodiscard]] size_t GetSize() const noexcept {
    return size_;
  }

  [[nodiscard]] bool IsEmpty() const noexcept {
    return !size_;
  }

  void Clear() noexcept {
    while (head_.next_node != nullptr) {
      auto current_node = head_.next_node;
      head_.next_node = head_.next_node->next_node;
      delete current_node;
    }
    size_ = 0;
  }

  void PushFront(const Type &value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
  }

  Iterator InsertAfter(ConstIterator pos, const Type &value) {
    assert(pos.node_ != nullptr);
    auto node = new Node(value, pos.node_->next_node);
    pos.node_->next_node = node;
    ++size_;
    return Iterator(pos.node_->next_node);
  }

  void PopFront() noexcept {
    if (IsEmpty()) {
      return;
    }
    const auto front = head_.next_node;
    head_.next_node = head_.next_node->next_node;
    delete front;
    --size_;
  }

  Iterator EraseAfter(ConstIterator pos) noexcept {
    assert(pos.node_ != nullptr);
    const auto node_to_delete = pos.node_->next_node;
    pos.node_->next_node = pos.node_->next_node->next_node;
    delete node_to_delete;
    --size_;
    return Iterator(pos.node_->next_node);
  }

 private:
  Node head_;
  size_t size_ = 0;

  template<typename CollectionIterator>
  void FillLinkedList_(SingleLinkedList &linked_list,
                       CollectionIterator it_begin,
                       CollectionIterator it_end) {
    Node **node_ptr = &linked_list.head_.next_node;
    for (auto it = it_begin; it != it_end; ++it) {
      *node_ptr = new Node(*it, nullptr);
      node_ptr = &((*node_ptr)->next_node);
      ++linked_list.size_;
    }
  }
};

template<typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
  lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return !(lhs == rhs);
}

template<typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return !(lhs > rhs);
}

template<typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return rhs < lhs;
}

template<typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
  return !(lhs < rhs);
}
