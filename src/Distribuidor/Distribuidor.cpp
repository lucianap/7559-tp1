//
// Created by nestor on 17/09/19.
//

#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger, Pipe* pipeEntrada)  :
        ProcesoHijo(logger), pipeEntrada(pipeEntrada) {
}

Distribuidor::~Distribuidor() {
    logger.log("Distribuidor destruido");
}

pid_t Distribuidor::ejecutar() {

    logger.log("Ejecutamos una distribuidor");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    logger.log("Naci como distribuidor y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea de la distribuidor");
    SignalHandler::destruir();

    exit(0);
}

void Distribuidor::iniciarAtencion() {

    // TODO: LOOPEAR
}
