/**
 * @file LoadDatFile.h
 * @brief  
 * @author DongChenLong 
 * @version 1.0
 * @date 2017年10月26日 15时26分52秒
 */
#pragma once
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>
struct LoadDatFile
{
    static void Cout(std::string filename, std::ostream& out = std::cout)
    {
        try{
            std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
            if(file.is_open())
            {
                out << "---- ID ---------- VALUE ---------" << std::endl;
                while(file.good())
                {
                    /**获取group id和element id */
                    uint16_t gid, eid;
                    file.read((char*)(&gid), 2);
                    file.read((char*)(&eid), 2);

                    /**获取value的长度length */
                    uint32_t length;
                    file.read((char*)(&length), 4);

                    /**根据length长度获取value */
                    std::shared_ptr<char> value(new char[length+1]{'\0'});
                    file.read(value.get(), length);
                    
                    /**在中断打印gid eid 和value */
                    char buf[128] ={ '\0' };
                    snprintf(buf, 128," 0x%08x\t", (gid << 16 | eid));
                    out <<  buf << value.get() << std::endl;
                }
                file.close();
            }
            else
            {
                throw std::logic_error{"open error"};
            }
        }
        catch(std::exception& e)
        {
            std::cout << "ERROR:" << e.what() << std::endl;
        }
    }
    
    static void Cout(std::string filename, std::string outfilename)
    {
        std::ofstream out(outfilename); 
        Cout(filename, out);
    }
};

