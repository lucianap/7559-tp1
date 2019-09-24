#include <Ramo/Ramo.h>
#include <Pedido/Pedido.h>
#include <TipoProceso/TipoProceso.h>
#include <Guardador/Guardador.h>
#include "PuntoVenta.h"
#include "../Signal/SignalHandler.h"

PuntoVenta::PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeEntrada)  :
        ProcesoHijo(logger),
        idPuntoVenta(idPuntoVenta),
        pipeEntrada(*pipeEntrada){};

PuntoVenta::PuntoVenta(Logger &logger, std::string puntoVentaSerializado) : ProcesoHijo(logger) {

    int tamanioTipoProcesoBytes = 5;
    int tamanioIdBytes = 5;
    int tamanioCantTulipanes = 5;
    int tamanioCantRosas = 5;

    int tipo = std::stoi(puntoVentaSerializado.substr(0, tamanioTipoProcesoBytes));
    this->idPuntoVenta = std::stoi(puntoVentaSerializado.substr(tamanioTipoProcesoBytes, tamanioIdBytes));

    // deserealizao tamaño y ramos tulipanes
    int sizeTulipanes = std::stoi(puntoVentaSerializado.substr(tamanioTipoProcesoBytes + tamanioIdBytes, tamanioCantTulipanes));
    int posicionInicioTulipanes = tamanioTipoProcesoBytes + tamanioIdBytes + tamanioCantTulipanes;
    for(int i = 0; i < sizeTulipanes; i++) {
        //leo de un ramo
        int inicioRamo = i * Ramo::TAM_TOTAL + posicionInicioTulipanes;
        std::string ramoStr = puntoVentaSerializado.substr(inicioRamo, Ramo::TAM_TOTAL);
        Ramo unRamo(ramoStr);
        stockTulipanes.push_back(unRamo);
    }

    // deserealizao tamaño y ramos rosas
    int inicioBloqueRosas = tamanioTipoProcesoBytes +
                            tamanioIdBytes +
                            tamanioCantTulipanes +
                            Ramo::TAM_TOTAL * sizeTulipanes;

    int sizeRosas = std::stoi(puntoVentaSerializado.substr(inicioBloqueRosas, tamanioCantRosas));
    int posicionInicioRosas = inicioBloqueRosas + tamanioCantRosas;
    for (int i = 0; i < sizeRosas; i++) {
        //leo de un ramo
        int inicioRamo = i * Ramo::TAM_TOTAL + posicionInicioRosas;
        std::string ramoStr = puntoVentaSerializado.substr(inicioRamo, Ramo::TAM_TOTAL);
        Ramo unRamo(ramoStr);
        stockRosas.push_back(unRamo);
    }

};

PuntoVenta::~PuntoVenta() {
    logger.log("Punto de venta destruido");
}

pid_t PuntoVenta::ejecutar() {

    logger.log("Ejecutamos un Punto de venta");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    logger.log("Naci como Punto de venta y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea del Punto de venta");
    SignalHandler::destruir();

    exit(0);
}

void PuntoVenta::iniciarAtencion() {
    char buffer_header[Utils::TAM_HEADER];
    char buffer_cajon[Cajon::TAM_TOTAL_BYTES_DISTRIBUIDOR];
    char buffer_pedido[Pedido::TAM_TOTAL];
    Cajon paqueteCajon;
    TipoProceso proceso_header;
    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        try {

            std::stringstream ss;
            ss << "Soy el punto de venta y voy a intentar recibir el header del mensaje" << endl;
            proceso_header = recibirHeader(buffer_header);

            if(proceso_header == CLIENTE_T){
                t_parametros_pedido pedido_actual;
                pedido_actual = recibirPedido(buffer_pedido);
                this->manejarPedido(pedido_actual);
            }else{
                paqueteCajon = recibirCajon(buffer_cajon);

                ss << "Pto Vta Nro " << this->idPuntoVenta << " recibe un cajón con el contenido:" << endl;
                this->clasificar(paqueteCajon);

                logger.log(ss.str());
            }

        } catch (std::string &error) {
            logger.log("Error atendiendo personas: " + error);
            break;
        }
    }

    if(sigusr1_handler.getSaveAndQuit() != 0) {
        logger.log( "Clientes: "+ to_string(this->idPuntoVenta) +" sale.");
        Guardador g;
        g.guardar_ptoVenta(this);

        //Cierro la canilla y espero a que me maten, eventualmente
        this->cerrarPipe();
    }

    //Espero a que me maten.
    while(sigint_handler.getGracefulQuit() == 0) {}
}


void PuntoVenta::cerrarPipe() {
    logger.log("Mando EOF a mis pipes. Productor "+to_string(this->idPuntoVenta));
    stringstream ss;
    ss << setw(Ramo::TAM_TOTAL) << EOF;
    //pipeStats->escribir(ss.str().c_str(), Utils::TAM_HEADER);
}

