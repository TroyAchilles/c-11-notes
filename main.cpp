/**
 * @file main.cpp
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年05月04日 16时15分23秒
 */

#include <iostream>
#include "Delegate.h"
#include "Aop.h"
#include "Ioc.h"
#include "right_reference.h"
#include "move_forward.h"
#include "LoadDatFile.h"
#include "Message.h"
#include "DependentNames.h"
int main(int argc, char** argv)
{
//    TestDelegate td; td.Cout();
//    TestAop::Cout();
//    TestIoc::Cout();
//    TestRightRef::Cout(); 
    TestMoveForward::Cout(); 
//    TestAny::Cout();
//    LoadDatFile::Cout("Test.DAT");
//    LoadDatFile::Cout(argv[1]);
//    TestMessage::Cout(); 
//    TestDependentNames::Cout(); 
    return 0;
}
