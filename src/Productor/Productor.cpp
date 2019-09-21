#include <Signal/SignalHandler.h>

#include <utility>
#include "Productor.h"

Productor::Productor(int id, std::vector<Pipe*> distribuidores, int ramos_por_cajon, Logger& logger):
        ProcesoHijo(logger), id(id), distribuidores(std::move(distribuidores)), ramos_por_cajon(ramos_por_cajon) {};

Productor::Productor(int id, int ramos_por_cajon, Logger &logger):
        ProcesoHijo(logger), id(id), ramos_por_cajon(ramos_por_cajon) {};

void Productor::ejecutar_productor(){

    std::cout << "Soy el hijo " << this->id << " y comienzo mi ejecución." << std::endl;
    std::vector<Ramo*> ramos_a_enviar;
    ramos_a_enviar.reserve(10);

    int ramos_producidos = 0;

    auto distribuidores_iterator = distribuidores.begin();

    bool TRUE = true;
    //Distribuye uniformemente entre los distribuidores asignados a este productor.
    while(TRUE) {

        //Voy recorriendo el array de distribuidores para ir enviando cajones equitativamente.
        Pipe* distribuidor_actual = *distribuidores_iterator;

        if(distribuidores_iterator == distribuidores.end()) {
            distribuidores_iterator = distribuidores.begin();
            distribuidor_actual = *distribuidores_iterator;
        }

        Ramo* r = this->producir_ramo();
        ramos_a_enviar.push_back(r);

        ramos_producidos++;
        
        if(ramos_producidos >= this->ramos_por_cajon) {
            std::cout << "Soy el hijo " << this->id << " y estoy mandando un cajón." << std::endl; 
            this->enviar_cajon(ramos_a_enviar, distribuidor_actual);
            ++distribuidores_iterator;
            ramos_a_enviar.clear();
        }

    }
}

pid_t Productor::ejecutar() {

    logger.log("Ejecutamos un PRODUCTOR");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    this->ejecutar_productor();

    logger.log("Termino la tarea de la productor");
    SignalHandler::destruir();

    exit(0);
}

void Productor::agregar_distribuidor(Pipe* distribuidor) {
    this->distribuidores.push_back(distribuidor);
}

Ramo* Productor::producir_ramo() {
    //Solo para simular la producción del ramo.
    sleep(5);
    return new Ramo(this->id);
}

void Productor::enviar_cajon(std::vector<Ramo*> ramos, Pipe *distribuidor_destino) {
    Cajon c(ramos);
    std::string cajon_a_enviar = c.serializar();
    distribuidor_destino->escribir(cajon_a_enviar.c_str(), sizeof(cajon_a_enviar.c_str()));

}


