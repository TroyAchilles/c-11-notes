/**
 * @file right_reference.h
 * @brief  介绍并学习c++11引入的新概念：右值，右值引用，std::forward, std::move等
 *      1. 左值和右值是独立他们的类型的，一个左值的有可能是右值引用类型，如example1.
 *      2. T&& 是一个未定义的引用类型，可能是左值引用也可能是右值引用，取决于初始化的值类型,example2
 *      3. &&成为未定义的引用类型的唯一条件是：T&&且发生类型推断,且任何一点附加条件都会变成一个右值引用,example3
 *      4. 所有的右值应用叠加到右值引用上变成一个右值引用，其他引用折叠都为左值引用。
 *      5. 右值引用用来支持转移语义，转移语义可以将资源（堆，系统对象）从一个对象转移到另一个对象，减少不必要的临时
 *         对象创建、拷贝、销毁，大幅度提高c++应用程序性能，消除临时对象的维护（创建和销毁）对性能的影响。example4
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月24日 09时16分04秒
 */
#include <iostream>
#include <utility>
#include <string.h>
#include <vector>
template<typename T>
void example(T&& t)
{
    if(std::is_lvalue_reference<decltype(t)>::value)
        std::cout << "left reference\n";
    else if(std::is_rvalue_reference<decltype(t)>::value)
        std::cout << "right reference\n";
    else if(std::is_integral<decltype(t)>::value)
        std::cout << "integral type\n";
    else
        std::cout << "unknow type\n";

    /******************************推导规则如下***************************************/
    /**调用前形参类型，调用时传入的实参类型，调用后形参实际类型， 调用后T的实际类型  */
    /**    T&&              A&&                 A&&                     A            */
    /**    T&&              A&                  A&                      A&           */
}

template<typename T>
void exampleMore(const T&& t) //3. 任何附加条件导致此处&&为右值引用，而不是universal ref。
{
    if(std::is_lvalue_reference<decltype(t)>::value)
        std::cout << "left reference\n";
    else if(std::is_rvalue_reference<decltype(t)>::value)
        std::cout << "right reference\n";
    else if(std::is_integral<decltype(t)>::value)
        std::cout << "integral type\n";
    else
        std::cout << "unknow type\n";
}

class MyString{
    private:
        char* m_data;
        size_t m_len;
        void copy_data(const char* s)
        {
            m_data = new char[m_len+1];
            memcpy(m_data, s, m_len);
            m_data[m_len] = '\0';
        }
    public:
        MyString()
        {
            m_data = NULL;
            m_len = 0;
            std::cout << "Construct PARAM0 is called!" << std::endl;
        }

        MyString(const char* p)
        {
            m_len = strlen(p);
            copy_data(p);
            std::cout << "Construct PARAM1 is called! source: " << m_data << std::endl;
        }

        MyString(const MyString& str)
        {
            m_len = str.m_len;
            copy_data(str.m_data);
            std::cout << "copy Construct is called! source: " << str.m_data << std::endl;
        }

        MyString& operator=(const MyString& str)
        {
            if(this != &str)
            {
                m_len = str.m_len;
                copy_data(str.m_data);
            }
            std::cout << "copy Assignment is called! source: " << str.m_data << std::endl;
            return *this;
        }

        virtual ~MyString()
        {
            if(m_data)
            {
                std::cout << "destruct is called! source: " << m_data << std::endl;
                free(m_data);
            }
        }
        char* GetStr()
        {
            return m_data;
        }
};

class MyStringMove{
    private:
        char* m_data;
        size_t m_len;
        void copy_data(const char* s)
        {
            m_data = new char[m_len+1];
            memcpy(m_data, s, m_len);
            m_data[m_len] = '\0';
        }
    public:
        MyStringMove()
        {
            m_data = NULL;
            m_len = 0;
            std::cout << "Construct PARAM0 is called!" << std::endl;
        }

        MyStringMove(const char* p)
        {
            m_len = strlen(p);
            copy_data(p);
            std::cout << "Construct PARAM1 is called! source: " << m_data << std::endl;
        }

        MyStringMove(const MyStringMove& str)
        {
            m_len = str.m_len;
            copy_data(str.m_data);
            std::cout << "copy Construct is called! source: " << str.m_data << std::endl;
        }
        MyStringMove(MyStringMove&& str)
        {
            std::cout << "Move Construct is called! source: " << str.m_data << std::endl;
            m_len = str.m_len;
            m_data = str.m_data;
            str.m_len = 0;
            str.m_data = NULL;
        }

        MyStringMove& operator=(const MyStringMove& str)
        {
            if(this != &str)
            {
                m_len = str.m_len;
                copy_data(str.m_data);
            }
            std::cout << "copy Assignment is called! source: " << str.m_data << std::endl;
            return *this;
        }

        MyStringMove& operator=(MyStringMove&& str)
        {
            std::cout << "Move Assigment is called! source: " << str.m_data << std::endl;
            if(this != &str)
            {
                m_len = str.m_len;
                m_data = str.m_data; //避免了不必要的拷贝
                str.m_len = 0;
                str.m_data = NULL;
            }
            return *this;
        }

        virtual ~MyStringMove()
        {
            if(m_data)
            {
                std::cout << "destruct is called! source: " << m_data << std::endl;
                free(m_data);
            }else
                std::cout << "destruct is called!" << std::endl;
        }
};
MyString getStr(const char* p)
{
    return MyString(p);
}

