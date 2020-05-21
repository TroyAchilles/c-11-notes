/**
 * @file expand_tuple.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2019-05-16
 */
#include <iostream>
#include <tuple>
#include "utility.h"

#if __cplusplus == 201703L
template<typename... Args, typename Func>
void for_each(const std::tuple<Args...>& t, Func&& f)
{
    for_each(t, f, std::index_sequence_for<Args...>());
}

template<typename... Args, typename Func, std::size_t... Idx>
void for_each(const std::tuple<Args...>& t, Func&& f, std::index_sequence<Idx...>)
{
    (f(std::get<Idx>(t)), ...);
}

template<typename Array, std::size_t... Idx>
auto a2t_impl(const Array& a, std::index_sequence<Idx...>)
{
    return std::make_tuple(a[Idx]...);
}

template<typename T, std::size_t N>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, std::make_index_sequence<N>());
}
#endif

template<typename... Args>
void test(Args... args)
{
    util::sequence::show(util::sequence::index_sequence_for<Args...>());
}

struct Test_meta
{
    int id;
    std::string name;
    double score;
    META(id, name, score);
};
#define PRINT_VA_ARGS(...) std::cout << "print va_args:" << #__VA_ARGS__ << ":" << std::endl;

int main(void)
{
#if __cplusplus == 201703L
    std::array<std::size_t, 5> ay = { 0,1,2,3,4 };

    for_each(
            a2t_impl(ay, std::make_index_sequence<5>()),
            [](auto& e) { std::cout << ":" << e << std::endl; }
            );
    
    for_each(
            a2t(ay),
            [](auto& e) { std::cout << ":" << e << std::endl; }
            );
#endif
    
    util::sequence::show(util::sequence::make_index_sequence<10>());
    std::cout << std::endl;
    
    test("abcde", 1, 2.3);
    std::cout << std::endl;

    std::cout << "va_args_num: " << util::va_args_num(1, 2.3, "hello", "444", std::tuple<int, int, std::string>(1,2,"tuple")) << std::endl;
    std::cout << "va_args_num: " << VA_ARGS_NUM(1, 2.3, "hello", "444", std::tuple<int, int, std::string>(1,2,"tuple")) << std::endl;
  
    Test_meta tm = { 100, "dcl", 3.14 }; 

    tm.meta();
    for_each(
            tm.meta(),
            [](auto& e) { std::cout << e.first << ":" << e.second << std::endl; }
            );
    return 0;
}


#include <iostream>

template <typename Tuple, typename F>
void expand_tuple(Tuple&& t, F&& f)
{

}








