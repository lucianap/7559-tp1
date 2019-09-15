#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>

#include "Productor/Productor.h"

const int productores = 50;
const int distribuidores = 50;

const int ramos_por_cajon = 10;

int main() {

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