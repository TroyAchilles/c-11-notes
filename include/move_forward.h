/**
 * @file move_forward.h
 * @brief move和完美转发 
 * 引用类型才分左右值引用，通过左右值应用来分清原数据是左值还是右值，如果参数不是引用类型，那么内部将无法分清原值是左右.
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月24日 17时35分18秒
 */
#include <iostream>

template<typename T>
void printT(T& t)
{
    std::cout << "l_value" << std::endl;
}


template<typename T>
void printT(T&& t)
{
    std::cout << "r_value" << std::endl;
}


template<typename T>
void testForward(T&& v)
{
    printT(v);
//    printT(std::forward<T>(v));
    printT(static_cast<T&&>(v));
    printT(std::move(v));
}

using namespace std;
template<typename T>
T&& MyForward(typename std::remove_reference<T>::type& t)
{
    cout << "forward:: left..." << endl;
    return static_cast<T&&>(t);
}

template<typename T>
T&& MyForward(typename std::remove_reference<T>::type&& t)
{
    cout << "forward:: right..." << endl;
    return static_cast<T&&>(t);
}

template<typename T>
T&& MyForward1(T t) //无引用，不分左右值引用,因此无法分清,我日说不清了。
{
    cout << "---forward:: right..." << endl;
    return static_cast<T&&>(t); //始终是T类型的右值引用
}
struct TestMoveForward
{
    static void Cout()
    {
        testForward(10); //l,r,r
        int x = 10;
        testForward(x); //l,l,r
        testForward(std::forward<int>(x));//l,r,r //forward实现可参考MyForward
    }
};

//万能的函数包装器
template <typename Function, typename... Args>
inline auto FuncWrapper(Function&& f, Args&& ...args)->decltype(f(std::forward<Args>(args)...))
{
    return f(std::forward<Args>(args)...);
}


