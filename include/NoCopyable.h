/**
 * @file NoCopyable.h
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年11月08日 15时13分01秒
 */
#pragma once

struct NonCopyable{
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};


