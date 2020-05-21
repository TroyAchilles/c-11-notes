/**
 * @file Matrix.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2020-02-27
 */
#include "Matrix.h"
#include <stdio.h>
#include <iostream>

//constexpr int MAX_ROW = 4;
//constexpr int MAX_COL = 4;
//int iMatrix[MAX_ROW][MAX_COL] = {
    //1, 3, 5, 9,
    //8, 1, 3, 4,
    //5, 0, 6, 1,
    //8, 8, 4, 0,
//};

//constexpr int MAX_ROW = 3;
//constexpr int MAX_COL = 3;
//int iMatrix[MAX_ROW][MAX_COL] = {
    //1, 3, 5,
    //8, 1, 3,
    //5, 0, 6,
//};

constexpr int MAX_ROW = 2;
constexpr int MAX_COL = 2;
int iMatrix[MAX_ROW][MAX_COL] = {
    1, 3,
    8, 1,
};

//constexpr int MAX_ROW = 1;
//constexpr int MAX_COL = 1;
//int iMatrix[MAX_ROW][MAX_COL ] = {
    //1
//};


struct Node* g_root = NULL; 

static void BTree(int* address, int rows, int cols, Node** node)
{
    if (rows < MAX_ROW && cols < MAX_COL)
    {
        Node* p = new Node();
        p->d.row = rows;
        p->d.col = cols;
        p->d.data = *(address +  p->d.row * MAX_COL+ p->d.col);
        *node = p;

        if (rows + 1 < MAX_ROW)
        { 
            BTree(address, rows + 1, cols, &(p->lNode));
        }

        if (cols + 1 < MAX_COL)
        {
            BTree(address, rows, cols + 1, &p->rNode);
        }
    }
}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static int gotShort(int row, int col)
{
    static Array2D<int , 4, 4> aMatrix = {
        { 1, 3, 5, 9 },
        { 8, 1, 3, 4 },
        { 5, 0, 6, 1 },
        { 8, 8, 4, 0 },
    };

    if (row == 0 && col == 0)
       return aMatrix[row][col];

    if (row == 0 && col > 0)
        return gotShort(row, col -1) + aMatrix[row][col];
    
    if (col == 0 && row > 0)
        return gotShort(row - 1, col) + aMatrix[row][col];

    return MIN(gotShort(row - 1, col), gotShort(row, col - 1)) + aMatrix[row][col];
}

void PrintTreee(Node* node)
{
    if (node != NULL)
    {
        //printf("([%d,%d] = %d)", node->d.row, node->d.col, node->d.data);
        printf("(%d) ", node->d.data);
        PrintTreee(node->lNode);
        PrintTreee(node->rNode);
    }
}

int Run(Node* tree)
{
    if (tree == NULL)
        return 0;
    
    if (tree->rNode == NULL && tree->lNode == NULL)
    {
        return tree->d.data;
    }

    if (tree->rNode == NULL)
    {
        auto t = Run(tree->lNode) + tree->d.data;
        return t;
    }

    if (tree->lNode == NULL)
    {
        auto t = Run(tree->rNode) + tree->d.data;
        return t;
    }

    //int l = Run(tree->lNode);
    //int r = Run(tree->rNode);
    //return MIN(l, r) + tree->d.data;
    return MIN(Run(tree->lNode), Run(tree->rNode)) + tree->d.data;
}

void Cout()
{
    //for (int i = 0; i < 4; ++i)
    //{
    //for (int j = 0; j < 4; ++j)
    //printf("%d ", iMatrix[i][j]);
    //printf("\n");
    //}

    BTree((int*)iMatrix, 0, 0, &g_root);
    PrintTreee(g_root);
    std::cout << std::endl;
    std::cout << Run(g_root);
    std::cout << std::endl;
    std::cout << "--lalalal------" << gotShort(3, 3);
    std::cout << std::endl;
}


#define MIN(x, y) (((x) < (y)) ? (x) : (y))  //两数取其小

int aMatrix[4][4] = {
    { 1, 3, 5, 9 },
    { 8, 1, 3, 4 },
    { 5, 0, 6, 1 },
    { 8, 8, 4, 0 },
};

//求矩阵起点到row和col坐标的最短距离
static int ShortestPath(int row, int col)
{
    if (row == 0 && col == 0)
       return aMatrix[row][col];

    if (row == 0 && col > 0)
        return gotShort(row, col -1) + aMatrix[row][col];
    
    if (col == 0 && row > 0)
        return gotShort(row - 1, col) + aMatrix[row][col];

    return MIN(gotShort(row - 1, col), gotShort(row, col - 1)) + aMatrix[row][col];
}



