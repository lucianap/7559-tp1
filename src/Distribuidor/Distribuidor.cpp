

#include <Guardador/Guardador.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"

Distribuidor::Distribuidor(Logger& logger): ProcesoHijo(logger) {

}

Distribuidor::Distribuidor(Logger& logger, int idDistribuidor, Pipe* entrada)  :
        ProcesoHijo(logger),
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
    char buffer[Cajon::TAM_TOTAL_BYTES];

    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        try {

            Cajon unCajon = recibirCajon(buffer);

            this->clasificar(unCajon);
            this->logearStatus();
            if (this->hayDiponiblidadParaEnvio()) {
                this->enviarAPuntosDeVenta();
            } else {
            }

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

Cajon Distribuidor::recibirCajon(char *buffer) {
    string mensajeError;

    ssize_t bytesleidos = entradaFlores.leer(static_cast<void*>(buffer), Cajon::TAM_TOTAL_BYTES);

    std::stringstream ss;
    ss << "DISTRIBUIDOR "<< this->idDistribuidor << " lee " << bytesleidos << " bytes del pipe.";

    if (bytesleidos != Cajon::TAM_TOTAL_BYTES) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer la siguiente persona en la fifo";
        throw(std::string(mensajeError));
    }

    ss << "Datos recibidos: " << buffer << endl;
    logger.log(ss.str());

    Cajon unCajon(buffer, Cajon::CAPACIDAD_RAMOS);

    std::stringstream msg;
    msg << "DISTRIBUIDOR " << this->idDistribuidor << " recibe un cajón con el contenido:" << endl;
    for(auto it = unCajon.ramos.begin(); it != unCajon.ramos.end(); ++it ) {
        msg << (*it).toString() << endl;
    }
    logger.log(msg.str());


    return unCajon;
}

std::string Distribuidor::serializar() {
    std::stringstream ss;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << TipoProceso::DISTRIBUIDOR_T;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << this->idDistribuidor;

    return ss.str();

}

void Distribuidor::clasificar(Cajon &cajon) {

    vector<Ramo> rosas = cajon.filtrar(TipoFlor::Rosa);
    this->stockRosas.insert(this->stockRosas.end(), rosas.begin(), rosas.end());

    vector<Ramo> tulipanes = cajon.filtrar(TipoFlor::Tulipan);
    this->stockTulipanes.insert(this->stockTulipanes.end(), tulipanes.begin(), tulipanes.end());

}

bool Distribuidor::hayDiponiblidadParaEnvio() {
    return (this->stockTulipanes.size() >= CANT_RAMOS_PARA_ENVIO
            && this->stockRosas.size() >= CANT_RAMOS_PARA_ENVIO);
}

void Distribuidor::enviarAPuntosDeVenta() {
    // TODO: ENVIO A PUNTOS DE VENTA
}

const vector<Ramo> &Distribuidor::getStockRosas() const {
    return stockRosas;
}

void Distribuidor::setStockRosas(const vector<Ramo> &stockRosas) {
    Distribuidor::stockRosas = stockRosas;
}

const vector<Ramo> &Distribuidor::getStockTulipanes() const {
    return stockTulipanes;
}

void Distribuidor::setStockTulipanes(const vector<Ramo> &stockTulipanes) {
    Distribuidor::stockTulipanes = stockTulipanes;
}

void Distribuidor::logearStatus() {
    std::stringstream msg;
    msg << "STATUS DISTRIBUIDOR " << this->idDistribuidor
        << " [Rosas=" << this->stockRosas.size()
        << ", Tulipanes="<<this->stockTulipanes.size()<<"]";
    logger.log(msg.str());
}

