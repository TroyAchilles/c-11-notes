/**
 * @file Ioc.h
 * @brief  Ioc 即控制反转，思想是由IOC容器来管理对象的生命周期、依赖关系等，从而
 * 使应用程序的配置和依赖性规范与实际的应用程序分开，其中一个特点就是通过文本的配置
 * 文件进行应用程序组件间相互关系的配置，而不用重新修改并编译具体的代码。
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月20日 17时34分13秒
 */
#ifndef _IOC_H_
#define _IOC_H_
#include <string>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include "Any.h"
#include "NoCopyable.h"
//#define LAMBA_EX
class IocContainer : NonCopyable
{
    public:
        IocContainer(void){ }
        ~IocContainer(void){ }
        template <class T>
        void RegisterType(std::string strKey)
        {
            typedef T* I;
            std::function<I()> function = Construct<I, T>::invoke;
//            std::function<T*()> function = Construct<T*, T>::invoke; //T* () 返回值为T*参数为空的函数指针
            RegisterType(strKey, function);
        }
        template <class I,  class T, typename... TS> //没想明白，为什么要加一个接口类型。
        void RegisterType(std::string strKey)
        {
//            std::function<I*(TS...)> function = Construct<I*, T, TS...>::invoke;//我用lamba实现，但是还是要存入fucntion，故次注释的实现比较好。
//            RegisterType(strKey, function);
            
            auto lb = [](TS... Args){
                return static_cast<I*>(new T(Args...));
            };
            auto lb1 = [](TS... Args){
                return static_cast<I*>(NULL);
            };
#ifdef LAMBA_EX //此实现有问题。
//            /*lb1和lb的类型都不同，因此不可以直接存储lb进去,会导致AnyCast转换失败，必须要用function封装一次.   
//            std::cout << "Fuck:" << typeid(decltype(lb)).name() << std::endl;
//            std::cout << "Fuck:" << typeid(decltype(lb1)).name() << std::endl;
            RegisterType(strKey, lb); //这里存成lb，则reslove里面也要改动，不可以用funciton啦
#else
            std::function<I*(TS...)> function = lb;
            RegisterType(strKey, function);
#endif
        }

        template <class I>
        I* Resolve(std::string strKey)
        {
            if(m_creatorMap.find(strKey) == m_creatorMap.end())
                return nullptr;

            Any resolver = m_creatorMap[strKey];
            std::function<I*()> function = resolver.AnyCast<std::function<I*()>>();
            return function();
        }
        template <class I, typename... TS>
        I* Resolve(std::string strKey, TS... Args)
        {
            if(m_creatorMap.find(strKey) == m_creatorMap.end())
                return nullptr;

            Any resolver = m_creatorMap[strKey]; //取得该类型的invoke，invoke执行会new出该类型。
            typedef I* (*FT)(TS...);
#ifdef LAMBA_EX
            std::cout << "-========= 3 ===========\n";
            auto lb = [](TS... Args1){
                return static_cast<I*>(NULL); //这个lb和上面的lb，类型不同，导致AnyCast会转换错误
            };
            std::cout << "Fuck:" << typeid(decltype(lb)).name() << std::endl;
            auto function = resolver.AnyCast<decltype(lb)>();
#else
            std::function<I*(TS...)> function = resolver.AnyCast<std::function<I*(TS...)>>();
#endif
            return function(Args...); //执行invoke，new出对象，（没有人释放，有内存泄露的危险)
        }

        template <class I, typename... TS>
        std::shared_ptr<I> ResolveShared(std::string strKey, TS... Args)
        {
            auto p = Resolve<I, TS...>(strKey, Args...);
            return std::shared_ptr<I>(p);   //安全，无内存泄露。
        }

        template <class I>
        std::shared_ptr<I> ResolveShared(std::string strKey)
        {
            auto p = Resolve<I>(strKey);
            return std::shared_ptr<I>(p);   //安全，无内存泄露。
        }
    private:
        /** 
         * @brief I 接口类型， T 类型， TS参数类型
         */
        template<typename I, typename T, typename... TS>
        struct Construct
        {
            static I invoke(TS... Args) 
            {
//                return I(new T(Args...)); //这里是类型强换转：I必须是一个指针，否则就成了I对象的构造了。感觉我下面的写法更合理,但I还必须是指针
                return static_cast<I>(new T(Args...));
            }
        };

        void RegisterType(std::string strKey, Any constructor)
        {
            if(m_creatorMap.find(strKey) != m_creatorMap.end())
                throw std::logic_error("this key has alread exist!");
            m_creatorMap.insert(make_pair(strKey, constructor));
        }
    private:
        std::unordered_map<std::string, Any>    m_creatorMap;
};



/************************* 测试代码 *******************************/
struct ICar
{
    virtual ~ICar(){ 
        std::cout << "ICar::Destory\n"; 
    };
    virtual void move() const = 0;
};


struct Bus : ICar
{
    Bus(int i, float f){ };
    ~Bus()
    {
        std::cout << "Bus::Destory\n"; 
    }
    void move() const {
        std::cout << "Bus::move()\n"; 
    }
};

class Ix
{
    public:
        Ix() = default;
        virtual ~Ix() = default;
        virtual void move() = 0;
};

class X1 : public Ix
{
    public:
        ~X1(){
            std::cout << "X1::~X1()\n"; 
        }
        void move()final {
            std::cout << "X1::move()\n"; 
        }
};

struct TestIoc
{
    static void Cout()
    {
        /**简单注册，需要类型信息和Key */
        IocContainer ioc;
//        ioc.RegisterType<X1>("ss");
//        /*ioc.RegisterType<X1>("ss")*/; //key重复会报错
//        auto x1 = ioc.Resolve<X1>("ss");
//        x1->move();

        /**简单注册，注册需要接口类型，类型信息，key */
//        ioc.RegisterType<Ix, X1>("ff");
//        auto ix1 = ioc.Resolve<Ix>("ff");
//        ix1->move();

        /**含参数注册，需要类型信息，接口类型，参数类型和key */
        ioc.RegisterType<ICar, Bus, int, float>("bus");
        int i = 1;
        float f = 2.0;
        auto mycar = ioc.Resolve<ICar>("bus", i, f);
        mycar->move();


        /**shared_ptr 无内存泄露，会执行析狗函数 */
        ioc.RegisterType<Bus, Bus, int, float>("bus2");
        auto mycar2 = ioc.ResolveShared<Bus>("bus2", i, f);
        mycar2->move();

        ioc.RegisterType<X1>("bb");
        auto xb = ioc.ResolveShared<X1>("bb");
        xb->move();
    }
};













#endif
