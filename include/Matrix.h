/**
 * @file Matrix.h
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2020-02-27
 */

/*
 * 例5：给一个由数字组成的矩阵，初始在左上角，要求每次只能向下或向右移动，路径和就是经过的数字全部加起来，求可能的最小路径和。
 * 1  3  5  9
 * 8  1  3  4 
 * 5  0  6  1 
 * 8  8  4  0
 *
 */
#include <cassert>
#include <initializer_list>
#include <iostream>

struct Element
{
    int row;
    int col;
    int data;
};

struct Node
{
    Node()
    {
        lNode = nullptr;
        rNode = nullptr;
    }
    Element d;
    struct Node* lNode;
    struct Node* rNode;
};

template <class T, size_t ROW, size_t COL>
class Array2D
{
    public:
        Array2D(std::initializer_list<std::initializer_list<T>> l)
        {
            assert(l.size() <= ROW);
            auto it_row = l.begin();
            for ( int i = 0; i < ROW; ++i)
            {
                if (it_row != l.end())
                {
                    assert(it_row->size() <= COL);
                    auto it = it_row->begin();
                    for (int j = 0; j < COL; ++j)
                    {
                        if (it != it_row->end())
                        {
                            _data[i][j] = *it;
                            it++;
                        }
                        else
                            _data[i][j] = 0;
                    }
                    ++it_row;
                }
                else
                {
                    for (int j = 0; j < COL; ++j)
                        _data[i][j] = 0;
                }
            }
        }

        void Print()
        {
            for (int i = 0; i < ROW; ++i)
            {
                for (int j = 0; j < COL; ++j)
                    std::cout << _data[i][j] << " ";
                std::cout << std::endl;
            }
        }
    private:
        template <class TYPE, size_t SIZE> 
        class array1d
        {
            private: 
                TYPE _d[SIZE];

            public:
                T& operator[](size_t index)
                {
                    assert(index < SIZE);
                    return _d[index];
                }
                
                const T& operator[](size_t index) const
                {
                    assert(index < SIZE);
                    return _d[index];
                }
        };

    public:
        array1d<T, COL>& operator[](size_t index)
        {
            assert(index < ROW);
            return _data[index];
        }

    private:
        array1d<T, COL> _data[ROW];

};

void Cout();
//int iMatrix[4][4];
