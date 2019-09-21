#include <Ramo/Ramo.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger, int idDistribuidor)  :
        ProcesoHijo(logger),
        idDistribuidor(idDistribuidor),
        entradaFlores() { // todo validar
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
    char buffer[20];        // cambiarlo con Lu
    Ramo paqueteFlores(1); // cambiarlo con Lu

    while (sigint_handler.getGracefulQuit() == 0) {
        try {
            paqueteFlores = recibirPaquetesFlores(buffer);
            // todo agregar logica y enviar a punto de venta
        } catch (std::string &error) {
            logger.log("Error atendiendo personas: " + error);
            break;
        }
    }

    entradaFlores.cerrar();
}

// todo: refactor
Ramo Distribuidor::recibirPaquetesFlores(char *buffer) {
    string mensajeError;
    ssize_t bytesleidos = entradaFlores.leer(static_cast<void*>(buffer), 20);  // cambiarlo con lu
    if (bytesleidos != 20) { // cambiarlo con lu
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }
    Ramo paqueteRecibido(buffer);
    return paqueteRecibido;
};
