#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>

#include "Productor/Productor.h"

const int productores = 50;
const int distribuidores = 30;

const int ramos_por_cajon = 10;

int main() {

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

    while(true) {
        //TODO mostrar menu.
        //Opciones:
        //.............getstats
        //.............finalizar
        //.............??
    }
}