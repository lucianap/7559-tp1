#ifndef HH_LOGGER_H
#define HH_LOGGER_H

#include <iostream>
#include <string>

#include "../Lock/LockFile.h"

using namespace std;

class Logger {

public:
    explicit Logger(string output, bool debug);
    ~Logger();

    void log(string message);

private:
    bool debug;

};


#endif