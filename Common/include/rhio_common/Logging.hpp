#ifndef RHIO_LOGGING_HPP
#define RHIO_LOGGING_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <string>

namespace RhIO {

/**
 * Structure for RhIO typed 
 * value logged in binary file
 */
template <typename T>
struct LogValue {
    //Value's Id use to map to value's name
    //(used to reduce storage size)
    size_t id;
    //Timestamp in microseconds
    int64_t timestamp;
    //Typed value
    T value;
};

/**
 * Typed logged value typedef
 */
typedef LogValue<bool> LogValBool;
typedef LogValue<int64_t> LogValInt;
typedef LogValue<double> LogValFloat;
typedef LogValue<std::string> LogValStr;

/**
 * Write in custom binary format the mapping
 * from values name to values id and all
 * values containers into output stream.
 */
void RhIOWriteBinaryLog(
    std::ostream& os, 
    const std::map<std::string, size_t>& mappingBool,
    const std::map<std::string, size_t>& mappingInt,
    const std::map<std::string, size_t>& mappingFloat,
    const std::map<std::string, size_t>& mappingStr,
    const std::deque<LogValBool>& valuesBool,
    const std::deque<LogValInt>& valuesInt,
    const std::deque<LogValFloat>& valuesFloat,
    const std::deque<LogValStr>& valuesStr);

/**
 * Read and load the values name to id mapping
 * and values containers from given input file 
 * stream in custom binary format.
 *
 * False is returned if the stream is empty and 
 * if no data has been loaded. 
 * Else, return True if the loaded data are valid.
 */
bool RhIOReadBinaryLog(
    std::ifstream& is,
    std::map<std::string, size_t>& mappingBool,
    std::map<std::string, size_t>& mappingInt,
    std::map<std::string, size_t>& mappingFloat,
    std::map<std::string, size_t>& mappingStr,
    std::vector<LogValBool>& valuesBool,
    std::vector<LogValInt>& valuesInt,
    std::vector<LogValFloat>& valuesFloat,
    std::vector<LogValStr>& valuesStr);
bool RhIOReadBinaryLog(
    std::ifstream& is,
    std::map<std::string, std::vector<LogValBool>>& containerBool,
    std::map<std::string, std::vector<LogValInt>>& containerInt,
    std::map<std::string, std::vector<LogValFloat>>& containerFloat,
    std::map<std::string, std::vector<LogValStr>>& containerStr);

}

#endif

