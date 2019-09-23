#include <Ramo/Ramo.h>
#include <Pedido/Pedido.h>
#include <TipoProceso/TipoProceso.h>
#include "PuntoVenta.h"
#include "../Signal/SignalHandler.h"

PuntoVenta::PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeEntrada)  :
        ProcesoHijo(logger),
        idPuntoVenta(idPuntoVenta),
        pipeEntrada(*pipeEntrada){};

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
    char buffer_header[10];
    char buffer_cajon[Cajon::TAM_TOTAL_BYTES_DISTRIBUIDOR];
    char buffer_pedido[Pedido::TAM_TOTAL];
    Cajon* paqueteCajon;
    TipoProceso proceso_header;
    while (sigint_handler.getGracefulQuit() == 0) {
        try {

            std::stringstream ss;
            ss << "Soy el punto de venta y voy a intentar recibir el header del mensaje" << endl;
            proceso_header = recibirHeader(buffer_header);
            ss << proceso_header<< endl;

            t_parametros_pedido pedido_actual;

            if(proceso_header == CLIENTE_T){
                pedido_actual = recibirPedido(buffer_pedido);
            }else{
                paqueteCajon = recibirCajon(buffer_cajon);

                ss << "Pto Vta Nro " << this->idPuntoVenta << " recibe un cajón con el contenido:" << endl;

                for(auto it = paqueteCajon->ramos.begin(); it != paqueteCajon->ramos.end(); ++it ) {
                    ss << "Ramo de " << (*it)->get_productor() << " con flores de tipo " << (*it)->getTipoFlor() << endl;
                }

                logger.log(ss.str());
            }

        } catch (std::string &error) {
            logger.log("Error atendiendo personas: " + error);
            break;
        }
    }

    pipeEntrada.cerrar();
}



TipoProceso PuntoVenta::recibirHeader(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = pipeEntrada.leer(static_cast<void*>(buffer), 10);

    std::stringstream ss;
    ss << "PTO VENTA "<< this->idPuntoVenta << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != 10) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    std::string strTipoProceso = ((string)buffer).substr(0, 5);
    std::stringstream s;
    s << "Debuggin: " << strTipoProceso << endl;
    logger.log(s.str());
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

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    std::string strPedido = ((string)buffer).substr(0, 5);
    Pedido paqueteRecibido(buffer);
    t_parametros_pedido pedido;
    pedido.cantRosas = paqueteRecibido.get_rosas();
    pedido.cantTulipanes = paqueteRecibido.get_tulipanes();
    pedido.origen = paqueteRecibido.getTipoPedido();

    std::stringstream s;
    ss << "Rosas : " << pedido.cantRosas
        << "Tulipanes : " << pedido.cantTulipanes
        << "Origen : " << Utils::getTextTipoPedido( pedido.origen) << endl;
    logger.log(s.str());

    return pedido;

};

Cajon* PuntoVenta::recibirCajon(char *buffer) {
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

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    Cajon* paqueteRecibido = new Cajon(buffer, Cajon::CAPACIDAD_RAMOS_DISTRIBUIDOR);
    return paqueteRecibido;
};