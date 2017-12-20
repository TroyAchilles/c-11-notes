/**
 * @file Aop.h
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月19日 13时41分40秒
 */
#include <memory>
#include <iostream>
#include <string>
#include <chrono>
template < typename WrappedType, typename DerivedAspect>
class BaseAspect
{
    protected:
        WrappedType*        m_wrappedPtr;//被植入的对象

        DerivedAspect*      GetDerived()
        {
            return static_cast<DerivedAspect*>(this);
        }

        //被植入对象的删除器，用来自动触发切面中的After方法
        struct AfterWrapped
        {
            DerivedAspect* m_derived;
            AfterWrapped(DerivedAspect* derived)
                :m_derived(derived)
            {
                
            }
            
            void operator()(WrappedType* p)
            {
                m_derived->After(p);
            }
        };

    public:
        BaseAspect(WrappedType *p): m_wrappedPtr(p){ }

        void Before(WrappedType* )
        {
            std::cout << "BaseAspect::Before do nothin" << std::endl;
        }

        void After(WrappedType* )
        {
            std::cout << "BaseAspect::After do nothin" << std::endl;
        }

        std::shared_ptr<WrappedType> operator->()
        {
            GetDerived()->Before(m_wrappedPtr);
            return std::shared_ptr<WrappedType>(m_wrappedPtr, AfterWrapped(GetDerived()));
        }
};

//植入切面的工厂函数，返回包含被植入对象的切面,这步封装是为了隐藏WrappedType类型
//MakeAspect<LoggingAspect>(p.get())->g();  等价于LoggingAspect<X>(new X())->g();
template <template <typename> class Aspect, typename WrappedType = void >
Aspect<WrappedType> MakeAspect(WrappedType* p)
{
    return Aspect<WrappedType>(p);
}

template <typename WrappedType>
class LoggingAspect : public BaseAspect<WrappedType, LoggingAspect<WrappedType> >
{
    public:
        //编译时候要加cflags += -fpermissive
//        using BaseAspect = BaseAspect<WrappedType, LoggingAspect<WrappedType> >; 
//        LoggingAspect(WrappedType* p):BaseAspect(p){ }
        LoggingAspect(WrappedType* p):BaseAspect<WrappedType, LoggingAspect<WrappedType> >(p){ }

        void Before(WrappedType* )
        {
            std::cout << "entering..." << std::endl;
        }

        void After(WrappedType* )
        {
            std::cout << "leaving..." << std::endl;
        }
};

template <typename WrappedType>
class TimeElapsedAspect : public BaseAspect<WrappedType, TimeElapsedAspect<WrappedType> >
{
    std::chrono::system_clock::time_point   m_tbegin;
    public:
        TimeElapsedAspect(WrappedType* p): BaseAspect<WrappedType, TimeElapsedAspect<WrappedType> >(p){ }

        void Before(WrappedType* )
        {
            m_tbegin = std::chrono::system_clock::now();
            std::time_t tt = std::chrono::system_clock::to_time_t(m_tbegin);
            std::cout << "Enter at: " << ctime(&tt);
        }

        void After(WrappedType* )
        {
            std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
            std::time_t tt = std::chrono::system_clock::to_time_t(tp);
            std::cout << "Leave at: " << ctime(&tt);
            std::cout << "TimeElapsedAspect: "  << std::chrono::duration_cast< std::chrono::duration<double, std::ratio<1,1000> > >(tp - m_tbegin).count() << "ms\n"; //ok, 0.295323ms
//            std::cout << std::chrono::duration_cast< std::chrono::duration<int, std::ratio<1,1> > >(tp - m_tbegin).count() << "Seconds\n"; //ok, 0Seconds
//            std::cout << std::chrono::duration_cast< std::chrono::microseconds>(tp - m_tbegin).count() << "us"; //ok , 295us,
           
//            /**计算1970年到现在有多少年了 */
//            std::chrono::duration<int, std::ratio<3600*24*30*12, 1> > year;//(tp.time_since_epoch()); 
//            year = std::chrono::duration_cast<decltype(year)>(tp.time_since_epoch());
//            std::cout << "\nOffset 1970 sec: " << year.count();
        }
};

class IX
{
    public:
        IX(){ }
        virtual ~IX(){ std::cout << "                     destory... X\n";}
        virtual void g() = 0;
        virtual void f(std::string str) = 0;
        virtual void t() = 0;
};

class X : public IX
{
    public:
        void g()
        {
            std::cout << "it is test X" << std::endl;
        }
        
        void f(std::string str)
        {
            std::cout << str  << std::endl;
        }

        void t()
        {
            for(int i = 0; i < 100000; ++i);
        }
};

struct TestAop
{
    static void Cout()
    {
        std::shared_ptr<IX> p(new X());
        MakeAspect<LoggingAspect>(p.get())->g();
        MakeAspect<LoggingAspect>(p.get())->f("DCL SMART");
        MakeAspect<TimeElapsedAspect>(p.get())->t();
//        IX* p = new X(); //内存泄露
//        MakeAspect<LoggingAspect>(p)->g();
//        MakeAspect<LoggingAspect>(p)->f("DCL SMART");
        
//        LoggingAspect<X>(new X())->g();           //内存泄露,用来理解MakeAspect函数
   


//        /**对std::chrono的练习 */
//        std::chrono::minutes mm(1);
//        std::chrono::milliseconds ms(mm);
//        std::cout << ms.count() << std::endl;
//        std::cout << std::chrono::milliseconds::period::num << std::endl;   //分子
//        std::cout << std::chrono::milliseconds::period::den << std::endl;   //分母
    }
};






