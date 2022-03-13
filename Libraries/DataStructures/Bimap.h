#pragma once

#include "SplayTree.h"

struct left_tag;
struct right_tag;

template<typename Left, typename Right>
struct node_bimap : Node<Left, left_tag>, Node<Right, right_tag>
{
    node_bimap(const Left &left, const Right &right)
        : Node<Left, left_tag>(std::forward<Left>(left))
        , Node<Right, right_tag>(std::forward<Right>(right))
    {
    };

    node_bimap(const Left &left, Right &&right)
        : Node<Left, left_tag>(std::forward<Left>(left))
        , Node<Right, right_tag>(std::forward<Right>(right))
    {
    };

    node_bimap(Left &&left, const Right &right)
        : Node<Left, left_tag>(std::forward<Left>(left))
        , Node<Right, right_tag>(std::forward<Right>(right))
    {
    };
    
    node_bimap(Left &&left, Right &&right)
        : Node<Left, left_tag>(std::forward<Left>(left))
        , Node<Right, right_tag>(std::forward<Right>(right))
    {
    };

    node_bimap(const node_bimap &other) = default;

    ~node_bimap() = default;
};

template<typename Left, typename Right, typename CompareLeft = std::less<Left>, typename CompareRight = std::less<Right>>
struct bimap
{
private:
    using left_t = Left;
    using right_t = Right;

    using left_node_t = Node<Left, left_tag>;
    using right_node_t = Node<Right, right_tag>;

    using left_tree_t = Tree<Left, left_tag, CompareLeft>;
    using right_tree_t = Tree<Right, right_tag, CompareRight>;

    using node_t = node_bimap<Left, Right>;

public:
    struct right_iterator;

    struct left_iterator
    {
        explicit left_iterator(const bimap *itr_bimap) noexcept
            : itr_bimap(itr_bimap){};

        left_iterator(const bimap *itr_bimap, node_t *node) noexcept
            : itr_bimap(itr_bimap)
            , itr_node(node)
        {
        };

        left_iterator(const bimap *itr_bimap, left_node_t *node) noexcept
            : itr_bimap(itr_bimap)
            , itr_node(static_cast<node_t *>(node))
        {
        };

        left_iterator(const left_iterator &other) noexcept = default;

        left_t const &operator*() const { return get_node_ptr()->value; }

        left_iterator &operator++() noexcept
        {
            set_itr_node(get_itr_tree().get_next(get_node_ptr()));
            return *this;
        }

        left_iterator operator++(int) noexcept
        {
            left_iterator prev(*this);
            ++(*this);
            return prev;
        }

        left_iterator &operator--() noexcept
        {
            if (itr_node == nullptr) {
                set_itr_node(get_itr_tree().find_last());
            } else {
                set_itr_node(get_itr_tree().get_prev(get_node_ptr()));
            }
            return *this;
        }

        left_iterator operator--(int) noexcept
        {
            left_iterator prev(*this);
            --(*this);
            return prev;
        }

        right_iterator flip() const noexcept
        {
            return right_iterator(itr_bimap, itr_node);
        }

        friend bool operator==(left_iterator const &a, left_iterator const &b)
        {
            return (a.itr_bimap == b.itr_bimap && a.itr_node == b.itr_node);
        }

        friend bool operator!=(left_iterator const &a, left_iterator const &b)
        {
            return !(a == b);
        }

    private:
        const bimap *itr_bimap;
        node_t *itr_node = nullptr;

        left_tree_t &get_itr_tree() const noexcept { return itr_bimap->left_tree; }

        left_node_t *get_node_ptr() const noexcept
        {
            return static_cast<left_node_t *>(itr_node);
        }

        void set_itr_node(left_node_t *new_node) noexcept
        {
            itr_node = static_cast<node_t *>(new_node);
        }

        template<typename T1, typename T2, typename T3>
        friend T1
            bimap<Left, Right, CompareLeft, CompareRight>::common_erase(T1 &, T2 &, T3 &) noexcept;
    };

    struct right_iterator
    {
        explicit right_iterator(const bimap *itr_bimap) noexcept
            : itr_bimap(itr_bimap){};

        right_iterator(const bimap *itr_bimap, node_t *node) noexcept
            : itr_bimap(itr_bimap)
            , itr_node(node){};

        right_iterator(const bimap *itr_bimap, right_node_t *node) noexcept
            : itr_bimap(itr_bimap)
            , itr_node(static_cast<node_t *>(node)){};

        right_iterator(const right_iterator &other) noexcept = default;

        right_t const &operator*() const { return get_node_ptr()->value; }

        right_iterator &operator++() noexcept
        {
            set_itr_node(get_itr_tree().get_next(get_node_ptr()));
            return *this;
        }

        right_iterator operator++(int) noexcept
        {
            right_iterator prev(*this);
            ++(*this);
            return prev;
        }

        right_iterator &operator--() noexcept
        {
            if (itr_node == nullptr) {
                set_itr_node(get_itr_tree().find_last());
            } else {
                set_itr_node(get_itr_tree().get_prev(get_node_ptr()));
            }
            return *this;
        }

