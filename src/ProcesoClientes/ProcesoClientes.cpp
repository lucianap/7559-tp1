//
// Created by root on 22/9/19.
//


#include "ProcesoClientes.h"


ProcesoClientes::ProcesoClientes(Logger &logger, Pipe *pipePtoVenta, std::vector<t_parametros_pedido> paramPedidosInternet) :
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

    int llegaCliente;
    int pideRosa;
    auto pedidos_internet_iterator = this->paramPedidosInternet.begin();
    t_parametros_pedido pedido_actual = *pedidos_internet_iterator;
    //Distribuye uniformemente entre los distribuidores asignados a este productor.
    while (sigint_handler.getGracefulQuit() == 0) {
        llegaCliente = std::rand() % 2;
        if(llegaCliente == 1){
            pideRosa = std::rand() % 2;
            t_parametros_pedido pedido;
            pedido.cantTulipanes = pideRosa == 1? 1 : 0;
            pedido.cantRosas = pideRosa == 1? 0 : 1;
            this->enviar_pedido(pedido, Local);
        }
        if(pedidos_internet_iterator == this->paramPedidosInternet.end()) {
            pedidos_internet_iterator = this->paramPedidosInternet.begin();
            pedido_actual = *pedidos_internet_iterator;
        }
        this->enviar_pedido(pedido_actual, Internet);
        ++pedidos_internet_iterator;
        sleep(1);
    }

}

ProcesoClientes::~ProcesoClientes() {
    logger.log("Proceso de clientes destruido");
}

void ProcesoClientes::enviar_pedido(t_parametros_pedido pedido, TipoPedido tipoPedido) {
    //Pedido* pedido = new Pedido(pedido, tipoPedido);
};
