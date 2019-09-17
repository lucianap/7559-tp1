#ifndef CONCULANDIA_PROCESOHIJO_H
#define CONCULANDIA_PROCESOHIJO_H

#include <unistd.h>

#include "../Logger/Logger.h"
#include "../Signal/SIGINT_Handler.h"

using namespace std;

class ProcesoHijo {

public:

    pid_t getProcessId();
    void terminar();    // le mandamos SIGINT al proceso hijo

    /* Estos metodos hay que sobreescribirlos en la clase que implemente esta interfaz
     *
     * En el constructor, vamos a recibir los recursos de sincronizacion desde el proceso padre
     * En el destructor, vamos a liberar los recursos utilizados y recibidos
     * El metodo ejecutar, va a hacer el fork, retornando el pid para el proceso padre y ejecutando lo que
     * quiera en el hijo para finalmente morir y retornar 0 si exito o el codigo de error correspondiente
     * */
    explicit ProcesoHijo(Logger& logger);
    virtual ~ProcesoHijo();
    virtual pid_t ejecutar() = 0;

protected:
    Logger& logger;
    pid_t pid;
    SIGINT_Handler sigint_handler;

};


#endif // CONCULANDIA_PROCESOHIJO_H
