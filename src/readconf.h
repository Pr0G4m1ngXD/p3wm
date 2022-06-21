

#include <iostream>
#include <fstream>


int readVarInt(const char* filelocation, const char* valueName, int returnInt);
std::string readVarStr(const char* filelocation, const char* valueName, std::string returnString);
unsigned long readVarUnLong(const char* filelocation, const char* valueName, unsigned long returnLong);