
#include "ProcesoInicial.h"


using std::vector;


ProcesoInicial::ProcesoInicial(t_parametros parametros):
        parametros(parametros), logger(true), loggerProcess("log.txt", logger) {
}

void ProcesoInicial::iniciarEjecucion() {

    int productores = parametros.cantProductores;
    int distribuidores = parametros.cantDistribuidores;

    int ramos_por_cajon = 10;

    Menu menu;
    loggerProcess.ejecutar();
    logger.log("-----------Iniciando sistema-------------");

    //Mapa de asignación de productores a distribuidores.
    //La key es el número de orden del productor
    //El valor es el Pipe al cual se conecta el distribuidor.
    std::map<int, vector<Pipe*>> distribuidores_por_productor;

    for (int j = 0; j < distribuidores; ++j) {
        Pipe* pipeInDistribuidor = new Pipe();
        this->distribuidoresEntrada.push_back(pipeInDistribuidor);
        this->asignar_productor(j, pipeInDistribuidor, productores, &distribuidores_por_productor);
    }

    for (int j = 0; j < distribuidores; ++j) {
        Distribuidor* distribuidor = new Distribuidor(logger, j, this->distribuidoresEntrada.at(j));
        this->distribuidores.push_back(distribuidor);
        distribuidor->ejecutar();
    }

    //Creación de los productores en procesos separados..
    for(int i = 0; i < productores; i++) {
        if(distribuidores_por_productor.find(i) == distribuidores_por_productor.end()) {
            logger.log("No hay distribuidores para el productor " + to_string(i));
        } else {
            std::vector<Pipe*> distribuidores_escuchando = distribuidores_por_productor.at(i);
            Productor* p = new Productor(i, distribuidores_escuchando, ramos_por_cajon, logger);
            this->productores.push_back(p);
            p->ejecutar();

        }
    }

    menu.iniciar();

}

ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiar();
}

void ProcesoInicial::asignar_productor(const int j, Pipe* pipeInDistribuidor, const int cantidad_productores,
        std::map<int, vector<Pipe*>>* distribuidores_por_productor ) {

    //Se asigna el distribuidor j a un productor N y se guarda el resultado en el mapa distribuidores_por productor.
    int n = j % cantidad_productores;

    if(distribuidores_por_productor->find(n) == distribuidores_por_productor->end()) {
        std::vector<Pipe*> pipe_vector{pipeInDistribuidor};
        distribuidores_por_productor->insert(std::make_pair(n, pipe_vector));
    } else {
        std::vector<Pipe*> pipe_vector = distribuidores_por_productor->at(n);
        pipe_vector.push_back(pipeInDistribuidor);
    }

}


void ProcesoInicial::limpiar() {

    for (int i = 0; i < this->productores.size(); ++i) {
        ProcesoHijo* proceso = this->productores.at(i);
        proceso->terminar();
    }

    for (int i = 0; i < this->distribuidores.size(); ++i) {
        ProcesoHijo* proceso = this->distribuidores.at(i);
        proceso->terminar();
    }

    for (int j = 0; j < productores.size(); ++j) {
        delete(productores.at(j));
    }

    for (int j = 0; j < distribuidores.size(); ++j) {
        delete(distribuidores.at(j));
    }

    this->loggerProcess.terminar(); // tiene que ser el ultimo siempre

}
