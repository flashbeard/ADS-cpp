#pragma once

#include <algorithm>
#include <iostream>

#include "Bimap.h"

template <typename T, typename Tag> struct Node {
  T value;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;

  Node(const Node &other) = default;

  explicit Node(const T &value) : value(value) {}

  explicit Node(T &&value) noexcept : value(std::move(value)) {}

  ~Node() = default;

  static void set_parent(Node *son, Node *parent) noexcept {
    if (son)
      son->parent = parent;
  }

  static void zig(Node *x) noexcept {
    Node *p = x->parent;

    if (p->left == x) {
      Node *b = x->right;
      x->right = p;
      p->left = b;
      if (b != nullptr)
        b->parent = p;

    } else {
      Node *b = x->left;
      x->left = p;
      p->right = b;
      if (b != nullptr)
        b->parent = p;
    }
    x->parent = nullptr;
    p->parent = x;
  }

  static void zig_zig(Node *x) noexcept {
    Node *p = x->parent;
    Node *g = p->parent;

    if (p->left == x) {
      Node *b = x->right;
      Node *c = p->right;

      x->parent = g->parent;
      x->right = p;

      p->parent = x;
      p->left = b;
      p->right = g;

      g->parent = p;
      g->left = c;

      if (b != nullptr)
        b->parent = p;
      if (c != nullptr)
        c->parent = g;

      if (x->parent != nullptr) {
        if (x->parent->left == g)
          x->parent->left = x;
        else
          x->parent->right = x;
      }
    } else {
      Node *b = p->left;
      Node *c = x->left;

      x->parent = g->parent;
      x->left = p;

      p->parent = x;
      p->left = g;
      p->right = c;

      g->parent = p;
      g->right = b;

      if (b != nullptr)
        b->parent = g;
      if (c != nullptr)
        c->parent = p;

      if (x->parent != nullptr) {
        if (x->parent->left == g)
          x->parent->left = x;
        else
          x->parent->right = x;
      }
    }
  }

  static void zig_zag(Node *x) noexcept {
    Node *p = x->parent;
    Node *g = p->parent;

    if (p->right == x) {
      Node *b = x->left;
      Node *c = x->right;

      x->left = p;
      x->right = g;
      x->parent = g->parent;

      p->right = b;
      p->parent = x;

      g->left = c;
      g->parent = x;

      if (b != nullptr)
        b->parent = p;
      if (c != nullptr)
        c->parent = g;

      if (x->parent != nullptr) {
        if (x->parent->left == g)
          x->parent->left = x;
        else
          x->parent->right = x;
      }
    } else {
      Node *b = x->left;
      Node *c = x->right;

      x->left = g;
      x->right = p;
      x->parent = g->parent;

      p->left = c;
      p->parent = x;

      g->right = b;
      g->parent = x;

      if (b != nullptr)
        b->parent = g;
      if (c != nullptr)
        c->parent = p;

      if (x->parent != nullptr) {
        if (x->parent->left == g)
          x->parent->left = x;
        else
          x->parent->right = x;
      }
    }
  }

  static Node *splay(Node *x) noexcept {
    if (x == nullptr)
      return x;
    while (x->parent != nullptr) {
      Node *p = x->parent;
      Node *g = p->parent;

      if (g == nullptr) {
        zig(x);
      } else {
        if ((g->left == p && p->left == x) || (g->right == p && p->right == x))
          zig_zig(x);
        else
          zig_zag(x);
      }
    }
    return x;
  }
  template <typename Comp>
  static bool equals(const T &val1, const T &val2, const Comp &comparator) {
    return (!comparator(val2, val1) && !comparator(val1, val2));
  }

  template <typename Comp>
  static Node *find_lower_bound(Node *cur_node, const T &val,
                                const Comp &comparator) {
    if (cur_node == nullptr)
      return nullptr;
    if (comparator(cur_node->value, val))
      return find_lower_bound(cur_node->right, val, comparator);
    Node *left_result = find_lower_bound(cur_node->left, val, comparator);
    if (left_result == nullptr) {
      return cur_node;
    }
    return left_result;
  }

  /**
   * @return pair where
   *    left tree < val
   *    right tree >= val
   */
  template <typename Comp>
  static std::pair<Node *, Node *> split(Node *root, const T &val,
                                         const Comp &comparator) {
    if (root == nullptr)
      return {nullptr, nullptr};
    Node *cur_node = find_lower_bound(root, val, comparator);
    if (cur_node == nullptr) {
      return {root, nullptr};
    }
    Node *left_node = root->left;
    set_parent(left_node, nullptr);
    root->left = nullptr;
    return {left_node, root};
  }
};

