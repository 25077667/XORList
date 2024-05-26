#include <XORList.hpp>
#include <gtest/gtest.h>

namespace scc
{

    TEST(XORListTest, PushFront)
    {
        XORList<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);

        EXPECT_EQ(list.get_size(), 3);
        EXPECT_EQ(list.front(), 3);
        EXPECT_EQ(list.back(), 1);
    }

    TEST(XORListTest, PushBack)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        EXPECT_EQ(list.get_size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST(XORListTest, PopFront)
    {
        XORList<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        list.pop_front();

        EXPECT_EQ(list.get_size(), 2);
        EXPECT_EQ(list.front(), 2);
    }

    TEST(XORListTest, PopBack)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.pop_back();

        EXPECT_EQ(list.get_size(), 2);
        EXPECT_EQ(list.back(), 2);
    }

    TEST(XORListTest, Insert)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(3);
        list.insert(1, 2);

        EXPECT_EQ(list.get_size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);

        list.pop_front();
        EXPECT_EQ(list.front(), 2);
    }

    TEST(XORListTest, Erase)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.erase(1);

        EXPECT_EQ(list.get_size(), 2);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST(XORListTest, Reverse)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.reverse();

        EXPECT_EQ(list.get_size(), 3);
        EXPECT_EQ(list.front(), 3);
        EXPECT_EQ(list.back(), 1);
    }

    // Robustness Tests

    TEST(XORListTest, HandlesEmptyListPopFront)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.pop_front());
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.pop_front(), std::runtime_error);
        }
    }

    TEST(XORListTest, HandlesEmptyListPopBack)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.pop_back());
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.pop_back(), std::runtime_error);
        }
    }

    TEST(XORListTest, HandlesOutOfBoundsInsert)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.insert(1, 1));
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.insert(-1, 1), std::out_of_range);
        }
    }

    TEST(XORListTest, HandlesOutOfBoundsErase)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.erase(1));
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.erase(-1), std::out_of_range);
        }
    }

    // Performance Tests

    TEST(XORListTest, LargeNumberOfInsertions)
    {
        XORList<int> list;
        const int num_elements = 1000000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.get_size(), num_elements);
        std::cout << "Time taken for " << num_elements << " insertions: " << elapsed.count() << " seconds\n";
    }

    TEST(XORListTest, LargeNumberOfDeletions)
    {
        XORList<int> list;
        const int num_elements = 1000000;

        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.pop_back();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.get_size(), 0);
        std::cout << "Time taken for " << num_elements << " deletions: " << elapsed.count() << " seconds\n";
    }
} // namespace scc
