#include <Ramo/Ramo.h>
#include "PuntoVenta.h"
#include "../Signal/SignalHandler.h"

PuntoVenta::PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeDistribuidor, Pipe* pipeCliente)  :
        ProcesoHijo(logger),
        idPuntoVenta(idPuntoVenta),
        pipeDistribuidor(*pipeDistribuidor),
        pipeClientes(*pipeCliente){};

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
    char buffer[2000];
    Cajon* paqueteCajon;

    while (sigint_handler.getGracefulQuit() == 0) {
        try {

            std::stringstream ss;

            ss << "Soy el punto de venta y voy a intentar recibir un cajon" << endl;

            paqueteCajon = recibirCajon(buffer);

            ss << "Pto Vta Nro " << this->idPuntoVenta << " recibe un cajón con el contenido:" << endl;
            for(auto it = paqueteCajon->ramos.begin(); it != paqueteCajon->ramos.end(); ++it ) {
                ss << "Ramo de " << (*it)->get_productor() << " con flores de tipo " << (*it)->getTipoFlor() << endl;
            }
            logger.log(ss.str());
            // todo agregar logica recibir pedidos clientes
        } catch (std::string &error) {
            logger.log("Error atendiendo personas: " + error);
            break;
        }
    }

    pipeDistribuidor.cerrar();
}

// todo: refactor
Cajon* PuntoVenta::recibirCajon(char *buffer) {
    string mensajeError;

    //TODO este 200 debería ser un parámetro de configuración.
    ssize_t bytesleidos = pipeDistribuidor.leer(static_cast<void*>(buffer), 2000);

    std::stringstream ss;
    ss << "PTO VENTA "<< this->idPuntoVenta << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != 2000) { // cambiarlo con lu
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    Cajon* paqueteRecibido = new Cajon(buffer, 100);
    return paqueteRecibido;
};