template <typename T, typename Tag, typename Comparator> struct Tree {
  using node = Node<T, Tag>;

  explicit Tree(Comparator &&comparator) noexcept
      : comparator(std::move(comparator)){};

  Tree(Tree const &other) : comparator(other.comparator){};

  Tree(Tree &&other) noexcept
      : root(std::move(other.root)), comparator(std::move(comparator)) {
    other.root = nullptr;
  }

  Tree &operator=(Tree const &other) = default;

  Tree &operator=(Tree &&other) noexcept {
    comparator = std::move(other.comparator);
    root = other.root;
    other.root = nullptr;
  }

  ~Tree() = default;

  void swap(Tree &other) {
    std::swap(root, other.root);
    Comparator tmp = std::move(other.comparator);
    other.comparator = std::move(comparator);
    comparator = std::move(tmp);
  }

  node *get_next(node *cur) noexcept {
    root = node::splay(cur);
    cur = root->right;
    if (cur != nullptr) {
      while (cur->left != nullptr) {
        cur = cur->left;
      }
    }
    return cur;
  }

  node *get_prev(node *cur) noexcept {
    root = node::splay(cur);
    cur = root->left;
    if (cur != nullptr) {
      while (cur->right != nullptr) {
        cur = cur->right;
      }
    }
    return cur;
  }

  node *find_last() noexcept {
    if (root == nullptr) {
      return nullptr;
    }
    root = node::splay(root);
    node *cur_node = root;
    while (cur_node->right != nullptr) {
      cur_node = cur_node->right;
    }
    return cur_node;
  }

  node *find_first() noexcept {
    if (root == nullptr) {
      return nullptr;
    }
    root = node::splay(root);
    node *cur_node = root;
    while (cur_node->left != nullptr) {
      cur_node = cur_node->left;
    }
    return cur_node;
  }

  bool contains(const T &val) {
    if (root == nullptr) {
      return false;
    }
    node *cur_node = node::find_lower_bound(root, val, comparator);
    if (cur_node) {
      root = node::splay(cur_node);
      return node::equals(root->value, val, comparator);
    }
    return false;
  }

  node *find_value(const T &val) {
    if (root == nullptr) {
      return nullptr;
    }
    node *cur_node = node::find_lower_bound(root, val, comparator);
    if (cur_node) {
      root = node::splay(cur_node);
      if (node::equals(root->value, val, comparator)) {
        return root;
      }
    }
    return nullptr;
  }

  node *find_grater_or_equal(const T &val, bool equal_permissible = true) {
    if (root == nullptr) {
      return nullptr;
    }
    node *result = node::find_lower_bound(root, val, comparator);
    if (result && node::equals(val, result->value, comparator) &&
        !equal_permissible) {
      result = get_next(result);
    }
    return result;
  }

  node *add(node *new_node) {
    if (root == nullptr) {
      root = new_node;
      return new_node;
    }
    auto p = node::split(root, new_node->value, comparator);
    root = new_node;
    new_node->left = p.first;
    new_node->right = p.second;
    node::set_parent(p.first, root);
    node::set_parent(p.second, root);
    return new_node;
  }

  node *remove(node *cur) noexcept {
    if (cur == nullptr)
      return nullptr;
    root = node::splay(cur);
    node *left_part = root->left;
    root = root->right;
    node::set_parent(left_part, nullptr);
    node::set_parent(root, nullptr);
    cur->left = cur->right = cur->parent = nullptr;
    if (root == nullptr) {
      root = left_part;
    } else {
      root = node::splay(find_first());
      root->left = left_part;
      node::set_parent(left_part, root);
    }
    return root;
  }

  template <typename U> void invalidate_tree() noexcept {
    root = node::splay(root);
    recur_delete<U>(root);
    root = nullptr;
  }

private:
  node *root = nullptr;
  Comparator comparator;

  template <typename U> void recur_delete(node *cur_node) noexcept {
    if (cur_node == nullptr)
      return;
    recur_delete<U>(cur_node->left);
    recur_delete<U>(cur_node->right);
    delete static_cast<U *>(cur_node);
  }

  template <typename Left, typename Right, typename CompareLeft,
            typename CompareRight>
  friend struct bimap;
};