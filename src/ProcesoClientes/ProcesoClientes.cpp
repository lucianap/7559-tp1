//
// Created by root on 22/9/19.
//
#include <Guardador/Guardador.h>
#include "ProcesoClientes.h"


ProcesoClientes::ProcesoClientes(Logger &logger, int idCliente, Pipe *pipePtoVenta, std::vector<t_parametros_pedido> paramPedidosInternet) :
        ProcesoHijo(logger),
        idCliente(idCliente),
        pipePtoVenta(pipePtoVenta),
        paramPedidosInternet(paramPedidosInternet){}

ProcesoClientes::ProcesoClientes(Logger &logger, std::string puntoVentaSerializado) : ProcesoHijo(logger) {
    int tamanioTipoProcesoBytes = 5;
    int tamanioIdBytes = 5;

    int tipo = std::stoi(puntoVentaSerializado.substr(0, tamanioTipoProcesoBytes));
    this->idCliente = std::stoi(puntoVentaSerializado.substr(tamanioTipoProcesoBytes, tamanioIdBytes));

    int cantidad_pedidos =  std::stoi(puntoVentaSerializado.substr(tamanioTipoProcesoBytes+tamanioIdBytes, tamanioIdBytes));
    int post_ini = tamanioTipoProcesoBytes+tamanioIdBytes;
    for(int i = 0;i <cantidad_pedidos;i++){
        t_parametros_pedido pedido;
        pedido.cantRosas =  std::stoi(puntoVentaSerializado.substr(post_ini, tamanioIdBytes));
        post_ini += tamanioIdBytes;
        pedido.cantTulipanes = std::stoi(puntoVentaSerializado.substr(post_ini, tamanioIdBytes));;
        post_ini += tamanioIdBytes;
        pedido.origen = (TipoPedido)std::stoi(puntoVentaSerializado.substr(post_ini, tamanioIdBytes));
        post_ini += tamanioIdBytes;
    }
}

pid_t ProcesoClientes::ejecutar() {
    logger.log("Ejecutamos un Proceso de clientes");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

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

    std::stringstream ss;
    ss << "Soy el proceso de clientes de id: "<< this->idCliente << endl;
    //Distribuye uniformemente entre los distribuidores asignados a este productor
    sleep(5);
    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        std::stringstream ss;
        llegaCliente = std::rand() % 2;
        if(llegaCliente == 1){
            ss << "Se genera un pedido local: "<< this->idCliente << endl;
            pideRosa = std::rand() % 2;
            t_parametros_pedido pedido;
            pedido.cantTulipanes = pideRosa == 1? 1 : 0;
            pedido.cantRosas = pideRosa == 1? 0 : 1;
            pedido.origen = LOCAL;
            this->enviar_pedido(pedido);
            sleep(1);
        }
        if(pedidos_internet_iterator == this->paramPedidosInternet.end()) {
            pedidos_internet_iterator = this->paramPedidosInternet.begin();
            pedido_actual = *pedidos_internet_iterator;
        }
        ss << "Se genera un pedido online: "<< this->idCliente << endl;
        this->enviar_pedido(pedido_actual);
        ++pedidos_internet_iterator;
        logger.log(ss.str());
        sleep(2);
    }
    if(sigusr1_handler.getSaveAndQuit() != 0) {
        logger.log( "Clientes: "+ to_string(this->idCliente) +" sale.");
        Guardador g;
        g.guardar_cliente(this);

        //Cierro la canilla y espero a que me maten, eventualmente
        this->cerrarPipe();
    }

    //Espero a que me maten.
    while(sigint_handler.getGracefulQuit() == 0) {}

}

void ProcesoClientes::cerrarPipe() {
    logger.log("Mando EOF a mis pipes. Productor "+to_string(this->idCliente));
    stringstream ss;
    ss << setw( Utils::TAM_HEADER) << EOF;
    pipePtoVenta->escribir(ss.str().c_str(), Utils::TAM_HEADER);
}

ProcesoClientes::~ProcesoClientes() {
    logger.log("Proceso de clientes destruido");
}

void ProcesoClientes::enviar_pedido(t_parametros_pedido param_pedido) {
    std::stringstream ss;
    //5 bytes: tipo de proceso.
    ss << std::setw(Utils::TAM_HEADER) << CLIENTE_T;

    Pedido pedido(this->idCliente, param_pedido);
    std::string pedido_serializado = pedido.serializar();
    ss << std::setw(Pedido::TAM_TOTAL)<< pedido_serializado;
    std::string data_envio = ss.str();

    pipePtoVenta->escribir(data_envio.c_str(), data_envio.length());

};
std::string ProcesoClientes::serializar() {
    std::stringstream ss;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << TipoProceso::CLIENTE_T;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << this->idCliente;

    //5 bytes: cantidad de pedidos.
    ss << std::setw(5) << paramPedidosInternet.size();

    for(auto it = paramPedidosInternet.begin(); it != paramPedidosInternet.end(); it++) {
        ss << std::setw(5)  << (*it).cantTulipanes;
        ss << std::setw(5)  << (*it).cantRosas;
        ss << std::setw(5)  << (*it).origen;
    }

    return ss.str();

}