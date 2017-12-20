/**
 * @file Aop.cpp
 * @brief  实现方法的拦截，能植入before()和after(),不追求动态植入
 *         思路：包装一个类，里面定义before()和after()方法，->运算符重载方法
 *              在重载操作函数中实现before()和after()
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月19日 11时07分49秒
 */
#include "Aop.h"

