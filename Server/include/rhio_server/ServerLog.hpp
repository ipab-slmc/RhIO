#ifndef RHIO_SERVERLOG_HPP
#define RHIO_SERVERLOG_HPP

#include <deque>
#include <string>
#include <list>
#include <mutex>
#include "RhIO.hpp"
#include "rhio_common/LockFreeDoubleQueue.hpp"
#include "rhio_common/Logging.hpp"

namespace RhIO {

/**
 * ServerLog
 *
 * Implement local value logging
 * in memory and binary file dump
 * in separate thread to allow real-time
 * logging.
 */
class ServerLog
{
    public:
        
        /**
         * Initialization 
         */
        ServerLog();
        
        /**
         * Append to logging buffer value for type
         * Bool, Int, Float, Str with given absolute
         * name and timestamp.
         * Real time compatible.
         */
        void logBool(
            const std::string& name, 
            bool val, 
            int64_t timestamp);
        void logInt(
            const std::string& name, 
            int64_t val, 
            int64_t timestamp);
        void logFloat(
            const std::string& name, 
            double val, 
            int64_t timestamp);
        void logStr(
            const std::string& name, 
            const std::string& val, 
            int64_t timestamp);

        /**
         * Allocate and transfert data from RT buffer 
         * to non-RT containers.
         * Clamp the history to given time length in microseconds.
         * If negative, no clamping.
         */
        void tick(int64_t lengthHistory = (int64_t)-1);

        /**
         * Write all logged data into 
         * file of given path
         */
        void writeLogsToFile(const std::string& filepath);

    private:

        /**
         * Named value data point to be 
         * logged in lock free buffer
         * (the name is not yet replaced by
         * an id to prevent memory allocation).
         */
        template <typename T>
        struct LogNamedValue {
            //Full absolute name
            std::string name;
            //Timestamp in microseconds
            int64_t timestamp;
            //Typed value
            T value;
        };

        /**
         * Named and typed value typedef
         */
        typedef LogNamedValue<bool> LogNamedValBool;
        typedef LogNamedValue<int64_t> LogNamedValInt;
        typedef LogNamedValue<double> LogNamedValFloat;
        typedef LogNamedValue<std::string> LogNamedValStr;
        
        /**
         * Lock free double buffer for RT logging
         * of bool, int, float and str values
         */
        LockFreeDoubleQueue<LogNamedValBool> _bufferBool;
        LockFreeDoubleQueue<LogNamedValInt> _bufferInt;
        LockFreeDoubleQueue<LogNamedValFloat> _bufferFloat;
        LockFreeDoubleQueue<LogNamedValStr> _bufferStr;

        /**
         * Non real time container for logged values
         * and names to ids mapping to reduce memory usage
         * in logs files.
         */
        std::map<std::string, size_t> _mappingBool;
        std::map<std::string, size_t> _mappingInt;
        std::map<std::string, size_t> _mappingFloat;
        std::map<std::string, size_t> _mappingStr;
        std::deque<LogValBool> _valuesBool;
        std::deque<LogValInt> _valuesInt;
        std::deque<LogValFloat> _valuesFloat;
        std::deque<LogValStr> _valuesStr;
        
        /**
         * Mutex protecting data during logs writing
         */
        std::mutex _mutex;
};

}

#endif