        right_iterator operator--(int) noexcept
        {
            right_iterator prev(*this);
            --(*this);
            return prev;
        }

        left_iterator flip() const noexcept
        {
            return left_iterator(itr_bimap, itr_node);
        }

        friend bool operator==(right_iterator const &a, right_iterator const &b)
        {
            return (a.itr_bimap == b.itr_bimap && a.itr_node == b.itr_node);
        }

        friend bool operator!=(right_iterator const &a, right_iterator const &b)
        {
            return !(a == b);
        }

    private:
        const bimap *itr_bimap;
        node_t *itr_node = nullptr;

        right_tree_t &get_itr_tree() const { return itr_bimap->right_tree; }

        right_node_t *get_node_ptr() const
        {
            return static_cast<right_node_t *>(itr_node);
        }

        void set_itr_node(right_node_t *new_node)
        {
            itr_node = static_cast<node_t *>(new_node);
        }

        template<typename T1, typename T2, typename T3>
        friend T1
            bimap<Left, Right, CompareLeft, CompareRight>::common_erase(T1 &, T2 &, T3 &) noexcept;
    };

    bimap(CompareLeft compare_left = CompareLeft(),
          CompareRight compare_right = CompareRight())
        : left_tree(std::move(compare_left))
        , right_tree(std::move(compare_right)){};

    bimap(bimap const &other)
        : left_tree(other.left_tree)
        , right_tree(other.right_tree)
        , cur_size(other.cur_size)
    {
        for (auto itr = other.begin_left(); itr != other.end_left(); ++itr) {
            insert(*itr, *itr.flip());
        }
    }

    bimap(bimap &&other) noexcept = default;

    bimap &operator=(bimap const &other)
    {
        if (this != &other) {
            bimap(other).swap(*this);
        }
        return *this;
    };

    bimap &operator=(bimap &&other) noexcept
    {
        if (this != &other) {
            bimap(std::move(other)).swap(*this);
        }
        return *this;
    };

    ~bimap()
    {
        left_tree.template invalidate_tree<node_t>();
        right_tree.root = nullptr;
    }

    left_iterator insert(left_t const &left, right_t const &right)
    {
        return common_insert(std::forward<const left_t>(left), std::forward<const right_t>(right));
    }
    left_iterator insert(left_t const &left, right_t &&right)
    {
        return common_insert(std::forward<const left_t>(left), std::forward<right_t>(right));
    }
    left_iterator insert(left_t &&left, right_t const &right)
    {
        return common_insert(std::forward<left_t>(left), std::forward<const right_t>(right));
    }
    left_iterator insert(left_t &&left, right_t &&right) noexcept
    {
        return common_insert(std::forward<left_t>(left), std::forward<right_t>(right));
    }

    /*
   * Delete left element and it's paired element.
   * erase of an invalid iterator is undefined.
   * erase(end_left()) and erase(end_right()) is undefined.
   * Invalidates all iterators referred to element and it's pair, that 'it' pointed at
   */
    left_iterator erase_left(left_iterator it) noexcept
    {
        return common_erase(it, left_tree, right_tree);
    }

    right_iterator erase_right(right_iterator it) noexcept
    {
        return common_erase(it, right_tree, left_tree);
    }
    /*
   * Same as 'erase', but uses key.
   * If such element exists, it will be deleted.
   * If it doesn't, do nothing.
   * Returns if pair of elements was erased.
   */
    bool erase_left(left_t const &left) noexcept
    {
        return common_erase_bool(left, left_tree, right_tree);
    }

    bool erase_right(right_t const &right) noexcept
    {
        return common_erase_bool(right, right_tree, left_tree);
    }

    /*
   * Erase range of elements [first, last).
   * Returns iterator to the last element.
   */
    left_iterator erase_left(left_iterator first, left_iterator last) noexcept
    {
        return common_erase_interval(first, last, left_tree, right_tree);
    }

    right_iterator erase_right(right_iterator first, right_iterator last) noexcept
    {
        return common_erase_interval(first, last, right_tree, left_tree);
    }

    // Returns iterator to element. If such element doesn't exists, returns end()
    left_iterator find_left(left_t const &left) const noexcept
    {
        return left_iterator(this, left_tree.find_value(left));
    }
    right_iterator find_right(right_t const &right) const noexcept
    {
        return right_iterator(this, right_tree.find_value(right));
    }

    /*
   * Returns iterator to paired element.
   * If element by given key doesn't exists, throws std::out_of_range.
   */
    right_t const &at_left(left_t const &key) const
    {
        return common_at<left_t, left_tree_t, left_iterator>(key, left_tree);
    }

    left_t const &at_right(right_t const &key) const
    {
        return common_at<right_t, right_tree_t, right_iterator>(key, right_tree);
    }

