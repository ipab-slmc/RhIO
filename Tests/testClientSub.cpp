#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"
#include "ClientSub.hpp"

int main() 
{
    RhIO::ClientSub client(
        std::string("tcp://localhost:") + RhIO::ServerPubPort);
    
    client.setHandlerBool(
        [](const std::string name, long timestamp, bool val) 
    {
        std::cout << "Receiving Bool:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/paramBool");
        assert(val == true);
    });
    client.setHandlerInt(
        [](const std::string name, long timestamp, long val) 
    {
        std::cout << "Receiving Int:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/test3/paramInt");
        assert(val == 2);
    });
    client.setHandlerFloat(
        [](const std::string name, long timestamp, double val) 
    {
        std::cout << "Receiving Float:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/paramFloat");
        assert(val == 3.0);
    });
    client.setHandlerStr(
        [](const std::string name, long timestamp, const std::string& val) 
    {
        std::cout << "Receiving Str:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/test3/paramStr");
        assert(val == "4");
    });
    client.setHandlerStream(
        [](const std::string name, long timestamp, const std::string& val) 
    {
        std::cout << "Receiving Stream:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/stream1");
        assert(val == "test stream1\n");
    });
    
    std::cout << "Waiting" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(3000));

    return 0;
}

