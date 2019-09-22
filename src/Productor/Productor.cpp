#include <Signal/SignalHandler.h>

#include <utility>
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
    int tamanioRamoEnBytes = 20;
    int tamanioSizeEnBytes = 5;
    int tamanioRamosPorCajonEnBytes = 5;

    int size = std::stoi(productorSerializado.substr(0,tamanioSizeEnBytes));
    for(int i = 0; i < size; i++){
        //leo un ramo
        int inicio = i*tamanioRamoEnBytes + tamanioSizeEnBytes;
        std::string ramoStr = productorSerializado.substr(inicio, inicio + tamanioRamoEnBytes);
        Ramo* r = new Ramo(ramoStr);
        ramosAEnviar.push_back(r);
    }

    //Ramos por cajon
    int inicioRamosPorCajon = tamanioRamoEnBytes*size+tamanioSizeEnBytes;
    int ramosPorCajon = std::stoi(productorSerializado.substr(inicioRamosPorCajon, inicioRamosPorCajon + tamanioRamosPorCajonEnBytes));
    this->ramosPorCajon = ramosPorCajon;

    //Por último avanzo el iterador al próximo distribuidor a postear.
    int inicio = tamanioRamoEnBytes*size+tamanioSizeEnBytes+tamanioRamosPorCajonEnBytes;
    int siguienteDistribuidorLeido = std::stoi(productorSerializado.substr(inicio, inicio + 5));
    for(int i = 0; i < siguienteDistribuidorLeido; i++) {
        distribuidoresIterator++;
    }
}

//Protocolo de serializacion: 5 bytes: cantidad de ramos a enviar, 20 bytes: repetido por ramo, 5 bytes: ramosPorCajon, útlimos 5 bytes: siguiente productor.
std::string Productor::serializar() {
    std::stringstream ss;
    ss << std::setw(5) << ramosAEnviar.size();
    for(auto it = ramosAEnviar.begin(); it != ramosAEnviar.end(); it++) {
        ss << (*it)->serializar();
    }
    ss << std::setw(5) << ramosPorCajon;
    ss << std::setw(5) << siguienteDistribuidor << endl;
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

        Ramo* r = this->producir_ramo();
        ramosAEnviar.push_back(r);

        if(ramosAEnviar.size() >= this->ramosPorCajon) {
            this->enviar_cajon(ramosAEnviar, distribuidorActual);
            ++this->distribuidoresIterator;
            ramosAEnviar.clear();

            //Guardo por cuál distribuidor voy, para poder reanudar después.
            ++siguienteDistribuidor;
        }

    }

    stringstream ss;
    ss<<"Productor serializado: ";
    ss<<this->serializar()<<endl;
    cout << ss.str();
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

    logger.log("Termino la tarea de la productor");
    SignalHandler::destruir();

    exit(0);
}

void Productor::agregarDistribuidor(Pipe* distribuidor) {
    this->distribuidores.push_back(distribuidor);
}

Ramo* Productor::producir_ramo() {
    //Solo para simular la producción del ramo.
    sleep(1);
    int rnd = std::rand() % 2;
    TipoFlor t = rnd == 1? TipoFlor::Rosa : TipoFlor::Tulipan;
    return new Ramo(this->id, t);
}

void Productor::enviar_cajon(std::vector<Ramo*> ramos, Pipe *distribuidor_destino) {
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






