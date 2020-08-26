#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

int main()
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    
    //Create nodes
    RhIO::Root.newChild("/path/to/child");
    RhIO::Root.newFloat("/path/to/child/val1");
    RhIO::Root.newFloat("/path/to/child/val2");
    RhIO::Root.newFloat("/path/to/child/val3");
    RhIO::Root.newFloat("/path/to/child/val4");
    RhIO::Root.newFloat("/path/to/child/val5");
    RhIO::Root.newFloat("/path/to/child/val6");

    volatile double val;
    (void)val;
    {
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            val = RhIO::Root.getFloat("/path/to/child/val1");
            val = RhIO::Root.getFloat("/path/to/child/val2");
            val = RhIO::Root.getFloat("/path/to/child/val3");
            val = RhIO::Root.getFloat("/path/to/child/val4");
            val = RhIO::Root.getFloat("/path/to/child/val5");
            val = RhIO::Root.getFloat("/path/to/child/val6");
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time getFloat absolue name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::IONode& node = RhIO::Root.child("/path/to/child");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            val = node.getFloat("val1");
            val = node.getFloat("val2");
            val = node.getFloat("val3");
            val = node.getFloat("val4");
            val = node.getFloat("val5");
            val = node.getFloat("val6");
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time getFloat relative name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::IONode& node = RhIO::Root.child("/path/to/child");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            val = node.getRTFloat("val1");
            val = node.getRTFloat("val2");
            val = node.getRTFloat("val3");
            val = node.getRTFloat("val4");
            val = node.getRTFloat("val5");
            val = node.getRTFloat("val6");
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time getRTFloat relative name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::WrapperFloat w1(RhIO::Root.child("/path/to/child"), "val1");
        RhIO::WrapperFloat w2(RhIO::Root.child("/path/to/child"), "val2");
        RhIO::WrapperFloat w3(RhIO::Root.child("/path/to/child"), "val3");
        RhIO::WrapperFloat w4(RhIO::Root.child("/path/to/child"), "val4");
        RhIO::WrapperFloat w5(RhIO::Root.child("/path/to/child"), "val5");
        RhIO::WrapperFloat w6(RhIO::Root.child("/path/to/child"), "val6");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            val = w1.get();
            val = w2.get();
            val = w3.get();
            val = w4.get();
            val = w5.get();
            val = w6.get();
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time getRTFloat wrapper (6x10000): " << dur.count() << " ms" << std::endl;
    }

    {
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            RhIO::Root.setFloat("/path/to/child/val1", 42.0);
            RhIO::Root.setFloat("/path/to/child/val2", 42.0);
            RhIO::Root.setFloat("/path/to/child/val3", 42.0);
            RhIO::Root.setFloat("/path/to/child/val4", 42.0);
            RhIO::Root.setFloat("/path/to/child/val5", 42.0);
            RhIO::Root.setFloat("/path/to/child/val6", 42.0);
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time setFloat absolue name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::IONode& node = RhIO::Root.child("/path/to/child");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            node.setFloat("val1", 42.0);
            node.setFloat("val2", 42.0);
            node.setFloat("val3", 42.0);
            node.setFloat("val4", 42.0);
            node.setFloat("val5", 42.0);
            node.setFloat("val6", 42.0);
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time setFloat relative name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::IONode& node = RhIO::Root.child("/path/to/child");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            node.setRTFloat("val1", 42.0);
            node.setRTFloat("val2", 42.0);
            node.setRTFloat("val3", 42.0);
            node.setRTFloat("val4", 42.0);
            node.setRTFloat("val5", 42.0);
            node.setRTFloat("val6", 42.0);
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time setRTFloat relative name (6x10000): " << dur.count() << " ms" << std::endl;
    }
    {
        RhIO::WrapperFloat w1(RhIO::Root.child("/path/to/child"), "val1");
        RhIO::WrapperFloat w2(RhIO::Root.child("/path/to/child"), "val2");
        RhIO::WrapperFloat w3(RhIO::Root.child("/path/to/child"), "val3");
        RhIO::WrapperFloat w4(RhIO::Root.child("/path/to/child"), "val4");
        RhIO::WrapperFloat w5(RhIO::Root.child("/path/to/child"), "val5");
        RhIO::WrapperFloat w6(RhIO::Root.child("/path/to/child"), "val6");
        auto time1 = std::chrono::steady_clock::now();
        for (size_t i=0;i<10000;i++) {
            w1.set(42.0);
            w2.set(42.0);
            w3.set(42.0);
            w4.set(42.0);
            w5.set(42.0);
            w6.set(42.0);
        }
        auto time2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> dur = time2 - time1;
        std::cout << "Time setRTFloat wrapper (6x10000): " << dur.count() << " ms" << std::endl;
    }
}

