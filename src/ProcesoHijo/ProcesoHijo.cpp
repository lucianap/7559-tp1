#include <csignal>
#include <iostream>

#include "./ProcesoHijo.h"
#include "../Signal/SignalHandler.h"

pid_t ProcesoHijo::getProcessId() {
    return pid;
}

void ProcesoHijo::terminar() {
    logger.log("Finalizo el proceso "+to_string(getProcessId()));
    kill(getProcessId(), SIGINT);
}

ProcesoHijo::ProcesoHijo(Logger& logger) :
    logger(logger) {};

ProcesoHijo::~ProcesoHijo() = default;