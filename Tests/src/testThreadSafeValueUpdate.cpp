#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

void function1()
{
    RhIO::IONode& node = RhIO::Root.child("/test");
    for (size_t i=0;i<10000;i++) {
        node.addRTInt("int", 1);
    }
}

void function2()
{
    RhIO::IONode& node = RhIO::Root.child("/test");
    for (size_t i=0;i<10000;i++) {
        node.addRTInt("int", -1);
    }
}

int main()
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    assert(RhIO::started());

    RhIO::Root.newChild("test");
    RhIO::Root.newInt("test/int");
    RhIO::Root.setInt("test/int", 0);

    std::thread t1(function1);
    std::thread t2(function2);
    
    t1.join();
    t2.join();
    
    std::cout << RhIO::Root.getInt("/test/int") << std::endl;

    return 0;
}

