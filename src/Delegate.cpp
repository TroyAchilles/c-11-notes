/**
 * @file Delegate.cpp
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年05月04日 16时39分14秒
 */

#include <iostream>
#include "Delegate.h"
int fun_1(int x, int y);


void TestDelegate::Cout()
{
    /**test normal funciton */
    Delegate<int,int,int> normal_function; 
    normal_function.bind(fun_1);
    normal_function(2,6);
    
    /**test member funciton */
    Delegate<int,int,int> mf; 
    mf.bind(this, &TestDelegate::MFunction);
    mf(2,6);
    
    /**test lamba expression*/
    auto lb = [](int x, int y){std::cout << "lamba expression: x+y = " << x+y << std::endl; return x; };
    Delegate<int,int,int> lf; 
    lf.bind(lb);
    lb(2,6);
    
}

int TestDelegate::MFunction(int x, int y)
{
    std::cout << "member function: x+y = " << x+y << std::endl;
    return x;
}

int fun_1(int x, int y)
{
    std::cout << "normal function: x+y = " << x+y << std::endl;
    return x+y;
}