    /*
   * Returns paired element by given key value.
   * If such key doesn't exists, puts it in bimap with
   * paired element with default value and returns reference
   * to this element. In case default paired element is already
   * in bimap, change it's pair to given key.
   */
    right_t const &at_left_or_default(left_t const &key)
    {
        left_node_t *ptr = left_tree.find_value(key);
        if (ptr == nullptr) {
            right_t default_right = right_t();
            if (right_tree.find_value(default_right)) {
                erase_right(default_right);
                cur_size -= 1;
            }
            cur_size += 1;
            return *(insert(key, std::move(default_right)).flip());
        }
        return flip_nodes_types<right_node_t, left_node_t>(ptr)->value;
    }

    left_t const &at_right_or_default(right_t const &key)
    {
        right_node_t *ptr = right_tree.find_value(key);
        if (ptr == nullptr) {
            left_t default_value = left_t();
            if (left_tree.find_value(default_value)) {
                erase_left(default_value);
                cur_size -= 1;
            }
            cur_size += 1;
            return *insert(std::move(default_value), key);
        }
        return flip_nodes_types<left_node_t, right_node_t>(ptr)->value;
    }

    /*
   * Return iterator for lower or upper bound element.
   * Same as std::lower_bound
   */
    left_iterator lower_bound_left(const left_t &left) const noexcept
    {
        return left_iterator(this, left_tree.find_grater_or_equal(left));
    }
    left_iterator upper_bound_left(const left_t &left) const noexcept
    {
        return left_iterator(this, left_tree.find_grater_or_equal(left, false));
    }

    right_iterator lower_bound_right(const right_t &left) const noexcept
    {
        return right_iterator(this, right_tree.find_grater_or_equal(left));
    }
    right_iterator upper_bound_right(const right_t &left) const noexcept
    {
        return right_iterator(this, right_tree.find_grater_or_equal(left, false));
    }

    left_iterator begin_left() const noexcept
    {
        return left_iterator(this, left_tree.find_first());
    }
    left_iterator end_left() const noexcept { return left_iterator(this); }

    right_iterator begin_right() const noexcept
    {
        return right_iterator(this, right_tree.find_first());
    }
    right_iterator end_right() const noexcept { return right_iterator(this); }

    bool empty() const noexcept { return (cur_size == 0); }

    std::size_t size() const noexcept { return cur_size; }

    friend bool operator==(bimap const &a, bimap const &b)
    {
        if (a.size() != b.size()) {
            return false;
        }
        for (auto al = a.begin_left(), bl = b.begin_left();
             al != a.end_left() && bl != b.end_left(); ++al, ++bl) {
            if (*al != *bl) {
                return false;
            }
        }
        for (auto al = a.begin_right(), bl = b.begin_right();
             al != a.end_right() && bl != b.end_right(); ++al, ++bl) {
            if (*al != *bl) {
                return false;
            }
        }
        return true;
    }

    friend bool operator!=(bimap const &a, bimap const &b) { return !(a == b); }

private:
    mutable left_tree_t left_tree;
    mutable right_tree_t right_tree;
    size_t cur_size = 0;

    void swap(bimap &other)
    {
        left_tree.swap(other.left_tree);
        right_tree.swap(other.right_tree);
        std::swap(cur_size, other.cur_size);
    }

    template<typename Left_t, typename Right_t>
    left_iterator common_insert(Left_t left, Right_t right) noexcept
    {
        if (left_tree.contains(left) || right_tree.contains(right)) {
            return end_left();
        }
        auto *new_node =
            new node_t(std::forward<Left_t>(left), std::forward<Right_t>(right));
        right_tree.add(new_node);
        cur_size += 1;
        return left_iterator(this, left_tree.add(new_node));
    }

    template<typename iterator_t, typename tree_t, typename other_tree_t>
    iterator_t common_erase(iterator_t &it, tree_t &cur_tree, other_tree_t &other_tree) noexcept
    {
        auto next_iterator = iterator_t(this, cur_tree.remove(it.itr_node));
        other_tree.remove(it.itr_node);
        delete it.itr_node;
        cur_size -= 1;
        return next_iterator;
    }

    template<typename val_t, typename tree_t, typename other_tree_t>
    bool common_erase_bool(const val_t &value, tree_t &tree, other_tree_t &other_tree) noexcept
    {
        if (auto left_node = tree.find_value(value)) {
            auto *main_node = static_cast<node_t *>(left_node);
            tree.remove(left_node);
            other_tree.remove(main_node);
            delete main_node;
            cur_size -= 1;
            return true;
        }
        return false;
    }

    template<typename iterator_t, typename tree_t, typename other_tree_t>
    iterator_t common_erase_interval(iterator_t &first, iterator_t &last, tree_t &cur_tree, other_tree_t &other_tree) noexcept
    {
        iterator_t cur = first;
        while (cur != last) {
            cur = common_erase(cur, cur_tree, other_tree);
        }
        return cur;
    }

    template<typename val_t, typename tree_t, typename iterator_t>
    auto const &common_at(const val_t &val, tree_t &tree) const
    {
        if (auto t = tree.find_value(val)) {
            return *iterator_t(this, t).flip();
        }
        throw std::out_of_range("No such key value in bimap");
    }

    template<typename T, typename U>
    T *flip_nodes_types(U *ptr) const
    {
        return static_cast<T *>(static_cast<node_t *>(ptr));
    }
};