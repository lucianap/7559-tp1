//
// Created by nestor on 21/09/19.
//

#ifndef INC_7559_TP1_LOGGERPROCESS_H
#define INC_7559_TP1_LOGGERPROCESS_H

#include "../ProcesoHijo/ProcesoHijo.h"
#include "../Logger/Logger.h"
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

class LoggerProcess : public ProcesoHijo {

private:
    ofstream fs;

    Pipe entradaMensajes;

    string recibirMensaje(char *buffer);

    void iniciarAtencion();

public:
    LoggerProcess(string output, Logger & logger);

    /***
     * Crea el proceso hijo que atiendo los mensajes para el log
     * @return pid
     */
    pid_t ejecutar() override;

    void log(string basic_string);
};


#endif //INC_7559_TP1_LOGGERPROCESS_H
