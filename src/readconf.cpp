#include "readconf.h"
#include "logging.h"
// stringstreams
#include <iostream> 
#include <string> 
#include <sstream> 

int readVarInt(const char* filelocation, const char* valueName, int returnInt) {
    // 1. Open the file
    // 2. For each line in the file
    //    check if the line contains the valueName between the first and a equal sign
    //    if it does, return the value between the equal sign and the end of the line
    // 3. if the valueName is not found, return returnInt
    // 4. close the file

    std::ifstream file;
    // check if the file exists
    file.open(filelocation);
    if (!file.is_open()) {
        log("Could not open config file" + std::string(filelocation), 3);
        return returnInt;
    }
    for (std::string line; std::getline(file, line);) { // for each line in the file
        if (line.find(valueName) != std::string::npos) { // if the line contains the valueName
            std::string value = line.substr(line.find("=") + 1); // get the value between the equal sign and the end of the line
            file.close();
            log("Found value: " + value, 7);
            return atoi(value.c_str()); // return the value
        }
    }
    // if we get here, the valueName was not found
    log("Could not find " + std::string(valueName) + " in " + std::string(filelocation), 1);
    return returnInt;
}
std::string readVarStr(const char* filelocation, const char* valueName, std::string returnString) {
    // 1. Open the file
    // 2. For each line in the file
    //    check if the line contains the valueName between the first and a equal sign
    //    if it does, return the value between the equal sign and the end of the line
    // 3. if the valueName is not found, return returnString
    // 4. close the file

    std::ifstream file;
    // check if the file exists
    file.open(filelocation);
    if (!file.is_open()) {
        log("Could not open config file: " + std::string(filelocation), 1);
        return returnString;
    }
    for (std::string line; std::getline(file, line);) { // for each line in the file
        if (line.find(valueName) != std::string::npos) { // if the line contains the valueName
            std::string value = line.substr(line.find("=") + 1); // get the value between the equal sign and the end of the line
            file.close();
            log("Found value: " + value, 7);
            return value; // return the value
        }
    }
    // if we get here, the valueName was not found
    log("Could not find " + std::string(valueName) + " in " + std::string(filelocation), 1);
    return returnString;
}
unsigned long readVarUnLong(const char* filelocation, const char* valueName, unsigned long returnLong) {
    // 1. Open the file
    // 2. For each line in the file
    //    check if the line contains the valueName between the first and a equal sign
    //    if it does, return the value between the equal sign and the end of the line
    // 3. if the valueName is not found, return returnLong
    // 4. close the file

    std::ifstream file;
    // check if the file exists
    file.open(filelocation);
    if (!file.is_open()) {
        log("Could not open config file: " + std::string(filelocation), 1);
        return returnLong;
    }
    for (std::string line; std::getline(file, line);) { // for each line in the file
        if (line.find(valueName) != std::string::npos) { // if the line contains the valueName
            //use stoul to convert the value to an unsigned long.
            // if the output of line.substr(line.find("=") + 1) starts with 0x, convert the hex value to an int first (stoul will turn the int to a ul)
            unsigned int x;   
            std::stringstream ss;
            ss << std::hex << line.substr(line.find("=") + 1);
            ss >> x;
            // convert the int to a unsigned long
            unsigned long value = x;
            file.close();
            log("Found value: " + std::to_string(value), 7);
            return value; // return the value
        }
    }
    // if we get here, the valueName was not found
    log("Could not find " + std::string(valueName) + " in " + std::string(filelocation), 1);
    return returnLong;
}