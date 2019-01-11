#include "rhio_common/Logging.hpp"

namespace RhIO {

/**
 * Shorthand function writing the given value
 * in binary format into an output stream.
 *
 * @param os Output stream.
 * @param val A typed value to write.
 */
template <typename T>
void writeBinary(std::ostream& os, const T& val)
{
    os.write((const char*)&val, sizeof(T));
}
template <>
void writeBinary<std::string>(std::ostream& os, const std::string& val)
{
    size_t len = val.length();
    os.write((const char*)&len, sizeof(size_t));
    os.write((const char*)val.c_str(), len);
}

/**
 * Shorthand function reading a value
 * in binary format from an input file stream.
 *
 * @param is Input file stream.
 * @param val A typed value to read.
 */
template <typename T>
void readBinary(std::ifstream& is, T& val)
{
    is.read((char*)&val, sizeof(T));
}
template <>
void readBinary<std::string>(std::ifstream& is, std::string& val)
{
    size_t len = 0;
    is.read((char*)&len, sizeof(size_t));
    val = std::string(len, '\0');
    is.read((char*)val.c_str(), len);
}

void RhIOWriteBinaryLog(
    std::ostream& os, 
    const std::map<std::string, size_t>& mappingBool,
    const std::map<std::string, size_t>& mappingInt,
    const std::map<std::string, size_t>& mappingFloat,
    const std::map<std::string, size_t>& mappingStr,
    const std::vector<LogValBool>& valuesBool,
    const std::vector<LogValInt>& valuesInt,
    const std::vector<LogValFloat>& valuesFloat,
    const std::vector<LogValStr>& valuesStr)
{
    //Write value name mapping sizes
    size_t sizeMapBool = mappingBool.size();
    size_t sizeMapInt = mappingInt.size();
    size_t sizeMapFloat = mappingFloat.size();
    size_t sizeMapStr = mappingStr.size();
    writeBinary(os, sizeMapBool);
    writeBinary(os, sizeMapInt);
    writeBinary(os, sizeMapFloat);
    writeBinary(os, sizeMapStr);

    //Write name mapping
    for (const auto& it : mappingBool) {
        writeBinary(os, it.first);
        writeBinary(os, it.second);
    }
    for (const auto& it : mappingInt) {
        writeBinary(os, it.first);
        writeBinary(os, it.second);
    }
    for (const auto& it : mappingFloat) {
        writeBinary(os, it.first);
        writeBinary(os, it.second);
    }
    for (const auto& it : mappingStr) {
        writeBinary(os, it.first);
        writeBinary(os, it.second);
    }
    
    //Write value data point sizes
    size_t sizeValBool = valuesBool.size();
    size_t sizeValInt = valuesInt.size();
    size_t sizeValFloat = valuesFloat.size();
    size_t sizeValStr = valuesStr.size();
    writeBinary(os, sizeValBool);
    writeBinary(os, sizeValInt);
    writeBinary(os, sizeValFloat);
    writeBinary(os, sizeValStr);

    //Write value data points
    for (size_t i=0;i<sizeValBool;i++) {
        writeBinary(os, valuesBool[i].id);
        writeBinary(os, valuesBool[i].timestamp);
        writeBinary(os, valuesBool[i].value);
    }
    for (size_t i=0;i<sizeValInt;i++) {
        writeBinary(os, valuesInt[i].id);
        writeBinary(os, valuesInt[i].timestamp);
        writeBinary(os, valuesInt[i].value);
    }
    for (size_t i=0;i<sizeValFloat;i++) {
        writeBinary(os, valuesFloat[i].id);
        writeBinary(os, valuesFloat[i].timestamp);
        writeBinary(os, valuesFloat[i].value);
    }
    for (size_t i=0;i<sizeValStr;i++) {
        writeBinary(os, valuesStr[i].id);
        writeBinary(os, valuesStr[i].timestamp);
        writeBinary(os, valuesStr[i].value);
    }
}

bool RhIOReadBinaryLog(
    std::ifstream& is,
    std::map<std::string, size_t>& mappingBool,
    std::map<std::string, size_t>& mappingInt,
    std::map<std::string, size_t>& mappingFloat,
    std::map<std::string, size_t>& mappingStr,
    std::vector<LogValBool>& valuesBool,
    std::vector<LogValInt>& valuesInt,
    std::vector<LogValFloat>& valuesFloat,
    std::vector<LogValStr>& valuesStr)
{
    //Reset containers
    mappingBool.clear();
    mappingInt.clear();
    mappingFloat.clear();
    mappingStr.clear();
    valuesBool.clear();
    valuesInt.clear();
    valuesFloat.clear();
    valuesStr.clear();
    
    size_t sizeMapBool = 0;
    size_t sizeMapInt = 0;
    size_t sizeMapFloat = 0;
    size_t sizeMapStr = 0;
    size_t sizeValBool = 0;
    size_t sizeValInt = 0;
    size_t sizeValFloat = 0;
    size_t sizeValStr = 0;
    
    //Read value name mapping sizes
    if (!is.good() || is.peek() == EOF) goto errorCase;
    readBinary(is, sizeMapBool);
    readBinary(is, sizeMapInt);
    readBinary(is, sizeMapFloat);
    readBinary(is, sizeMapStr);
    
    //Read name mapping
    for (size_t i=0;i<sizeMapBool;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingBool.insert(std::make_pair(name, id));
    }
    for (size_t i=0;i<sizeMapInt;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingInt.insert(std::make_pair(name, id));
    }
    for (size_t i=0;i<sizeMapFloat;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingFloat.insert(std::make_pair(name, id));
    }
    for (size_t i=0;i<sizeMapStr;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingStr.insert(std::make_pair(name, id));
    }
    
    //Read value data point sizes
    if (!is.good() || is.peek() == EOF) goto errorCase;
    readBinary(is, sizeValBool);
    readBinary(is, sizeValInt);
    readBinary(is, sizeValFloat);
    readBinary(is, sizeValStr);

    //Allocate memory
    if (!is.good() || is.peek() == EOF) goto errorCase;
    valuesBool.assign(sizeValBool, LogValBool());
    valuesInt.assign(sizeValInt, LogValInt());
    valuesFloat.assign(sizeValFloat, LogValFloat());
    valuesStr.assign(sizeValStr, LogValStr());
    
    //Read value data points
    for (size_t i=0;i<sizeValBool;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        readBinary(is, valuesBool[i].id);
        readBinary(is, valuesBool[i].timestamp);
        readBinary(is, valuesBool[i].value);
    }
    for (size_t i=0;i<sizeValInt;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        readBinary(is, valuesInt[i].id);
        readBinary(is, valuesInt[i].timestamp);
        readBinary(is, valuesInt[i].value);
    }
    for (size_t i=0;i<sizeValFloat;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        readBinary(is, valuesFloat[i].id);
        readBinary(is, valuesFloat[i].timestamp);
        readBinary(is, valuesFloat[i].value);
    }
    for (size_t i=0;i<sizeValStr;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        readBinary(is, valuesStr[i].id);
        readBinary(is, valuesStr[i].timestamp);
        readBinary(is, valuesStr[i].value);
    }
    
    return true;
    
    //Reading error case
errorCase:

    //Reset containers
    mappingBool.clear();
    mappingInt.clear();
    mappingFloat.clear();
    mappingStr.clear();
    valuesBool.clear();
    valuesInt.clear();
    valuesFloat.clear();
    valuesStr.clear();

    return false;
}
bool RhIOReadBinaryLog(
    std::ifstream& is,
    std::map<std::string, std::vector<LogValBool>>& containerBool,
    std::map<std::string, std::vector<LogValInt>>& containerInt,
    std::map<std::string, std::vector<LogValFloat>>& containerFloat,
    std::map<std::string, std::vector<LogValStr>>& containerStr)
{
    //Reset containers
    containerBool.clear();
    containerInt.clear();
    containerFloat.clear();
    containerStr.clear();
    
    size_t sizeMapBool = 0;
    size_t sizeMapInt = 0;
    size_t sizeMapFloat = 0;
    size_t sizeMapStr = 0;
    size_t sizeValBool = 0;
    size_t sizeValInt = 0;
    size_t sizeValFloat = 0;
    size_t sizeValStr = 0;
    std::map<size_t, std::string> mappingInvBool;
    std::map<size_t, std::string> mappingInvInt;
    std::map<size_t, std::string> mappingInvFloat;
    std::map<size_t, std::string> mappingInvStr;
    
    //Read value name mapping sizes
    if (!is.good() || is.peek() == EOF) goto errorCase;
    readBinary(is, sizeMapBool);
    readBinary(is, sizeMapInt);
    readBinary(is, sizeMapFloat);
    readBinary(is, sizeMapStr);
    
    //Read name mapping
    for (size_t i=0;i<sizeMapBool;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingInvBool.insert(std::make_pair(id, name));
        containerBool.insert(std::make_pair(
            name, std::vector<LogValBool>()));
    }
    for (size_t i=0;i<sizeMapInt;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingInvInt.insert(std::make_pair(id, name));
        containerInt.insert(std::make_pair(
            name, std::vector<LogValInt>()));
    }
    for (size_t i=0;i<sizeMapFloat;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingInvFloat.insert(std::make_pair(id, name));
        containerFloat.insert(std::make_pair(
            name, std::vector<LogValFloat>()));
    }
    for (size_t i=0;i<sizeMapStr;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        std::string name;
        size_t id;
        readBinary(is, name);
        readBinary(is, id);
        mappingInvStr.insert(std::make_pair(id, name));
        containerStr.insert(std::make_pair(
            name, std::vector<LogValStr>()));
    }
    
    //Read value data point sizes
    if (!is.good() || is.peek() == EOF) goto errorCase;
    readBinary(is, sizeValBool);
    readBinary(is, sizeValInt);
    readBinary(is, sizeValFloat);
    readBinary(is, sizeValStr);

    //Read value data points
    for (size_t i=0;i<sizeValBool;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        LogValBool val;
        readBinary(is, val.id);
        readBinary(is, val.timestamp);
        readBinary(is, val.value);
        containerBool.at(mappingInvBool.at(val.id)).push_back(val);
    }
    for (size_t i=0;i<sizeValInt;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        LogValInt val;
        readBinary(is, val.id);
        readBinary(is, val.timestamp);
        readBinary(is, val.value);
        containerInt.at(mappingInvInt.at(val.id)).push_back(val);
    }
    for (size_t i=0;i<sizeValFloat;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        LogValFloat val;
        readBinary(is, val.id);
        readBinary(is, val.timestamp);
        readBinary(is, val.value);
        containerFloat.at(mappingInvFloat.at(val.id)).push_back(val);
    }
    for (size_t i=0;i<sizeValStr;i++) {
        if (!is.good() || is.peek() == EOF) goto errorCase;
        LogValStr val;
        readBinary(is, val.id);
        readBinary(is, val.timestamp);
        readBinary(is, val.value);
        containerStr.at(mappingInvStr.at(val.id)).push_back(val);
    }
    
    return true;
    
    //Reading error case
errorCase:

    //Reset containers
    containerBool.clear();
    containerInt.clear();
    containerFloat.clear();
    containerStr.clear();

    return false;
}

}

