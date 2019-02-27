#include <fstream>
#include <stdexcept>
#include "rhio_server/ServerLog.hpp"

namespace RhIO {

ServerLog::ServerLog() :
    _bufferBool(1000000),
    _bufferInt(1000000),
    _bufferFloat(1000000),
    _bufferStr(1000000),
    _mappingBool(),
    _mappingInt(),
    _mappingFloat(),
    _mappingStr(),
    _valuesBool(),
    _valuesInt(),
    _valuesFloat(),
    _valuesStr(),
    _mutex()
{
}

void ServerLog::logBool(
    const std::string& name, 
    bool val, 
    int64_t timestamp)
{
    bool isOk = _bufferBool.appendFromWriter({name, timestamp, val});
    if (!isOk) {
        std::cout
            << "RhIO::logBool: Dropped value: "
            << name << std::endl;
    }
}
void ServerLog::logInt(
    const std::string& name, 
    int64_t val, 
    int64_t timestamp)
{
    bool isOk = _bufferInt.appendFromWriter({name, timestamp, val});
    if (!isOk) {
        std::cout
            << "RhIO::logInt: Dropped value: "
            << name << std::endl;
    }
}
void ServerLog::logFloat(
    const std::string& name, 
    double val, 
    int64_t timestamp)
{
    bool isOk = _bufferFloat.appendFromWriter({name, timestamp, val});
    if (!isOk) {
        std::cout
            << "RhIO::logFloat: Dropped value: "
            << name << std::endl;
    }
}
void ServerLog::logStr(
    const std::string& name, 
    const std::string& val, 
    int64_t timestamp)
{
    bool isOk = _bufferStr.appendFromWriter({name, timestamp, val});
    if (!isOk) {
        std::cout
            << "RhIO::logStr: Dropped value: "
            << name << std::endl;
    }
}
        
void ServerLog::tick()
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    //Swap double lock free 
    //buffer for all types
    _bufferBool.swapBufferFromReader();
    _bufferInt.swapBufferFromReader();
    _bufferFloat.swapBufferFromReader();
    _bufferStr.swapBufferFromReader();
    
    //Reference on value buffer to be logged
    const std::vector<LogNamedValBool>& bufBool = 
        _bufferBool.getBufferFromReader();
    size_t sizeBool = _bufferBool.getSizeFromReader();
    const std::vector<LogNamedValInt>& bufInt = 
        _bufferInt.getBufferFromReader();
    size_t sizeInt = _bufferInt.getSizeFromReader();
    const std::vector<LogNamedValFloat>& bufFloat = 
        _bufferFloat.getBufferFromReader();
    size_t sizeFloat = _bufferFloat.getSizeFromReader();
    const std::vector<LogNamedValStr>& bufStr = 
        _bufferStr.getBufferFromReader();
    size_t sizeStr = _bufferStr.getSizeFromReader();

    //Log values
    for (size_t i=0;i<sizeBool;i++) {
        if (_mappingBool.count(bufBool[i].name) == 0) {
            _mappingBool.insert(std::make_pair(
                bufBool[i].name, _mappingBool.size()));
        }
        size_t id = _mappingBool.at(bufBool[i].name);
        _valuesBool.push_back({
            id, bufBool[i].timestamp, bufBool[i].value});
    }
    for (size_t i=0;i<sizeInt;i++) {
        if (_mappingInt.count(bufInt[i].name) == 0) {
            _mappingInt.insert(std::make_pair(
                bufInt[i].name, _mappingInt.size()));
        }
        size_t id = _mappingInt.at(bufInt[i].name);
        _valuesInt.push_back({
            id, bufInt[i].timestamp, bufInt[i].value});
    }
    for (size_t i=0;i<sizeFloat;i++) {
        if (_mappingFloat.count(bufFloat[i].name) == 0) {
            _mappingFloat.insert(std::make_pair(
                bufFloat[i].name, _mappingFloat.size()));
        }
        size_t id = _mappingFloat.at(bufFloat[i].name);
        _valuesFloat.push_back({
            id, bufFloat[i].timestamp, bufFloat[i].value});
    }
    for (size_t i=0;i<sizeStr;i++) {
        if (_mappingStr.count(bufStr[i].name) == 0) {
            _mappingStr.insert(std::make_pair(
                bufStr[i].name, _mappingStr.size()));
        }
        size_t id = _mappingStr.at(bufStr[i].name);
        _valuesStr.push_back({
            id, bufStr[i].timestamp, bufStr[i].value});
    }
}
        
void ServerLog::writeLogsToFile(const std::string& filepath)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::ofstream file(filepath);
    if (file.is_open()) {
        RhIOWriteBinaryLog(
            file, 
            _mappingBool,
            _mappingInt,
            _mappingFloat,
            _mappingStr,
            _valuesBool,
            _valuesInt,
            _valuesFloat,
            _valuesStr);
    } else {
        throw std::runtime_error(
            "RhIO::ServerLog::writeLogs: "
            "Unable to write file: " 
            + filepath);
    }
}

}

