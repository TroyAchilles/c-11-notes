#pragma once
#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>

struct Any
{
    Any(void) : m_tpIndex(std::type_index(typeid(void))) {}
    Any(const Any& that) : m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex) {}
    Any(Any && that) : m_ptr(std::move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {} //move:这里需要显示的移动语义，因为unique_ptr并无copy语义。

    //创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
    template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> 
    Any(U && value) : m_ptr(new Derived < typename std::decay<U>::type>(std::forward<U>(value)))
                      ,m_tpIndex(std::type_index(typeid(typename std::decay<U>::type)))
    {

    }

    bool IsNull() const { return !bool(m_ptr); }

    template<class U> 
    bool Is() const
    {
        return m_tpIndex == std::type_index(typeid(U));
    }

    //将Any转换为实际的类型
    template<class U>
    U& AnyCast()
    {
        if (!Is<U>())
        {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << std::endl;
            throw std::logic_error{"bad cast"};
        }

        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
    }

    Any& operator=(const Any& a) //should apply move_version assigment constructor, just like copy constructor
    {
        if (m_ptr == a.m_ptr)
            return *this;

        m_ptr = a.Clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }

    private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

    struct Base
    {
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
    };

    template<typename T>
        struct Derived : Base
    {
        template<typename U>
            Derived(U && value) : m_value(std::forward<U>(value)) { } //如果没有forward的话，则此处具名value变成左值，若此时T有移动构造，则不会被调用到。

        BasePtr Clone() const
        {
            return BasePtr(new Derived<T>(m_value));
        }

        T m_value;
    };

    BasePtr Clone() const
    {
        if (m_ptr != nullptr)
            return m_ptr->Clone();

        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_tpIndex;
};

void test()
{
    std::cout << "test" << std::endl;
};
void* test1()
{
    std::cout << "test1" << std::endl;
    return NULL;
}
#include <string>
struct TestAny
{
    static void Cout()
    {
        int tmp = 5;

        //放一个int
        Any x(tmp);
        std::cout << "1.  " << x.AnyCast<int>() << std::endl;

        //放一个string
        Any str(std::string("dcl"));
        std::cout << "2.  " << str.AnyCast<std::string>() << std::endl;

        //放一个函数指针
        Any fp1(&test);
        typedef void (*VP)();
        fp1.AnyCast<VP>()();
//        fp1.AnyCast<decltype(test)>()(); //编译不过
//        fp1.AnyCast<void*()>()();        //编译不过
        

        //放一个function
        std::function<void()> function = &test;
        Any fp(function);
        (fp.AnyCast<decltype(function)>())();
        
        Any tt;
        tt = 100;
        std::cout << tt.AnyCast<int>() << std::endl;

        tt = std::string("string");
        std::cout << tt.AnyCast<std::string>() << std::endl;
    }
};
