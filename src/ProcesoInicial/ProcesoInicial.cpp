
#include "ProcesoInicial.h"


using std::vector;


ProcesoInicial::ProcesoInicial(t_parametros parametros): parametros(parametros) {
}

void ProcesoInicial::iniciarEjecucion() {

    int productores = 3; //parametros.cantProductores;
    int distribuidores = 8; //parametros.cantDistribuidores;

    int ramos_por_cajon = 10;

    Menu menu;
    Logger logger("log.txt", true);
    logger.log("Cargando");

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
        this->distribuidoresList.push_back(distribuidor);
        distribuidor->ejecutar();
    }

    //Creación de los productores en procesos separados..
    for(int i = 0; i < productores; i++) {
        if(fork() == 0) {
            std::cout << "Soy Productor Nro. " << i << std::endl;
            if(distribuidores_por_productor.find(0) == distribuidores_por_productor.end()) {

                std::cout << "No hay distribuidores para mi " << i << " :c "<< std::endl;

            } else {

                std::vector<Pipe*> distribuidores_escuchando = distribuidores_por_productor.at(0);
                Productor* p = new Productor(getpid(), distribuidores_escuchando, ramos_por_cajon, logger);
                p->ejecutar();

            }

        }
    }

    menu.iniciar();

}

ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiarDistribuidores();
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

void ProcesoInicial::limpiarDistribuidores() {

    for (int i = 0; i < this->distribuidoresList.size(); ++i) {
        Distribuidor* distribuidor = this->distribuidoresList.at(i);
        distribuidor->terminar();
        delete(distribuidor);
    }

    for (int j = 0; j < distribuidoresEntrada.size(); ++j) {
        delete(this->distribuidoresEntrada.at(j));
    }

}