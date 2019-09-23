

#include <Guardador/Guardador.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger, int idDistribuidor, Pipe* entrada)  :
        ProcesoHijo(logger),
        idDistribuidor(idDistribuidor),
        entradaFlores(*entrada) {
}

Distribuidor::Distribuidor(Logger &logger, std::string distribuidorSerializado) : ProcesoHijo(logger) {

    int tamanioTipoProcesoBytes = 5;
    int tamanioIdBytes = 5;

    int tipo = std::stoi(distribuidorSerializado.substr(0, tamanioTipoProcesoBytes));
    idDistribuidor = std::stoi(distribuidorSerializado.substr(tamanioTipoProcesoBytes,
            tamanioTipoProcesoBytes+tamanioIdBytes));

};


Distribuidor::~Distribuidor() {
}

pid_t Distribuidor::ejecutar() {

    logger.log("Ejecutamos un distribuidor");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

    logger.log("Naci como distribuidor y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea del distribuidor");
    SignalHandler::destruir();

    exit(0);
}

void Distribuidor::iniciarAtencion() {
    char buffer[Cajon::TAM_TOTAL_BYTES];
    Cajon* paqueteCajon;

    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        try {

            paqueteCajon = recibirCajon(buffer);

            if(paqueteCajon != NULL) {

                std::stringstream ss;
                ss << "DISTRIBUIDOR " << this->idDistribuidor << " recibe un cajón con el contenido:" << endl;
                for(auto it = paqueteCajon->ramos.begin(); it != paqueteCajon->ramos.end(); ++it ) {
                    //ss << "Ramo de " << (*it)->get_productor() << " con flores de tipo " << (*it)->getTipoFlor() << endl;
                    ss << (*it)->toString() << endl;
                }

                logger.log(ss.str());

                // todo agregar logica y enviar a punto de venta

            }



        } catch (std::string &error) {
            logger.log("Error atendiendo a productores: " + error);
            break;
        }
    }

    if(sigusr1_handler.getSaveAndQuit() != 0) {

        stringstream ss;
        ss << "Distribuidor: " << idDistribuidor << " sale." << endl;
        logger.log(ss.str());

        Guardador g;
        g.guardar(this);
    }

    entradaFlores.cerrar();
}

// todo: refactor
Cajon* Distribuidor::recibirCajon(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = entradaFlores.leer(static_cast<void*>(buffer), Cajon::TAM_TOTAL_BYTES);

    std::stringstream ss;
    ss << "DISTRIBUIDOR "<< this->idDistribuidor << " lee " << bytesleidos << " bytes del pipe." << endl;

    if (bytesleidos != Cajon::TAM_TOTAL_BYTES) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    //FIN del pipe, siginifica que los productores están siendo destruidos.
    //Por convención tomo que muera al recibir el primer EOF.
    if(stoi(string(buffer)) == EOF) {
        stringstream ss;
        ss << "EOF RECIBIDO. Distribuidor " << idDistribuidor;
        logger.log(ss.str());

        //fin del pipe, significa que se está apagando todo.
        return NULL;
    };

    Cajon* paqueteRecibido = new Cajon(buffer, 10);
    return paqueteRecibido;
}

std::string Distribuidor::serializar() {
    std::stringstream ss;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << TipoProceso::DISTRIBUIDOR_T;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << this->idDistribuidor;

    return ss.str();

}

