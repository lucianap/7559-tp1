#include <Signal/SignalHandler.h>

#include <utility>
#include <Guardador/Guardador.h>

#include "Productor.h"

Productor::Productor(int id, std::vector<Pipe*> distribuidores, int ramos_por_cajon, Logger& logger):
        ProcesoHijo(logger), id(id), distribuidores(distribuidores), ramosPorCajon(ramos_por_cajon) {
    this->inicializarValores();
};

Productor::Productor(int id, int ramos_por_cajon, Logger &logger):
        ProcesoHijo(logger), id(id), ramosPorCajon(ramos_por_cajon) {
    this->inicializarValores();
};

Productor::Productor(Logger &logger, std::string productorSerializado) : ProcesoHijo(logger) {
    int tamanioTipoDeProceso = 5;
    int tamanioRamoEnBytes = 20;
    int tamanioSizeEnBytes = 5;
    int tamanioRamosPorCajonEnBytes = 5;

    int inicio = tamanioTipoDeProceso;
    int fin = tamanioTipoDeProceso+tamanioSizeEnBytes;

    int size = std::stoi(productorSerializado.substr(inicio, fin));
    for(int i = 0; i < size; i++){
        //leo un ramo
        int inicioRamo = i * tamanioRamoEnBytes + inicio;
        std::string ramoStr = productorSerializado.substr(inicioRamo, inicioRamo + tamanioRamoEnBytes);
        Ramo unRamo(ramoStr);
        ramosAEnviar.push_back(unRamo);
    }

    //Ramos por cajon
    inicio = tamanioRamoEnBytes*size+tamanioSizeEnBytes+tamanioTipoDeProceso;
    fin = inicio+tamanioRamosPorCajonEnBytes;
    int ramosPorCajonLeido = std::stoi(productorSerializado.substr(inicio, fin));
    this->ramosPorCajon = ramosPorCajonLeido;

    //Por último avanzo el iterador al próximo distribuidor a postear.
    inicio = fin;
    fin = inicio + 5;
    int siguienteDistribuidorLeido = std::stoi(productorSerializado.substr(inicio, fin));
    for(int i = 0; i < siguienteDistribuidorLeido; i++) {
        distribuidoresIterator++;
    }
}


std::string Productor::serializar() {
    std::stringstream ss;

    //5 bytes: tipo de proceso.
    ss << std::setw(5) << TipoProceso::PRODUCTOR_T;

    //5 bytes: cantidad de ramos a enviar.
    ss << std::setw(5) << ramosAEnviar.size();

    //20 bytes por ramo.
    for(auto it = ramosAEnviar.begin(); it != ramosAEnviar.end(); it++) {
        ss << (*it).serializar();
    }

    //5 bytes: cantidad de ramos por cajón
    ss << std::setw(5) << ramosPorCajon;

    //5 bytes: siguiente productor a entregar cajón.
    ss << std::setw(5) << siguienteDistribuidor;

    //5 bytes finales para el id del productor.
    ss << std::setw(5) << this->id << endl;
    return ss.str();
}


void Productor::producir() {

    //Distribuye uniformemente entre los distribuidores asignados a este productor.
    while (sigint_handler.getGracefulQuit() == 0 && sigusr1_handler.getSaveAndQuit() == 0) {

        //Voy recorriendo el array de distribuidores para ir enviando cajones equitativamente.
        Pipe* distribuidorActual = *this->distribuidoresIterator;

        if(this->distribuidoresIterator == distribuidores.end()) {
            this->distribuidoresIterator = distribuidores.begin();
            siguienteDistribuidor = 0;
            distribuidorActual = *this->distribuidoresIterator;
        }

        Ramo r = this->producir_ramo();
        ramosAEnviar.push_back(r);

        if (ramosAEnviar.size() >= this->ramosPorCajon) {
            this->enviar_cajon(ramosAEnviar, distribuidorActual);
            ++this->distribuidoresIterator;
            ramosAEnviar.clear();

            //Guardo por cuál distribuidor voy, para poder reanudar después.
            ++siguienteDistribuidor;
        }

    }

    if(sigusr1_handler.getSaveAndQuit() != 0) {
        cout << "Me di cuenta que me quieren guardar y me guardo" << endl;
        Guardador g;
        g.guardar(this);
    }

    this->cerrarPipes();

}

void Productor::cerrarPipes() {
    for(auto itPipes = this->distribuidores.begin(); itPipes != this->distribuidores.end(); itPipes++){
        (*itPipes)->cerrar();
    }
}

pid_t Productor::ejecutar() {
    logger.log("Ejecutamos un PRODUCTOR");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);
    this->producir();

    logger.log("Termino la tarea del productor");
    SignalHandler::destruir();

    exit(0);
}

void Productor::agregarDistribuidor(Pipe* distribuidor) {
    this->distribuidores.push_back(distribuidor);
}

Ramo Productor::producir_ramo() {
    //Solo para simular la producción del ramo.
    sleep(1);
    int rnd = std::rand() % 2;
    TipoFlor t = rnd == 1? TipoFlor::Rosa : TipoFlor::Tulipan;
    Ramo unRamo(this->id, t);
    return unRamo;
}

void Productor::enviar_cajon(std::vector<Ramo> ramos, Pipe *distribuidor_destino) {
    std::stringstream ss;
    ss << "PRODUCTOR " << this->id << " envía cajón a destino." << endl;
    logger.log(ss.str());

    Cajon c(ramos);
    std::string cajon_a_enviar = c.serializar();

    std::stringstream ss2;
    ss2 << "Contenido del cajón: " << cajon_a_enviar.c_str() << endl;
    logger.log(ss.str());

    distribuidor_destino->escribir(cajon_a_enviar.c_str(), cajon_a_enviar.length());

}

void Productor::inicializarValores() {
    this->distribuidoresIterator = this->distribuidores.begin();
    ramosAEnviar.reserve(10);
    siguienteDistribuidor = 0;
}






