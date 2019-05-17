/**
 * @file main.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2019-04-25
 */
#include <iostream>
using namespace std;

size_t factorial(size_t n)
{
    return (n == 0) ? 1 : n * factorial(n-1);
}

/*如果仅仅时传递literal，我们希望让他在编译期计算出结果,那么代码改称这样或许是个不错的结果*/
template <size_t N>
struct t_factorial_
{
enum : size_t {value = N * t_factorial_<N - 1>::value };
};

template <>
struct t_factorial_<0>
{
enum : size_t {value = 1};
};

/** constexpr **/
template <size_t N>
constexpr auto t_factorial = t_factorial_<N>::value;

constexpr size_t c_factorial(size_t n)
{
    return (n == 0) ? 1 : n * factorial(n-1);
}

template <typename T, size_t N>
size_t c_count(T&& v, const T(& arry)[N])
{
    size_t c = 0;
    for (const auto& t : arry)
        if (v == t) ++c;
    return c;
}

/**
 * @brief: constexpr if: the feature allows you to discard branches of an if statement at compile-time based on a constant expression condition;
 *      if constexpr (cond)
 *          statement1; //Discarded if cond is false
 *      else
 *          statement2; //Discarded if cond is true
 * For example:
 */
template <typename T>
auto get_value(T t)
{
    if constexpr (std::is_pointer_v<T>)
    {
        cout << "----true------" << endl;
        return *t;
    }
    else
    {
        cout << "----false------" << endl;
        return t;
    }
}

struct S
{
    int n;
    std::string s;
    float d;
};
template <size_t I>
auto getN(S& s)
{
    if constexpr (I == 0)
        return s.n;
    if constexpr (I == 1)
        return s.s;
    if constexpr (I == 2)
        return s.d;
}

/**
 * @brief: structured binding:
 *      tuple<T1, T2, T3> f() { return {a,b,c};}
 *      auto [x, y, z] = f();
 * For example:
 */
#include <vector>
class Config {
public:
    std::string name;
    std::size_t id;
    //int data;
    std::vector<std::string> data;
    template<size_t N>
    decltype(auto) get() {
    if      constexpr  (N == 0) return std::string_view{ name };
    else if constexpr  (N == 1) return id;
    else if constexpr  (N == 2) return data;
    }
private:
    int  _d;
};
#include <tuple>
/*Step 1: specilize std::tupe_size **/
namespace std{
    template<>
        struct tuple_size<Config> : std::integral_constant<std::size_t, 3>{ };
}

/**Step 2: specilize std::get **/
namespace std{
    template<size_t N>
        decltype(auto) get(Config t)
        {
            if      constexpr  (N == 0) return std::string_view{ t.name };
            else if constexpr  (N == 1) return t.id;
            else if constexpr  (N == 2) return t.data;
        }
}

//template <typename T>
//T f();
/**Step 3: specilize std::tuple_element**/
namespace std{
    template<std::size_t N>
        struct tuple_element<N, Config>{
            using type = decltype(std::get<N>(std::declval<Config>()));
        };
}


int main(int argc, char** argv) {
    cout << factorial(3) << endl;
    cout << t_factorial_<3>::value << endl;
    cout << t_factorial<3> << endl;
    cout << c_factorial(3) << endl;
    cout << c_factorial(argc) << endl;

    cout << c_count(',', "1,2,3,4,0") << endl;
    int a[] = {
        1,2,3,3,4,3,5,6
    };
    cout << c_count(',', "1,2,3,4,0") << endl;
    cout << c_count(3, a) << endl;

    cout << "--------------example 1 for constexpr if-------------" << endl;
    int val = 100;
    cout << get_value(val) << endl;
    cout << get_value(&val) << endl;

    cout << "--------------example 2 for constexpr if-------------" << endl;
    S s = { 101, "string", 2.22 };
    //cout << getN<argc>(s) << endl; //error, can not deduction argc, argc  is not a constant expression.
  
    Config t;
    t.name =  "c++17";
    t.id = 2017;
    t.data = std::vector<std::string>({ "hello", "the", "world" });
    std::cout << "my get-> " <<  std::get<1>(t) << endl;;

    auto get_config = [t](){
        return t;
    };

    auto [name, id, data] = get_config();
    cout << "name:" << name << endl;
    for (auto& v : data)
        cout << v << endl; 
    return 0;
}