void PuntoVenta::clasificar(Cajon paqueteCajon){

    /*for(auto it = paqueteCajon.ramos.begin(); it != paqueteCajon.ramos.end(); ++it ) {
        std::stringstream ss;
        ss << "Ramo de " << (it)->get_productor() << " con flores de tipo " << (it)->getTipoFlor() << endl;
        logger.log(ss.str());
    }*/

    vector<Ramo> rosas = paqueteCajon.filtrar(TipoFlor::Rosa);
    this->stockRosas.insert(this->stockRosas.end(), rosas.begin(), rosas.end());

    vector<Ramo> tulipanes = paqueteCajon.filtrar(TipoFlor::Tulipan);
    this->stockTulipanes.insert(this->stockTulipanes.end(), tulipanes.begin(), tulipanes.end());
    this->printStock();
}

void PuntoVenta::manejarPedido(t_parametros_pedido pedido) {
    std::stringstream ss;
    ss << "Tipo Pedido: " << ((pedido.origen == 0)?"Internet":"Local")<< endl;
    int i;
    for(i = 0; i < pedido.cantTulipanes;i++){
        if(this->stockTulipanes.size()>0){
            Ramo ramo = stockTulipanes.back();
            stockTulipanes.pop_back();
            //enviar ramo stats
        }else{
            break;
        }
    }
    ss << "Cantidad Tulipanes enviados" << i<< endl;
    for(i = 0; i < pedido.cantRosas;i++){
        if(this->stockRosas.size()>0){
            Ramo ramo = stockRosas.back();
            stockRosas.pop_back();
            //enviar ramo stats
        }else{
            break;
        }
    }
    ss << "Cantidad Rosas enviados" << i << endl;
    logger.log(ss.str());
    this->printStock();
}

void PuntoVenta::printStock(){
    std::stringstream ss;
    ss << "Punto de venta " << this->idPuntoVenta << endl;
    ss << "Cantidad Tulipanes " << this->stockTulipanes.size() << endl;
    ss << "Cantidad Rosas " << this->stockRosas.size() << endl;
    logger.log(ss.str());
}

TipoProceso PuntoVenta::recibirHeader(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = pipeEntrada.leer(static_cast<void*>(buffer), Utils::TAM_HEADER);

    std::stringstream ss;
    ss << "PTO VENTA "<< this->idPuntoVenta << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != Utils::TAM_HEADER) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    logger.log(ss.str());

    std::string strTipoProceso = ((string)buffer).substr(0, 5);

    return (TipoProceso)std::stoi(Utils::trim(strTipoProceso));

};

t_parametros_pedido PuntoVenta::recibirPedido(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = pipeEntrada.leer(static_cast<void*>(buffer), Pedido::TAM_TOTAL);

    std::stringstream ss;
    ss << "PTO VENTA "<< this->idPuntoVenta << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos !=  Pedido::TAM_TOTAL) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }
    logger.log(ss.str());

    std::string strPedido = ((string)buffer).substr(0, Pedido::TAM_TOTAL);
    Pedido paqueteRecibido(buffer);
    t_parametros_pedido pedido;
    pedido.cantRosas = paqueteRecibido.get_rosas();
    pedido.cantTulipanes = paqueteRecibido.get_tulipanes();
    pedido.origen = paqueteRecibido.getTipoPedido();

    std::stringstream s;
    s << paqueteRecibido.toString()<< endl;
    logger.log(s.str());

    return pedido;

};

Cajon PuntoVenta::recibirCajon(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = pipeEntrada.leer(static_cast<void*>(buffer), Cajon::TAM_TOTAL_BYTES_DISTRIBUIDOR);

    std::stringstream ss;
    ss << "PTO VENTA "<< this->idPuntoVenta << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != Cajon::TAM_TOTAL_BYTES_DISTRIBUIDOR) { // cambiarlo con lu
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }
    logger.log(ss.str());

    Cajon paqueteRecibido(buffer, Cajon::CAPACIDAD_RAMOS_DISTRIBUIDOR);
    return paqueteRecibido;
};
std::string PuntoVenta::serializar() {
    std::stringstream ss;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << TipoProceso::VENDEDOR_T;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << this->idPuntoVenta;

    //5 bytes: cantidad de tulipanes.
    ss << std::setw(5) << stockTulipanes.size();

    //3 bytes por ramo de tulipan
    for(auto it = stockTulipanes.begin(); it != stockTulipanes.end(); it++) {
        ss << (*it).serializar();
    }

    //5 bytes: cantidad de rosas.
    ss << std::setw(5) << stockRosas.size();

    //3 bytes por ramo de rosas
    for(auto it = stockRosas.begin(); it != stockRosas.end(); it++) {
        ss << (*it).serializar();
    }

    return ss.str();

}
