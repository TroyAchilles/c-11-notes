/**
 * @file Message.h
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年11月08日 15时10分09秒
 */
#pragma once
#include <unordered_map>
#include "NoCopyable.h"
#include <typeinfo>
#include "Any.h"

template <typename... Args>
struct Impl;

template <typename First, typename... Args>
struct Impl<First, Args...>
{
    static std::string name()
    {
        return std::string(typeid(First).name()) + Impl<Args...>::name();
    }
};

template<>
struct Impl<>
{
    static std::string name()
    {
        return "";
    }
};
template <typename... Args>
std::string type_name()
{
    return Impl<Args...>::name();
}
#define ARGS_GET
class MessageBus : NonCopyable
{
    public:
        /**向某个主题注册主题，需要订阅主题（topic、消息类型）和消息处理函数 */
        template<typename... TArgs, typename TObject, typename TMember>
        void Attach(std::string strTopic, TObject* Object, TMember Member)
        {
            std::function<void(TArgs...)> f = std::function<void(TArgs...)>([=](TArgs... args){(Object->*Member)(args...);});
#ifdef ARGS_GET
            m_map.insert(make_pair(GetKey<TArgs...>(strTopic), f));
#else
            m_map.insert(make_pair(GetKey(strTopic), f));
#endif
        }

        /**向某个主题发送消息，需要主题和消息类型。总线受到消息后会找到并通知对应的消息处理函数*/
        template<typename... Args>
        void SendReq(std::string strTopic, Args... args)
        {
#ifdef ARGS_GET
            auto range = m_map.equal_range(GetKey<Args...>(strTopic)); 
//            auto range = m_map.equal_range(GetKey(strTopic)); 
#else
            auto range = m_map.equal_range(GetKey(strTopic)); 
#endif
            for(auto it = range.first; it != range.second; ++it)
            {
#ifdef ARGS_GET
                //差异在这里，必须要加上template关键字
                std::function<void(Args...)> f = it->second.template AnyCast<decltype(f)>();
//                std::function<void(Args...)> f = it->second.AnyCast<decltype(f)>();
#else
                std::function<void(Args...)> f = it->second.AnyCast<decltype(f)>();
#endif
                f(args...);
            }
        }
       
        /**移除某个主题 */
        template<typename... Args>
        void Remove(std::string strTopic)
        {
#ifdef ARGS_GET
            auto it = m_map.find(GetKey<Args...>(strTopic));
#else
            auto it = m_map.find(GetKey(strTopic));
#endif
            while(it != m_map.end())
            {
#ifdef ARGS_GET
                std::cout << GetKey<Args...>(strTopic) << std::endl;
                m_map.erase(it);
                it = m_map.find(GetKey<Args...>(strTopic));
#else
                std::cout << GetKey(strTopic) << std::endl;
                m_map.erase(it);
                it = m_map.find(GetKey(strTopic));
#endif
            }
        }
    
    private:
        /**获得某消息的键值，通过某个主题和消息类型可以确定观察者 */
        template<typename... TArgs>
        std::string GetKey(std::string& strTopic)
        {
            return strTopic + type_name<TArgs...>();
        }
        
    private:
        std::unordered_multimap<std::string, Any> m_map;
};

typedef struct {
    void test1(int i, std::string str)
    {
        std::cout << __FUNCTION__ << ":  (" << i << "," << str << ")" << std::endl;
    }

    void test2(int i, std::string str)
    {
        std::cout << __FUNCTION__ << ":  (" << i << "," << str << ")" << std::endl;
    }

    void test3(int i)
    {
        std::cout << __FUNCTION__ << std::endl;
    }
}MyStruct;

struct TestMessage{
    static void Cout()
    {
        MessageBus bus;
        MyStruct st;
        bus.Attach<int, std::string>("JT1", &st, &MyStruct::test1);
        bus.Attach<int, std::string>("JT2", &st, &MyStruct::test2);
        bus.Attach<int, std::string>("JT2", &st, &MyStruct::test2);
        bus.Attach<int, std::string>("JT3", &st, &MyStruct::test2);
        bus.Attach<int>("JT2", &st, &MyStruct::test3);

//        bus.SendReq<int, std::string>("JT", 1, "haha");
    
//        bus.Remove<int, std::string>("JT2");
        
        bus.SendReq<int, std::string>("JT1", 1, "haha1");
        bus.SendReq<int, std::string>("JT2", 1, "haha2");
        bus.SendReq<int>("JT2", 1);
        bus.SendReq<int, std::string>("JT3", 1, "haha3");
    }
};
