#include <iostream>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

/**
 * Test RhIO value streaming at 
 * high frequency (500Hz)
 */
int main()
{
    if (!RhIO::started()) {
        RhIO::start(
            RhIO::PortServerRep, 
            RhIO::PortServerPub, 
            20, 
            60);
    }
    
    //Declare RhIO directories and values
    const unsigned int countDir = 40;
    const unsigned int countVal = 50;
    RhIO::Root.newInt("cycle");
    std::vector<RhIO::IONode*> nodes;
    for (size_t i=0;i<countDir;i++) {
        RhIO::Root.newChild(
            "Dir_" + std::to_string(i));
        nodes.push_back(
            &(RhIO::Root.child("Dir_" + std::to_string(i))));
        for (size_t j=0;j<countVal;j++) {
            nodes.back()->newFloat("val_" + std::to_string(j))
                ->persisted(false)->defaultValue(0.0);
        }
    }
   
    //Command for exporting logs
    RhIO::Root.newCommand("/write_logs",
        "Write RhIO logs to file",
        [](const std::vector<std::string>& args) -> std::string
        {
            if (args.size()!=1) {
                return "Usage: write_logs arg";
            } else {
                RhIO::writeLogs(args[0]);
                return "Data written to: " + args[0];
            }
        });

    //Main loop
    size_t cycle = 0;
    auto timeStart = std::chrono::steady_clock::now();
    double avgDT1 = 0.0;
    double avgDT2 = 0.0;
    while (true) {
        auto time1 = std::chrono::steady_clock::now();
        auto time2 = std::chrono::steady_clock::now();
        //Value update
        for (size_t i=0;i<countDir;i++) {
            for (size_t j=0;j<countVal;j++) {
                nodes[i]->setRTFloat(
                    "val_" + std::to_string(j), i*j+cycle);
            }
        }
        RhIO::Root.setRTInt("cycle", cycle); 
        cycle++;
        //Scheduling up to 500Hz
        auto duration1 = std::chrono::steady_clock::now() - time1;
        double dt1 = std::chrono::duration_cast<std::chrono::duration<double>>(duration1).count();
        std::this_thread::sleep_for(std::chrono::duration<double>(0.002-dt1));
        //Display timing info
        auto duration2 = std::chrono::steady_clock::now() - time2;
        double dt2 = std::chrono::duration_cast<std::chrono::duration<double>>(duration2).count();
        avgDT1 += dt1;
        avgDT2 += dt2;
        if (cycle % 100 == 0) {
            auto duration3 = std::chrono::steady_clock::now() - timeStart;
            double dt3 = std::chrono::duration_cast<std::chrono::duration<double>>(duration3).count();
            std::cout 
                << "cycle=" << cycle
                << " timeRhIO=" << (avgDT1*1000.0)/100.0 << "ms"
                << " timeLoop=" << (avgDT2*1000.0)/100.0 << "ms"
                << " timeAll=" << dt3 << "s"
                << std::endl;
            avgDT1 = 0.0;
            avgDT2 = 0.0;
        }
    }
}

