#include "logging.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <time.h>
#include <iostream>
    
// Put a tag before the message that wil indicate the level
// 0 = info (just telling you something)
// 1 = warning (you should probably fix this)
// 2 = error (can recover from this)
// 3 = critical (cannot recover from this)
// 4 = wait (made a request and is waiting for a response)
// 5 = success (request was successful)
// 6 = fail (request was not successful)
// 7 = debug (idk yet)

void log(std::string message, int level) {

    // Get the current time
    time_t now = time(0);
    // Convert the time to a string
    char* dateTime = ctime(&now);
    // Remove the newline character
    dateTime[strlen(dateTime) - 1] = '\0';
    // Add [] to the beginning and end of the string
    std::string timestamp = "[";
    timestamp += dateTime;
    timestamp += "] ";

    // Make a string that will be set by the switch statement
    std::string levelString;

    
    switch (level) {
        case 0: levelString = "\x1b[0m[INFO] \033[0m"; break;
        case 1: levelString = "\x1b[33m[WARN] \033[0m"; break;
        case 2: levelString = "\x1b[31m[ERRO] \033[0m"; break;
        case 3: levelString = "\x1b[31m[CRIT] \033[0m"; break;
        case 4: levelString = "\x1b[36m[WAIT] \033[0m"; break;
        case 5: levelString = "\x1b[32m[SUCC] \033[0m"; break;
        case 6: levelString = "\x1b[31m[FAIL] \033[0m"; break;
        case 7: levelString = "\x1b[35m[DBUG] \033[0m"; break;
        default: levelString = "\x1b[37m[UNDF] \033[0m"; break;
    }

    // Print timestamp + levelString + message + newline 
    std::cout << timestamp << levelString << message << std::endl;











    //printf((timestamp + levelString + message + std::string("\n")).c_str());


}