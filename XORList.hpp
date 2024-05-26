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

        Node *head;
        Node *tail;
        size_t size;

        Node *XOR(Node *a, Node *b)
        {
            return reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b));
        }

    public:
        XORList() noexcept(canThrow == CanThrow::NoThrow)
            : head(nullptr), tail(nullptr), size(0) {}

        ~XORList()
        {
            clear();
        }

        void clear() noexcept
        {
            Node *current = head;
            Node *prev = nullptr;
            Node *next;

            while (current != nullptr)
            {
                next = XOR(prev, current->npx);
                delete current;
                prev = current;
                current = next;
            }

            head = tail = nullptr;
            size = 0;
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

            newNode->npx = head;

            if (head != nullptr)
            {
                head->npx = XOR(newNode, head->npx);
            }
            else
            {
                tail = newNode;
            }

            head = newNode;
            ++size;
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

            newNode->npx = tail;

            if (tail != nullptr)
            {
                tail->npx = XOR(newNode, tail->npx);
            }
            else
            {
                head = newNode;
            }

            tail = newNode;
            ++size;
        }

        bool empty() const noexcept
        {
            return size == 0;
        }

        size_t get_size() const noexcept
        {
            return size;
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
                    return tail->data; // undefined behavior
                }
            }
            return head->data;
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
                    return tail->data; // undefined behavior
                }
            }
            return tail->data;
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

            Node *temp = head;
            Node *next = XOR(nullptr, head->npx);

            if (next != nullptr)
            {
                next->npx = XOR(temp, next->npx);
            }
            else
            {
                tail = nullptr;
            }

            head = next;
            delete temp;
            --size;
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

            Node *temp = tail;
            Node *prev = XOR(nullptr, tail->npx);

            if (prev != nullptr)
            {
                prev->npx = XOR(temp, prev->npx);
            }
            else
            {
                head = nullptr;
            }

            tail = prev;
            delete temp;
            --size;
        }

        void insert(size_t position, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > size)
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
            if (position == size)
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
            Node *current = head;
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

            ++size;
        }

        void erase(size_t position) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position >= size)
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
            if (position == size - 1)
            {
                pop_back();
                return;
            }

            Node *prev = nullptr;
            Node *current = head;
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
            --size;
        }

        void splice(size_t position, XORList<T> &other_list) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > size)
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
                Node *other_tail = other_list.tail;
                other_tail->npx = XOR(other_tail->npx, head);

                if (head != nullptr)
                {
                    head->npx = XOR(other_tail, XOR(nullptr, head->npx));
                }
                else
                {
                    tail = other_tail;
                }

                head = other_list.head;
                size += other_list.size;
                other_list.head = other_list.tail = nullptr;
                other_list.size = 0;
                return;
            }

            Node *prev = nullptr;
            Node *current = head;
            Node *next;

            for (size_t i = 0; i < position; ++i)
            {
                next = XOR(prev, current->npx);
                prev = current;
                current = next;
            }

            Node *other_tail = other_list.tail;
            other_tail->npx = XOR(other_tail->npx, current);
            prev->npx = XOR(XOR(prev->npx, current), other_list.head);
            other_list.head->npx = XOR(prev, other_list.head->npx);

            size += other_list.size;
            other_list.head = other_list.tail = nullptr;
            other_list.size = 0;
        }

        void merge(XORList<T> &other_list) noexcept
        {
            if (other_list.empty())
            {
                return;
            }

            if (empty())
            {
                head = other_list.head;
                tail = other_list.tail;
                size = other_list.size;
                other_list.head = other_list.tail = nullptr;
                other_list.size = 0;
                return;
            }

            tail->npx = XOR(XOR(tail->npx, nullptr), other_list.head);
            other_list.head->npx = XOR(tail, other_list.head->npx);

            tail = other_list.tail;
            size += other_list.size;

            other_list.head = other_list.tail = nullptr;
            other_list.size = 0;
        }

        void reverse() noexcept
        {
            Node *temp = head;
            head = tail;
            tail = temp;
        }
    };
}

#endif // _SCC_XORLIST_HPP_
