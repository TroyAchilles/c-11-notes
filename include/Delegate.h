/**
 * @file Delegate.h
 * @brief  implement a simple delegate by pure c++11  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年05月04日 16时24分03秒
 */
#pragma once 

#include <functional>
class TestDelegate
{
    public: 
        void            Cout();
        int             MFunction(int , int); 
};

template<typename RetType, typename ...Args>
class Delegate
{
    public:
        template<typename Object>
            void bind(Object* obj, RetType(Object::*mf)(Args...))
            {
                m_function = [obj, mf](Args ...args){ return (obj->*mf)(args...); };
            }

        template<typename T>
            void bind(T& func) { m_function = func; }

        void operator ()(Args ...args) { m_function(args...); }

    private:
        std::function<RetType(Args...)>      m_function;
};
