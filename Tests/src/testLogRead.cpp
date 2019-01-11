#include <iostream>
#include <fstream>
#include <rhio_common/Logging.hpp>

int main()
{
    std::ifstream file("/tmp/logBig");
    
    std::map<std::string, size_t> mappingBool;
    std::map<std::string, size_t> mappingInt;
    std::map<std::string, size_t> mappingFloat;
    std::map<std::string, size_t> mappingStr;
    std::vector<RhIO::LogValBool> valuesBool;
    std::vector<RhIO::LogValInt> valuesInt;
    std::vector<RhIO::LogValFloat> valuesFloat;
    std::vector<RhIO::LogValStr> valuesStr;
    bool isSuccess = RhIO::RhIOReadBinaryLog(
        file,
        mappingBool,
        mappingInt,
        mappingFloat,
        mappingStr,
        valuesBool,
        valuesInt,
        valuesFloat,
        valuesStr);

    if (!isSuccess) {
        std::cout << "Loading failed" << std::endl;
    }

    for (const auto& it : mappingBool) {
        std::cout << "Bool: " << it.first << std::endl;
    }
    for (const auto& it : mappingInt) {
        std::cout << "Int: " << it.first << std::endl;
    }
    for (const auto& it : mappingFloat) {
        std::cout << "Float: " << it.first << std::endl;
    }
    for (const auto& it : mappingStr) {
        std::cout << "Str: " << it.first << std::endl;
    }

    for (size_t i=0;i<valuesInt.size();i++) {
        if (valuesInt[i].id == mappingInt.at("controller/cycle")) {
            std::cout << valuesInt[i].timestamp << " " << valuesInt[i].value << std::endl;
        }
    }

    return 0;
}

