#include <Guardador/Guardador.h>
#include <TipoProceso/TipoProceso.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger, std::vector<Pipe*> ptos_de_venta,int idDistribuidor, Pipe* entrada)  :
        ProcesoHijo(logger),
        ptos_de_venta(ptos_de_venta),
        idDistribuidor(idDistribuidor),
        entradaFlores(*entrada) { // todo validar, te agrego el pipe por param.
}

Distribuidor::Distribuidor(Logger &logger, std::string distribuidorSerializado) : ProcesoHijo(logger) {

    int tamanioTipoProcesoBytes = 5;
    int tamanioIdBytes = 5;

    int tipo = std::stoi(distribuidorSerializado.substr(0, tamanioTipoProcesoBytes));
    idDistribuidor = std::stoi(distribuidorSerializado.substr(tamanioTipoProcesoBytes,
            tamanioTipoProcesoBytes+tamanioIdBytes));

};


Distribuidor::~Distribuidor() {
    logger.log("Distribuidor destruido");
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
    int ramos_recibidos = 0;
    std::vector<Ramo*> stock;

    auto ptos_venta_iterator = ptos_de_venta.begin();
    char buffer[Cajon::TAM_TOTAL_BYTES];
    Cajon* paqueteCajon;

    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        try {
            Pipe* pto_venta_actual = *ptos_venta_iterator;
            paqueteCajon = recibirCajon(buffer);

            std::stringstream ss;
            ss << "DISTRIBUIDOR " << this->idDistribuidor << " recibe un cajón con el contenido:" << endl;
            for(auto it = paqueteCajon->ramos.begin(); it != paqueteCajon->ramos.end(); ++it ) {
                std::stringstream ss;
                ss << (*it)->toString() << endl;
                stock.push_back(*it);
                ramos_recibidos++;
                ss << "Cant actual " << ramos_recibidos << endl;
                logger.log(ss.str());
                if(ramos_recibidos == 10){
                    enviarCajon(stock, pto_venta_actual);
                    stock.clear();
                    ++ptos_venta_iterator;
                    ramos_recibidos = 0;
                }
            }


            if(ptos_venta_iterator == ptos_de_venta.end()) {
                ptos_venta_iterator = ptos_de_venta.begin();
                pto_venta_actual = *ptos_venta_iterator;
            }

            // todo agregar logica y enviar a punto de venta

        } catch (std::string &error) {
            logger.log("Error atendiendo a productores: " + error);
            break;
        }
    }

    if(sigusr1_handler.getSaveAndQuit() != 0) {
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


void Distribuidor::enviarCajon(std::vector<Ramo*> ramos, Pipe *distribuidor_destino) {

    std::stringstream ss;
    ss << "DISTRIBUIDOR " << this->idDistribuidor << " envía cajón a destino." << endl;
    logger.log(ss.str());

    Cajon c(ramos);
    std::string cajon_a_enviar = this->serializar() + c.serializar();

    std::stringstream ss2;
    ss2 << "Contenido del cajón: " << cajon_a_enviar.c_str() << endl;
    logger.log(ss.str());

    distribuidor_destino->escribir(cajon_a_enviar.c_str(), cajon_a_enviar.length());

}
