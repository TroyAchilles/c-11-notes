/**
 * @file utility.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2019-05-17
 */
#include <tuple>
#include <vector>

namespace util
{
    /**
     * @brief: implement some features that are only available since c++14,if you complied with 
     * -std=c++14 or higer, then plz use the STL.
     */
    struct sequence 
    {
        template<std::size_t...> struct index_sequence{ }; 

        /**
         * @brief: implementation by class template sepecialization
         */
        template<std::size_t... Is> struct g_sequence;

        template<std::size_t I, std::size_t... Is> 
            struct g_sequence<I, Is...> {using type = typename g_sequence<I-1, I-1, Is...>::type;};

        template<std::size_t... Is> 
            struct g_sequence<0, Is...> {using type = index_sequence<Is...>;};

        /**
         * @brief: implementation by class template inherit 
         */
        template<std::size_t I, std::size_t... Is>
            struct h_sequence : h_sequence<I-1, I-1, Is...>{ };
        
        template<std::size_t... N>
            struct h_sequence<0, N...>{ using type = index_sequence<N...>; };

        /**
         * @brief: A helper alias template 
         */
        template<std::size_t N> 
            using make_index_sequence = typename h_sequence<N>::type;

        template<typename... Args>
            using index_sequence_for = make_index_sequence<sizeof...(Args)>;

        /**
         * @brief  use to show the index_sequence<>
         */
        template<std::size_t... Is>
            static void show(index_sequence<Is...>)
            {
                std::initializer_list<int>{ (std::cout << Is, 0)...  }; 
                //((std::cout << Is),...); //available in c++17
            }
    };//end struct sequence


    std::vector<std::string> split(const std::string& str, char delimiter)
    {
        std::vector<std::string> output; 
        std::size_t start = 0;
        std::size_t end = 0;
        
        while (start < str.size())
        {
            end = str.find_first_of(delimiter, start);
            output.push_back(std::move(str.substr(start, end - start)));
            if (end == std::string::npos)
                break;
            start = end + 2;
        }

        return std::move(output);
    }



/********************* A simple meta **************************************/
#define VA_ARGS_NUM(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

    template<typename... Args>
        static inline std::size_t va_args_num(Args...) {return sizeof...(Args);}

template<typename V, typename Tp>
    auto my_apply(const V& v, std::size_t index, Tp const & tp)
    {
        return tp;
    }

template<typename V, typename Tp, typename A0, typename... Args>
    auto my_apply(const V& v, std::size_t index, const Tp & tp, A0& a0, Args&... args)
    {
        return my_apply(v, index + 1, std::tuple_cat(tp, std::make_tuple(std::pair<std::string, A0&>(v[index], a0))), args...);
    }

#define META(...) auto meta(){\    
    auto vec = util::split(#__VA_ARGS__, ',');\
    return util::my_apply(vec, 0, std::tuple<>(), __VA_ARGS__);\
    
/******************* a really async, and not block current process ***********/
template <typename F, typename... Args>
auto really_async(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using _Ret = typename std::result_of<F(Args...)>::type;
    auto _func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    std::packaged_task<_Ret()> tsk(std::move(_func));
    auto _fut = tsk.get_future();
    std::thread thd(std::move(tsk));
    thd.detach();
    return _fut;
}
}
/***********************************************************/

template <typename T>
struct ETS
{
        static const char* list[];
};

enum Speed
{
        S_50,
        S_25,
        S_15,
};

template <typename T>
const char* convert(T t)
{
        return ETS<T>::list[t];
}

template <>
const char* ETS<Speed>::list[] = {"50ms", "25ms", "15ms"};

/*******************************检测类中是否存在某个成员变量****************************/
// 定义一个类
// class MyClass {
// public:
//     int someValue;
// };

// 创建一个辅助模板
template <typename T>
struct has_someValue {
    template <typename C>
    static std::true_type test(decltype(&C::someValue));
    template <typename C>
    static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

// int main() {
//     // 检查 MyClass 是否包含名为 someValue 的成员变量
//     if (has_someValue<MyClass>::value) {
//         std::cout << "MyClass contains someValue member." << std::endl;
//     } else {
//         std::cout << "MyClass does not contain someValue member." << std::endl;
//     }

//     return 0;
// }


template <class Obj, class K, typename... ObjArgs>
class factory
{
    public:
        static factory*  instance()
        {
            static factory _sfactory;
            return &_sfactory;
        }

        std::shared_ptr<Obj> produce(K key, ObjArgs... args)
        {
            if (_map.find(key) == _map.end())
                return std::shared_ptr<Obj>(0);
            return std::shared_ptr<Obj>(_map[key](args...));
        }

    private:
        factory() = default;
        factory(const factory& ) = delete;
        factory(factory&& ) = delete;
    public:
        std::map<K, std::function<Obj*(ObjArgs...)>> _map;

    public:
        template <typename T>
        struct register_t
        {
            register_t(const K& key)
            {
                factory::instance()->_map.emplace(key, [](ObjArgs... args){
                    return new T(args...);
                });
            }

            typename std::enable_if<std::is_base_of<Obj,T>::value, char>::type pass;
        };
};

template <class Fn, class... Args>
 struct callable
 {
 private:
     template<typename U>
     static std::false_type check(...);

     template<typename U>
     static auto check(int)->decltype(std::declval<U>()(std::declval<Args>()...), std::true_type());

 public:
     enum { value = std::is_same<decltype(check<Fn>(0)), std::true_type>::value};
 };


// // 创建一个类
// class MyClass {
// public:
//     void myFunction(int value) {
//         // 类的成员函数带参数
//     }
// };

// 辅助模板用于检查成员函数是否存在
template <typename T, typename... Args>
struct has_myFunction {
    // 使用 SFINAE 技术检查成员函数是否存在
    template <typename C>
    static constexpr auto test(C* c) -> decltype(c->myFunction(std::declval<Args>()...), std::true_type{});
    template <typename C>
    static constexpr std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template<typename T, typename... Args>
struct has_member_foo
{
private:
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().foo(std::declval<Args>()...), std::true_type());

    template<typename U> static std::false_type Check(...);
public:
    enum{ value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };
};
#define HAS_MEMBER(member)\
template<typename T, typename... Args>struct has_member_##member\
{\
private:\
        template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
    template<typename U> static std::false_type Check(...);\
public:\
    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

// int main() {
//     // 检查 MyClass 是否包含名为 myFunction 的成员函数，带一个 int 参数
//     if (has_myFunction<MyClass, int>::value) {
//         std::cout << "MyClass contains myFunction member function with an int parameter." << std::endl;
//     } else {
//         std::cout << "MyClass does not contain myFunction member function with an int parameter." << std::endl;
//     }

//     // 检查 MyClass 是否包含 myFunction 成员函数，带其他参数
//     if (has_myFunction<MyClass, double>::value) {
//         std::cout << "MyClass contains myFunction member function with a double parameter." << std::endl;
//     } else {
//         std::cout << "MyClass does not contain myFunction member function with a double parameter." << std::endl;
//     }

//     return 0;
// }

}//end namespace util
