/**
 * @file integral_variable.cpp
 * @brief
 * @author dcl
 * @version 1.0.0.0
 * @date 2019-05-14
 */

#include <iostream>
template<size_t N>
struct ident{
    friend constexpr size_t add1_lookup(ident<N>);
    static constexpr size_t value = N;
};

template<class Ident>
struct writer {
    friend constexpr size_t add1_lookup(Ident)
    {
        return Ident::value;
    }
};

template<size_t N, int = add1_lookup(ident<N>{ })>
static constexpr size_t value_reader(int, ident<N>){
    return N;
}

template<size_t N>
static constexpr size_t value_reader(float, ident<N>, size_t R = value_reader(0, ident<N-1>())) {
    return R;
}

static constexpr size_t value_reader(float, ident<0>) {
    return 0;
}

template<size_t Max = 64>
static constexpr size_t value(size_t R = value_reader(0, ident<Max>())) { 
    return R;
}

template<size_t N = 1, class H = meta_counter>
static constexpr size_t next(size_t R = writer<ident<N + H::value) {
    return R;
}

template<size_t N, class H = meta_counter>
static constexpr size_t write (size_t R = writer<ident<N + H::value() - H::value()>>::value){
    return R;
}

int main(int argc, char** argv)
{
    //auto a = make_integral_varialble();
    return 0;
}
