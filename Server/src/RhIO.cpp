#include <thread>
#include <chrono>
#include <atomic>
#include <stdexcept>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>
#include "RhIO.hpp"
#include "rhio_server/ServerRep.hpp"
#include "rhio_server/ServerPub.hpp"
#include "rhio_server/ServerLog.hpp"

namespace RhIO {

/**
 * Main instance global allocation
 */
IONode Root("ROOT", nullptr);

/**
 * Default initialization of 
 * ServerStream and ServerLogging server
 */
ServerPub* ServerStream = nullptr;
ServerLog* ServerLogging = nullptr;

/**
 * Default initialization of 
 * time getter function
 */
std::function<int64_t()> FuncGetTime = 
    []() -> int64_t {
        return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    };

/**
 * Atomic register storing the number
 * of initialized Server
 * (for main thread waiting)
 */
static std::atomic<int> initServerCount;

/**
 * Server Pub, Log and Rep thread instance
 * and quit state
 */
static std::thread* serverThreadRep = nullptr;
static std::thread* serverThreadPub = nullptr;
static std::thread* serverThreadLog = nullptr;
static bool serverThreadRepOver = false;
static bool serverThreadPubOver = false;
static bool serverThreadLogOver = false;
static unsigned int portRep = PortServerRep;
static unsigned int portPub = PortServerPub;
static unsigned int period = 20;
static unsigned int logLengthSecs = (unsigned int)-1;
static bool serverStarting = false;

/**
 * In case of error exception in RhIO server 
 * threads, a signal is sent to self process.
 * The given error message is displayed.
 */
static void raiseSignalAndHold(const std::string& msg)
{
    //Display error message
    if (msg.length() > 0) {
        std::cerr << 
            "ERROR: uncaught exception in RhIO server thread: " 
            << msg << std::endl;
    } else {
        std::cerr << 
            "ERROR: uncaught unknown exception in RhIO server thread" 
            << std::endl;
    }
    std::cerr << 
        "ERROR: Holding the thread while a signal is raised" 
        << std::endl;
    //Send a signal to self so it can be catched
    raise(SIGABRT);
    //Hold
    while (true) {
        usleep(1000000);
    }
}

/**
 * Reply Server main loop handling
 * incomming Client request
 */
static void runServerRep()
{
    try {
        std::stringstream ss;
        ss << "tcp://*:" << portRep;
        ServerRep server(ss.str());
        //Notify main thread 
        //for initialization ready
        initServerCount++;

        //Set thread name
        prctl(PR_SET_NAME, "rhio_server_rep", 0, 0, 0);

        while (!serverThreadRepOver) {
            server.handleRequest();
        }
        initServerCount--;
    } catch (const std::string& e) {
        raiseSignalAndHold(e);
    } catch (const std::exception& e) {
        raiseSignalAndHold(e.what());
    } catch (...) {
        raiseSignalAndHold("");
    }
}

/**
 * Publisher Server main loop handling
 * streaming values
 */
static void runServerPub()
{
    try {
        //Allocating ServerStream
        std::stringstream ss;
        ss << "udp://" << AddressMulticast << ":" << portPub;
        ServerPub server(ss.str());
        ServerStream = &server;
        //Notify main thread 
        //for initialization ready
        initServerCount++;
        
        //Set thread name
        prctl(PR_SET_NAME, "rhio_server_pub", 0, 0, 0);

        while (!serverThreadPubOver) {
            int64_t tsStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            server.sendToClient();
            int64_t tsEnd = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            int64_t duration = tsEnd - tsStart;
            //Streaming value at target frequency (default is 50Hz)
            if (duration < period) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(period-duration));
            }
        }
        initServerCount--;
    } catch (const std::string& e) {
        raiseSignalAndHold(e);
    } catch (const std::exception& e) {
        raiseSignalAndHold(e.what());
    } catch (...) {
        raiseSignalAndHold("");
    }
}

/**
 * Logging Server main loop handling
 * memory allocation
 */
static void runServerLog()
{
    try {
        //Allocating ServerLogging
        ServerLog server;
        ServerLogging = &server;
        //Notify main thread 
        //for initialization ready
        initServerCount++;
        
        //Set thread name
        prctl(PR_SET_NAME, "rhio_server_log", 0, 0, 0);

        while (!serverThreadLogOver) {
            int64_t tsStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            if (logLengthSecs == (unsigned int)-1) {
                server.tick((int64_t)-1);
            } else {
                server.tick(logLengthSecs*1000000);
            }
            int64_t tsEnd = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
            int64_t duration = tsEnd - tsStart;
            //Logging value at target frequency (default is 50Hz)
            if (duration < period) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(period-duration));
            }
        }
        initServerCount--;
    } catch (const std::string& e) {
        raiseSignalAndHold(e);
    } catch (const std::exception& e) {
        raiseSignalAndHold(e.what());
    } catch (...) {
        raiseSignalAndHold("");
    }
}
void start(
    unsigned int portRep_, 
    unsigned int portPub_, 
    unsigned int period_,
    unsigned int logLengthSecs_)
{
    serverStarting = true;
    portRep = portRep_;
    portPub = portPub_;
    period = period_;
    logLengthSecs = logLengthSecs_;

    //Init atomic counter
    initServerCount = 0;
    //Start Server threads
    serverThreadRepOver = false;
    serverThreadPubOver = false;
    serverThreadLogOver = false;
    serverThreadRep = new std::thread(runServerRep);
    serverThreadPub = new std::thread(runServerPub);
    serverThreadLog = new std::thread(runServerLog);

    //Wait until both Server are initialized
    while (initServerCount != 3) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10));
    }
}

bool started()
{
    return serverStarting;
}

void stop()
{
    if (initServerCount > 0) {
        //Wait the end of server thread
        serverThreadLogOver = true;
        serverThreadPubOver = true;
        serverThreadRepOver = true;
        serverThreadLog->join();
        serverThreadPub->join();
        serverThreadRep->join();
        delete serverThreadLog;
        delete serverThreadPub;
        delete serverThreadRep;
    }
}

void reset()
{
    //Call destructor
    (&Root)->~IONode();
    //Call constructor with 
    //placement allocation
    new (&Root) IONode("ROOT", nullptr);
}

void writeLogs(const std::string& filepath)
{
    //Dump data from log 
    //server to file
    ServerLogging->writeLogsToFile(filepath);
}

void setRhIOTimeFunc(std::function<int64_t()> func)
{
    FuncGetTime = func;
}

/**
 * Ask and wait Server thread ending
 */
static void __attribute__ ((destructor)) stopThreadServer()
{
    stop();
}

}

