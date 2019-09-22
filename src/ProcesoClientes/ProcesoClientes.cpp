//
// Created by root on 22/9/19.
//


#include "ProcesoClientes.h"

ProcesoClientes::ProcesoClientes(Logger &logger, Pipe *pipePtoVenta, std::vector<t_parametros_pedido_internet> paramPedidosInternet) :
        ProcesoHijo(logger),
        pipePtoVenta(*pipePtoVenta),
        paramPedidosInternet(paramPedidosInternet){}

pid_t ProcesoClientes::ejecutar() {
    logger.log("Ejecutamos un Proceso de clientes");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    logger.log("Naci como Proceso de clientes y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea del Proceso de clientes");
    SignalHandler::destruir();

    exit(0);
}

void ProcesoClientes::iniciarAtencion() {

}

ProcesoClientes::~ProcesoClientes() {
    logger.log("Proceso de clientes destruido");
};
