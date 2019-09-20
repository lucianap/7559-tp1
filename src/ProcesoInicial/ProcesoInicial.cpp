
#include "ProcesoInicial.h"


using std::vector;


ProcesoInicial::ProcesoInicial(t_parametros parametros): parametros(parametros) {
}

void ProcesoInicial::iniciarEjecucion() {


    int productores = 2;
    int distribuidores = 2;

    int ramos_por_cajon = 10;

    Menu menu;
    Logger logger("log.txt", true);
    logger.log("Cargando");


    const int distribuidores_por_productor = distribuidores / productores;

    //Se guardan los files descriptors de los pipe creados para luego asignar sus extremos a otros procesos.
    std::vector<int[2]> file_d_productor_distribuidor(distribuidores);

    for(int i = 0; i < distribuidores; i++){
        //un pipe por distribuidor.
        //TODO usar la clase pipe.
        pipe(file_d_productor_distribuidor.at(i));

        if(fork() == 0) {
            //TODO EJECUTAR DISTRIBUIDOR ASIGNANDO EL EXTREMO DE LECTURA DEL PIPE RECIÉN CREADO
            exit(0);
        }
    }

    for (int j = 0; j < distribuidores; ++j) {
        Pipe* pipeInDistribuidor = new Pipe();
        this->distribuidoresEntrada.push_back(pipeInDistribuidor);
    }

    for (int j = 0; j < distribuidores; ++j) {
        Distribuidor* distribuidor = new Distribuidor(logger,
                            this->distribuidoresEntrada.at(j));
        this->distribuidoresList.push_back(distribuidor);
        distribuidor->ejecutar();
    }

    //Creación de los productores en procesos separados..
    for(int i = 0; i < productores; i++) {
        if(fork() == 0) {
            std::cout << "Soy hijo.." << std::endl;

            std::vector<int> distribuidores_escuchando;

            Productor* p = new Productor(getpid(), distribuidores_escuchando, ramos_por_cajon);
            p->ejecutar_productor();
            delete(p);
            exit(0);
        }
    }

    menu.iniciar();

}

ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiarDistribuidores();
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