MyStringMove getStrMove(const char* p)
{
    return MyStringMove(p);
}
//#define EXAMPLE1
//#define EXAMPLE2
//#define EXAMPLE3
//#define EXAMPLE4
#define EXAMPLE5
struct TestRightRef
{
    static void Cout()
    {
#ifdef EXAMPLE1
        /****************example1. *******************/
        std::cout << "************************** example1: *************************************\n";
        int&& var1 = 10; //var1的类型为右值引用类型，var1是左值(也可理解为，具名的右值引用为左值)
        auto&& var2 = var1;  //此处有类型推断，因此&&为universal renference， 其被var1左值初始化，则var2为左值引用int&
        
        
        std::cout << std::is_rvalue_reference<decltype(var1)>::value << std::endl;
        var1 = 11;
        std::cout << "var1=" << var1 << " var2=" << var2 << std::endl;  // 11, 11
        var2 = 12;
        std::cout << "var1=" << var1 << " var2=" << var2 << std::endl;  // 12. 12

        std::cout << "&var1=" << &var1 << std::endl;
        std::cout << "&var2=" << &var2 << std::endl;  //&var1和&var2，相等.
        std::cout << "**************************************************************************\n";
#endif


#ifdef EXAMPLE2
        /****************example2. *******************/
        std::cout << "************************** example2: *************************************\n";
        example(10);  //用右值初始化，则t为右值引用int&&, T = int
        
        int tmp = 11;
        example(tmp); //用左值初始化，则t为左值引用int&, T = int&

        int&& tmp1 = 12;
        std::cout << std::is_rvalue_reference<decltype(tmp1)>::value << std::endl; //true
        example(tmp1);  //tmp1是一个左值， 则t为左值引用int&, T = int&。
        std::cout << "**************************************************************************\n";
#endif

#ifdef EXAMPLE3
        /****************example3. *******************/
        std::cout << "************************** example3: *************************************\n";
        exampleMore(10);  //用右值初始化，则t为右值引用int&&, T = int
        
//        int tmpmore = 11;
//        exampleMore(tmpmore); //用左值初始化，因为const缘故，此处不是universal reference，而是右值引用，因此无法将左值绑定到右值引用上,编译报错。

//        int&& tmp1more = 12;
//        std::cout << std::is_rvalue_reference<decltype(tmp1more)>::value << std::endl; //true
//        exampleMore(tmp1more);  //tmp1是一个左值，因此无法将左值绑定到右值引用上,编译报错。
        std::cout << "**************************************************************************\n";
#endif

#ifdef EXAMPLE4
        //使用&&，减少资源的申请和释放
        MyString a;   //执行构造次数1
        a = MyString("hello"); //没有move语义时，先构造临时对象(执行构造次数2)，在将临时对象赋值构造给a(执行赋值构造=，3), 临时对象被析构，因此多余浪费资源的申请和释放等
        std::cout << "**************************************************************************\n";
        std::vector<MyString> vec;
        vec.push_back(MyString("world")); //先构造临时world对象，再执行拷贝构造, 然后临时对象被析狗,因此多余浪费资源的申请和浪费等
        std::cout << "**************************************************************************\n";

        //由于没有移动语义，造成没有意义的析构和构造过程在造成无意义资源申请和释放等。如果能够直接使用临时对象已经申请的资源
        //即能节省资源，又能节省资源申请和释放的时间。这正是转移语义的目的. 
        //(构造和析构的次数并没减少，只是执行了不同(移动、普通)的构造或者赋值构造)
        std::cout << "----------------------------------------------\n";
        MyStringMove b;
        b = MyStringMove("helloB");
        std::cout << "----------------------------------------------\n";
        std::vector<MyStringMove> vecb;
        vecb.push_back(MyStringMove("worldB")); //先构造临时world对象，再执行拷贝构造, 然后临时对象被析狗,因此多余浪费资源的申请和浪费等
        std::cout << "----------------------------------------------\n";
#endif

#ifdef EXAMPLE5
        //使用&&，构造和析构的次减少, 和对象内部有没有实现对应的转移语义没关系。
        std::cout << "----------------------------------------------\n";
//        getStr("MMM");  //总共二次构造和析构： MyString(p)一次， return到Cout栈上的返回地址第二次
//        MyString str = getStr("MMMM");  //总共三次构造和析构： 第三次 Cout栈上返回地址到str拷贝构造，第三次
//        MyString&& str1 = getStr("MMMM");  //总共二次构造和析构：同getStr("MMM"); 我觉得有问题，时系统自动和成了&&拷贝构造吧？下面验证下
//        MyString&& str2 = MyString("MMMM");  //总共一次构造和析构：
        std::cout << "----------------------------------------------\n";
        getStrMove("MMM");  //总共二次构造和析构： MyString(p)一次， return到Cout栈上的返回地址第二次,此处时Move版本的CopyConstruct
//        MyStringMove str = getStrMove("MMMM");  //总共三次构造和析构： 第三次 Cout栈上返回地址到str拷贝构造，第三次
//        MyStringMove&& str1 = getStrMove("MMMM");  //总共二次构造和析构：同getStr("MMM"); 我觉得有问题，时系统自动和成了&&拷贝构造吧？此处验证:确实只有两次
//        MyStringMove&& str2 = MyStringMove("MMMM");  //总共一次构造和析构：
        std::cout << "----------------------------------------------\n";
#endif
    }
};





