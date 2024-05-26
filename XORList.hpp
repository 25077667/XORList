#ifndef _SCC_XORLIST_HPP_
#define _SCC_XORLIST_HPP_
#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <limits>
#include <algorithm>

namespace scc
{
    enum class CanThrow
    {
        NoThrow,
        Throw,
    };

    template <typename T, CanThrow canThrow = CanThrow::NoThrow>
    class XORList
    {
    private:
        struct Node
        {
            T data;
            Node *npx; // XOR of next and previous node pointers

            Node(T value) : data(value), npx(nullptr) {}
        };

        Node *m_head_;
        Node *m_tail_;
        size_t m_size_;

        Node *XOR(Node *a, Node *b)
        {
            return reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b));
        }

    public:
        XORList() noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0) {}

        ~XORList()
        {
            clear();
        }

        class iterator
        {
        private:
            Node *prev_;
            Node *current_;
            XORList *list_;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            iterator(Node *prev, Node *current, XORList *list)
                : prev_(prev), current_(current), list_(list) {}

            reference operator*() const { return current_->data; }
            pointer operator->() const { return &(current_->data); }

            iterator &operator++()
            {
                if (current_ == nullptr && prev_ == list_->m_tail_)
                    return *this; // If already at the end, do nothing

                Node *next = list_->XOR(prev_, current_->npx);
                if (current_ == nullptr)
                { // If we were at the beginning
                    current_ = list_->m_head_;
                    prev_ = list_->XOR(nullptr, current_->npx);
                }
                else
                {
                    prev_ = current_;
                    current_ = next;
                }
                return *this;
            }

            iterator operator++(int)
            {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            iterator &operator--()
            {
                if (prev_ == nullptr && current_ == list_->m_head_)
                    return *this; // If already at the beginning, do nothing

                Node *next = list_->XOR(prev_, current_->npx);
                if (current_ == nullptr)
                { // If we were at the end
                    current_ = list_->m_tail_;
                    prev_ = list_->XOR(current_->npx, nullptr);
                }
                else
                {
                    Node *newPrev = list_->XOR(next->npx, current_);
                    current_ = prev_;
                    prev_ = newPrev;
                }
                return *this;
            }

            iterator operator--(int)
            {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            iterator operator+(difference_type n) const
            {
                iterator temp = *this;
                for (difference_type i = 0; i < n; ++i)
                {
                    ++temp;
                }
                return temp;
            }

            iterator operator-(difference_type n) const
            {
                iterator temp = *this;
                for (difference_type i = 0; i < n; ++i)
                {
                    --temp;
                }
                return temp;
            }

            difference_type operator-(const iterator &other) const
            {
                difference_type count = 0;
                if (*this < other)
                {
                    for (iterator it = *this; it != other; ++it)
                    {
                        ++count;
                    }
                }
                else
                {
                    for (iterator it = other; it != *this; ++it)
                    {
                        ++count;
                    }
                }
                return count;
            }

            bool operator<(const iterator &other) const
            {
                iterator it = *this;
                while (it != other && it != list_->end())
                {
                    ++it;
                }
                if (it == other)
                {
                    return true;
                }
                it = *this;
                while (it != other && it != list_->begin())
                {
                    --it;
                }
                return it == other;
            }

            bool operator==(const iterator &other) const { return current_ == other.current_; }
            bool operator!=(const iterator &other) const { return current_ != other.current_; }
        };

        XORList &operator=(const XORList &other) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (this != &other)
            {
                clear();
                for (Node *current = other.m_head_, *prev = nullptr, *next; current != nullptr; prev = current, current = next)
                {
                    next = XOR(prev, current->npx);
                    push_back(current->data);
                }
            }
            return *this;
        }

        void assign(size_t count, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            clear();
            for (size_t i = 0; i < count; ++i)
            {
                push_back(value);
            }
        }

        std::allocator<T> get_allocator() const noexcept
        {
            return std::allocator<T>();
        }

        iterator begin() noexcept
        {
            return iterator(nullptr, m_head_, this);
        }

        iterator cbegin() const noexcept
        {
            return iterator(nullptr, m_head_, this);
        }

        iterator end() noexcept
        {
            return iterator(m_tail_, nullptr, this);
        }

        iterator cend() const noexcept
        {
            return iterator(m_tail_, nullptr, this);
        }

        iterator rbegin() noexcept
        {
            return iterator(nullptr, m_tail_, this);
        }

        iterator crbegin() const noexcept
        {
            return iterator(nullptr, m_tail_, this);
        }

        iterator rend() noexcept
        {
            return iterator(m_head_, nullptr, this);
        }

        iterator crend() const noexcept
        {
            return iterator(m_head_, nullptr, this);
        }

        size_t max_size() const noexcept
        {
            return std::numeric_limits<size_t>::max();
        }

        template <typename... Args>
        void emplace(size_t position, Args &&...args) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > m_size_)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::out_of_range("Position out of range");
                }
                else
                {
                    return; // No operation on out of range position
                }
            }

            if (position == 0)
            {
                emplace_front(std::forward<Args>(args)...);
                return;
            }
            if (position == m_size_)
            {
                emplace_back(std::forward<Args>(args)...);
                return;
            }

            Node *newNode = new (std::nothrow) Node(T(std::forward<Args>(args)...));
            if (!newNode)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::bad_alloc();
                }
                else
                {
                    return; // No operation on allocation failure
                }
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next;

            for (size_t i = 0; i < position; ++i)
            {
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }

            newNode->npx = XOR(prev, current);
            prev->npx = XOR(XOR(prev->npx, current), newNode);
            current->npx = XOR(newNode, XOR(prev, current->npx));

            ++m_size_;
        }

        template <typename... Args>
        void emplace_back(Args &&...args) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = new (std::nothrow) Node(T(std::forward<Args>(args)...));
            if (!newNode)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::bad_alloc();
                }
                else
                {
                    return; // No operation on allocation failure
                }
            }

            newNode->npx = m_tail_;

            if (m_tail_ != nullptr)
            {
                m_tail_->npx = XOR(newNode, m_tail_->npx);
            }
            else
            {
                m_head_ = newNode;
            }

            m_tail_ = newNode;
            ++m_size_;
        }

        void resize(size_t count, const T &value = T()) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (count < m_size_)
            {
                while (m_size_ > count)
                {
                    pop_back();
                }
            }
            else if (count > m_size_)
            {
                while (m_size_ < count)
                {
                    push_back(value);
                }
            }
        }

        void unique() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (m_size_ < 2)
            {
                return;
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next = XOR(prev, current->npx);

            while (next != nullptr)
            {
                if (current->data == next->data)
                {
                    Node *next_next = XOR(current, next->npx);
                    current->npx = XOR(prev, next_next);

                    if (next_next != nullptr)
                    {
                        next_next->npx = XOR(current, XOR(next, next_next->npx));
                    }
                    else
                    {
                        m_tail_ = current;
                    }

                    delete next;
                    next = next_next;
                    --m_size_;
                }
                else
                {
                    prev = current;
                    current = next;
                    next = XOR(prev, current->npx);
                }
            }
        }

        void clear() noexcept
        {
            Node *current = m_head_;
            Node *prev = nullptr;
            Node *next;

            while (current != nullptr)
            {
                next = XOR(prev, current->npx);
                delete current;
                prev = current;
                current = next;
            }

            m_head_ = m_tail_ = nullptr;
            m_size_ = 0;
        }

        void push_front(const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = new (std::nothrow) Node(value);
            if (!newNode)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::bad_alloc();
                }
                else
                {
                    return; // No operation on allocation failure
                }
            }

            newNode->npx = m_head_;

            if (m_head_ != nullptr)
            {
                m_head_->npx = XOR(newNode, m_head_->npx);
            }
            else
            {
                m_tail_ = newNode;
            }

            m_head_ = newNode;
            ++m_size_;
        }

        void push_back(const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = new (std::nothrow) Node(value);
            if (!newNode)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::bad_alloc();
                }
                else
                {
                    return; // No operation on allocation failure
                }
            }

            newNode->npx = m_tail_;

            if (m_tail_ != nullptr)
            {
                m_tail_->npx = XOR(newNode, m_tail_->npx);
            }
            else
            {
                m_head_ = newNode;
            }

            m_tail_ = newNode;
            ++m_size_;
        }

        bool empty() const noexcept
        {
            return m_size_ == 0;
        }

        size_t size() const noexcept
        {
            return m_size_;
        }

        T &front() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_head_->data;
        }

        T &back() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_tail_->data;
        }

        void pop_front() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return; // No operation on empty list
                }
            }

            Node *temp = m_head_;
            Node *next = XOR(nullptr, m_head_->npx);

            if (next != nullptr)
            {
                next->npx = XOR(temp, next->npx);
            }
            else
            {
                m_tail_ = nullptr;
            }

            m_head_ = next;
            delete temp;
            --m_size_;
        }

        void pop_back() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return; // No operation on empty list
                }
            }

            Node *temp = m_tail_;
            Node *prev = XOR(nullptr, m_tail_->npx);

            if (prev != nullptr)
            {
                prev->npx = XOR(temp, prev->npx);
            }
            else
            {
                m_head_ = nullptr;
            }

            m_tail_ = prev;
            delete temp;
            --m_size_;
        }

        void insert(size_t position, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > m_size_)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::out_of_range("Position out of range");
                }
                else
                {
                    return; // No operation on out of range position
                }
            }

            if (position == 0)
            {
                push_front(value);
                return;
            }
            if (position == m_size_)
            {
                push_back(value);
                return;
            }

            Node *newNode = new (std::nothrow) Node(value);
            if (!newNode)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::bad_alloc();
                }
                else
                {
                    return; // No operation on allocation failure
                }
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next;

            for (size_t i = 0; i < position; ++i)
            {
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }

            newNode->npx = XOR(prev, current);
            prev->npx = XOR(XOR(prev->npx, current), newNode);
            current->npx = XOR(newNode, XOR(prev, current->npx));

            ++m_size_;
        }

        void erase(size_t position) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position >= m_size_)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::out_of_range("Position out of range");
                }
                else
                {
                    return; // No operation on out of range position
                }
            }

            if (position == 0)
            {
                pop_front();
                return;
            }
            if (position == m_size_ - 1)
            {
                pop_back();
                return;
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next;

            for (size_t i = 0; i < position; ++i)
            {
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }

            Node *next_next = XOR(prev, current->npx);
            prev->npx = XOR(XOR(prev->npx, current), next_next);
            next_next->npx = XOR(prev, XOR(current, next_next->npx));

            delete current;
            --m_size_;
        }

        void splice(size_t position, XORList<T> &other_list) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > m_size_)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::out_of_range("Position out of range");
                }
                else
                {
                    return; // No operation on out of range position
                }
            }

            if (other_list.empty())
            {
                return;
            }

            if (position == 0)
            {
                Node *other_tail = other_list.m_tail_;
                other_tail->npx = XOR(other_tail->npx, m_head_);

                if (m_head_ != nullptr)
                {
                    m_head_->npx = XOR(other_tail, XOR(nullptr, m_head_->npx));
                }
                else
                {
                    m_tail_ = other_tail;
                }

                m_head_ = other_list.m_head_;
                m_size_ += other_list.m_size_;
                other_list.m_head_ = other_list.m_tail_ = nullptr;
                other_list.m_size_ = 0;
                return;
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next;

            for (size_t i = 0; i < position; ++i)
            {
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }

            Node *other_tail = other_list.m_tail_;
            other_tail->npx = XOR(other_tail->npx, current);
            prev->npx = XOR(XOR(prev->npx, current), other_list.m_head_);
            other_list.m_head_->npx = XOR(prev, other_list.m_head_->npx);

            m_size_ += other_list.m_size_;
            other_list.m_head_ = other_list.m_tail_ = nullptr;
            other_list.m_size_ = 0;
        }

        void merge(XORList<T> &other_list) noexcept
        {
            if (other_list.empty())
            {
                return;
            }

            if (empty())
            {
                m_head_ = other_list.m_head_;
                m_tail_ = other_list.m_tail_;
                m_size_ = other_list.m_size_;
                other_list.m_head_ = other_list.m_tail_ = nullptr;
                other_list.m_size_ = 0;
                return;
            }

            m_tail_->npx = XOR(XOR(m_tail_->npx, nullptr), other_list.m_head_);
            other_list.m_head_->npx = XOR(m_tail_, other_list.m_head_->npx);

            m_tail_ = other_list.m_tail_;
            m_size_ += other_list.m_size_;

            other_list.m_head_ = other_list.m_tail_ = nullptr;
            other_list.m_size_ = 0;
        }

        void reverse() noexcept
        {
            Node *temp = m_head_;
            m_head_ = m_tail_;
            m_tail_ = temp;
        }
    };
}

#endif // _SCC_XORLIST_HPP_
