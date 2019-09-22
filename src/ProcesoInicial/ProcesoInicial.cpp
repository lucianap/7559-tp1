
#include <Guardador/Guardador.h>
#include "ProcesoInicial.h"


using std::vector;


ProcesoInicial::ProcesoInicial(t_parametros parametros):
        parametros(parametros), logger(true), loggerProcess("log.txt", logger) {
}

void ProcesoInicial::iniciarEjecucion() {

    int productores = parametros.cantProductores;
    int distribuidores = parametros.cantDistribuidores;

    Guardador::cleanUp();
    Guardador::inicializar();

    int ramos_por_cajon = 10;

    /***** inicializamos los pipes para todo el sistema *****/

    //Mapa de asignación de productores a distribuidores.
    //La key es el número del Productor, el valor es el vector de Pipes que tienen su salida en los distintos distribuidores.
    std::map<int, vector<Pipe*>> distribuidores_por_productor;
    for (int j = 0; j < distribuidores; ++j) {
        Pipe* pipeInDistribuidor1 = new Pipe();
        this->distribuidoresEntrada.push_back(pipeInDistribuidor1);
        this->asignar_productor(j, pipeInDistribuidor1, productores, &distribuidores_por_productor);
    }

    /**** hasta este punto se deben inicializar todos los pipes ****/
    loggerProcess.ejecutar();
    logger.log("-----------Iniciando sistema-------------");

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

    for (int j = 0; j < distribuidores; ++j) {
        Distribuidor* distribuidor = new Distribuidor(logger, j, this->distribuidoresEntrada.at(j));
        this->distribuidores.push_back(distribuidor);
        distribuidor->ejecutar();
    }

    Menu menu;

    menu.agregarProcesos(this->productores);
    menu.agregarProcesos(this->distribuidores);
    menu.iniciar();

}

ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiar();
}

void ProcesoInicial::asignar_productor(const int j, Pipe* pipeInDistribuidor, const int cantidad_productores,
        std::map<int, vector<Pipe*>>* distribuidores_por_productor ) {

    //Se asigna el Distribuidor j a un Productor N y se guarda el resultado en el mapa distribuidores_por Productor.
    int n = j % cantidad_productores;

    if(distribuidores_por_productor->find(n) == distribuidores_por_productor->end()) {
        std::vector<Pipe*> pipe_vector{pipeInDistribuidor};
        distribuidores_por_productor->insert(std::make_pair(n, pipe_vector));
    } else {
        std::vector<Pipe*> pipe_vector = distribuidores_por_productor->at(n);
        pipe_vector.push_back(pipeInDistribuidor);
        distribuidores_por_productor->erase(n);
        distribuidores_por_productor->insert(std::make_pair(n, pipe_vector));
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
