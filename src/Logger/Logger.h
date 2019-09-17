#ifndef CONCULANDIA_LOGGER_H
#define CONCULANDIA_LOGGER_H

#include <iostream>
#include <string>

#include "../Lock/LockFile.h"

using namespace std;

class Logger {

public:
    explicit Logger(string output, bool debug);
    ~Logger();

    int getFileDescriptor();
    void log(string message);

private:
    LockFile lock;
    bool debug;

};


#endif // CONCULANDIA_LOGGER_H
