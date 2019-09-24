#include <Guardador/Guardador.h>
#include <TipoProceso/TipoProceso.h>
#include "Distribuidor.h"
#include "../Signal/SignalHandler.h"


Distribuidor::Distribuidor(Logger& logger): ProcesoHijo(logger) {

}

Distribuidor::Distribuidor(Logger& logger, std::vector<Pipe*> ptos_de_venta,int idDistribuidor, Pipe* entrada)  :
        ProcesoHijo(logger),
        ptos_de_venta(ptos_de_venta),
        idDistribuidor(idDistribuidor),
        entradaFlores(*entrada) { // todo validar, te agrego el pipe por param.
}

Distribuidor::Distribuidor(Logger &logger, std::string distribuidorSerializado) : ProcesoHijo(logger) {

    int tamanioTipoProcesoBytes = 5;
    int tamanioIdBytes = 5;
    int tamanioCantTulipanes = 5;
    int tamanioCantRosas = 5;

    int tipo = std::stoi(distribuidorSerializado.substr(0, tamanioTipoProcesoBytes));
    this->idDistribuidor = std::stoi(distribuidorSerializado.substr(tamanioTipoProcesoBytes, tamanioIdBytes));

    // deserealizao tamaño y ramos tulipanes
    int sizeTulipanes = std::stoi(distribuidorSerializado.substr(tamanioTipoProcesoBytes + tamanioIdBytes, tamanioCantTulipanes));
    int posicionInicioTulipanes = tamanioTipoProcesoBytes + tamanioIdBytes + tamanioCantTulipanes;
    for(int i = 0; i < sizeTulipanes; i++) {
        //leo de un ramo
        int inicioRamo = i * Ramo::TAM_TOTAL + posicionInicioTulipanes;
        std::string ramoStr = distribuidorSerializado.substr(inicioRamo, Ramo::TAM_TOTAL);
        Ramo unRamo(ramoStr);
        stockTulipanes.push_back(unRamo);
    }

    // deserealizao tamaño y ramos rosas
    int inicioBloqueRosas = tamanioTipoProcesoBytes +
                            tamanioIdBytes +
                            tamanioCantTulipanes +
                            Ramo::TAM_TOTAL * sizeTulipanes;

    int sizeRosas = std::stoi(distribuidorSerializado.substr(inicioBloqueRosas, tamanioCantRosas));
    int posicionInicioRosas = inicioBloqueRosas + tamanioCantRosas;
    for (int i = 0; i < sizeRosas; i++) {
        //leo de un ramo
        int inicioRamo = i * Ramo::TAM_TOTAL + posicionInicioRosas;
        std::string ramoStr = distribuidorSerializado.substr(inicioRamo, Ramo::TAM_TOTAL);
        Ramo unRamo(ramoStr);
        stockRosas.push_back(unRamo);
    }

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

void Distribuidor::inicializarValores() {
    this->ptsVentaIterator = this->ptos_de_venta.begin();
}

void Distribuidor::iniciarAtencion() {
    int ramos_recibidos = 0;
    std::vector<Ramo*> stock;

    auto ptos_venta_iterator = ptos_de_venta.begin();
    char buffer[Cajon::TAM_TOTAL_BYTES];


    this->inicializarValores();
    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {
        try {

            Cajon unCajon = recibirCajon(buffer);

            if(!unCajon.estaVacio()) {
                this->clasificar(unCajon);
                this->logearStatus();
                if (this->hayDiponiblidadParaEnvio()) {
                    this->enviarAPuntosDeVenta();
                } else {
                }

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

    //FIN del pipe, siginifica que los productores están siendo destruidos.
    //Por convención tomo que muera al recibir el primer EOF.

    if(stoi(string(buffer)) == EOF) {
        stringstream ss;
        ss << "EOF RECIBIDO. Distribuidor " << idDistribuidor;
        logger.log(ss.str());

        //fin del pipe, significa que se está apagando todo.
        //Devuelvo un cajón vacío para indicar que se tiene que ir guardando.
        return Cajon();
    };

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

void Distribuidor::clasificar(Cajon &cajon) {

    vector<Ramo> rosas = cajon.filtrar(TipoFlor::Rosa);
    this->stockRosas.insert(this->stockRosas.end(), rosas.begin(), rosas.end());

    vector<Ramo> tulipanes = cajon.filtrar(TipoFlor::Tulipan);
    this->stockTulipanes.insert(this->stockTulipanes.end(), tulipanes.begin(), tulipanes.end());

}

bool Distribuidor::hayDiponiblidadParaEnvio() {
    this->logearStatus();

    return (this->stockTulipanes.size() + this->stockRosas.size()  >= CANT_RAMOS_PARA_ENVIO);
}

void Distribuidor::enviarAPuntosDeVenta() {
    this->puntoVenta_actual = *this->ptsVentaIterator;
    if(this->ptsVentaIterator == ptos_de_venta.end()) {
        this->ptsVentaIterator = ptos_de_venta.begin();
        this->siguientePtoVenta = 0;
        puntoVenta_actual = *this->ptsVentaIterator;
    }
    this->enviarCajon(puntoVenta_actual);
    ++this->ptsVentaIterator;
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


void Distribuidor::enviarCajon( Pipe *distribuidor_destino) {
    vector<Ramo> ramos(stockRosas.begin(), stockRosas.end());
    ramos.insert(ramos.end(),std::begin(stockTulipanes),std::end(stockTulipanes));
    std::stringstream ss;
    ss << "DISTRIBUIDOR " << this->idDistribuidor << " envía cajón a destino." << endl;
    logger.log(ss.str());

    std::stringstream header;
    Cajon c(ramos);
    header << std::setw(Utils::TAM_HEADER) << TipoProceso::DISTRIBUIDOR_T;
    std::string cajon_a_enviar = header.str() + c.serializar();

    std::stringstream ss2;
    ss2 << "Contenido del cajón: " << cajon_a_enviar.c_str() << endl;
    logger.log(ss.str());

    distribuidor_destino->escribir(cajon_a_enviar.c_str(), cajon_a_enviar.length());
    stockRosas.clear();
    stockTulipanes.clear();
}
