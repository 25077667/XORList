#ifndef _SCC_XORLIST_HPP_
#define _SCC_XORLIST_HPP_
#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>

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
