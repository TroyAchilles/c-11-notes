/**
 * @file CRTP.h
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2020-05-06
 */

#include <iostream>

template <class T>
struct Lazy
{
    void func() {
         std::cout << __FUNCTION__ << std::endl;
    }

    void func2(); //not define
};

#include <memory>

struct Good : std::enable_shared_from_this<Good>
{
    std::shared_ptr<Good> getptr()
    {
        return shared_from_this();
    }
};
