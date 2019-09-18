#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>


#include "Productor/Productor.h"
#include "Distribuidor/Distribuidor.h"
#include "Menu/Menu.h"


using std::vector;

const int productores = 2;
const int distribuidores = 2;

const int ramos_por_cajon = 10;

void limpiarDistribuidores(vector<Distribuidor *> &vector);

int main() {

    Menu menu;
    Logger logger("log.txt", true);
    logger.log("Cargando oficina de aduanas de conculandia");


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

    vector<Distribuidor*> distribuidoresList;
    for (int j = 0; j < distribuidores; ++j) {
        Distribuidor* distribuidor = new Distribuidor(logger);
        distribuidoresList.push_back(distribuidor);
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

    // liberar recursos de memoria e ipc
    limpiarDistribuidores(distribuidoresList);
}

void limpiarDistribuidores(vector<Distribuidor *> &distribuidores) {

    for (int i = 0; i < distribuidores.size(); ++i) {
        Distribuidor* distribuidor = distribuidores.at(i);
        distribuidor->terminar();
        delete(distribuidor);
    }

}