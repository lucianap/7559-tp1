#ifndef HH_LOGGER_H
#define HH_LOGGER_H

#include <iostream>
#include <string>
#include "../Pipes/Pipe.h"
using namespace std;

class Logger {

public:
    explicit Logger(bool debug);

    ~Logger();

    /***
     * Guarda el mensaje en el log
     * @param message
     */
    void log(string message);

    /***
     * Longitud del mensaje que va por el pipe
     */
    static const int MENSAGE_LOG_SIZE = 255;

    const Pipe &getPipe() const;

protected:

    bool debug;

    Pipe pipe;

};


#endif
