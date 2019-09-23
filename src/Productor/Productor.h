#ifndef PRODUCTOR_H_
#define PRODUCTOR_H_

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <cstdlib>
#include <ProcesoHijo/ProcesoHijo.h>
#include <Pipes/Pipe.h>


#include "Ramo/Ramo.h"
#include "../Cajon/Cajon.h"

class Productor : public ProcesoHijo {
    private:

        int id;
        int ramosPorCajon;

        //acá pongo los file descriptors de los distribuidores.
        std::vector<Pipe*> distribuidores;
        std::vector<Pipe*>::iterator distribuidoresIterator;

    public:

        Productor(int id, std::vector<Pipe*> distribuidores, int ramos_por_cajon, Logger& logger);
        Productor(int id, int ramos_por_cajon, Logger& logger);
        Productor(Logger &logger, std::string productorSerializado);

        void agregarDistribuidor(Pipe* distribuidor);
        pid_t ejecutar() override;
        std::string serializar() override;

    private:

        void cerrarPipes();
        Ramo producir_ramo();
        void enviar_cajon(std::vector<Ramo> ramos, Pipe *distribuidor_destino);

        //Comienzo de la ejecución de un productor.
        //El comportamiento actual es armar un cajón y enviarlo por el pipe apenas termina de armarlo.
        void producir();
        void inicializarValores();

        //Datos del estado actual, para luego cargarlos cuando se reanude
        std::vector<Ramo> ramosAEnviar;
        int siguienteDistribuidor;

};

#endif /* PRODUCTOR_H_ */