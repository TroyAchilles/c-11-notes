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
}
/***********************************************************/


}//end namespace util
