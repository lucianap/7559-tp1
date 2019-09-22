#include <Ramo/Ramo.h>
#include <Cajon/Cajon.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger, int idDistribuidor, Pipe* entrada)  :
        ProcesoHijo(logger),
        idDistribuidor(idDistribuidor),
        entradaFlores(*entrada) { // todo validar, te agrego el pipe por param.
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
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

    logger.log("Naci como distribuidor y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea de la distribuidor");
    SignalHandler::destruir();

    exit(0);
}

void Distribuidor::iniciarAtencion() {
    char buffer[200]; // cambiarlo con Lu
    Cajon* paqueteCajon; // cambiarlo con Lu

    while (sigint_handler.getGracefulQuit() == 0) {
        try {

            paqueteCajon = recibirCajon(buffer);

            std::stringstream ss;
            ss << "DISTRIBUIDOR " << this->idDistribuidor << " recibe un cajón con el contenido:" << endl;
            for(auto it = paqueteCajon->ramos.begin(); it != paqueteCajon->ramos.end(); ++it ) {
                ss << "Ramo de " << (*it)->get_productor() << " con flores de tipo " << (*it)->getTipoFlor() << endl;
            }

            logger.log(ss.str());

            // todo agregar logica y enviar a punto de venta

        } catch (std::string &error) {
            logger.log("Error atendiendo a productores: " + error);
            break;
        }
    }

    entradaFlores.cerrar();
}

// todo: refactor
Cajon* Distribuidor::recibirCajon(char *buffer) {
    string mensajeError;

    //TODO este 200 debería ser un parámetro de configuración.
    ssize_t bytesleidos = entradaFlores.leer(static_cast<void*>(buffer), 200);

    std::stringstream ss;
    ss << "DISTRIBUIDOR "<< this->idDistribuidor << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != 200) { // cambiarlo con lu
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    Cajon* paqueteRecibido = new Cajon(buffer, 10);
    return paqueteRecibido;
};
