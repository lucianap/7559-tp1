#ifndef PRODUCTOR_H_
#define PRODUCTOR_H_

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <ProcesoHijo/ProcesoHijo.h>
#include <Pipes/Pipe.h>
#include "../Ramo/Ramo.h"
#include "../Cajon/Cajon.h"

class Productor : public ProcesoHijo {
    private:
        int id;
        int ramos_por_cajon;

        //acá pongo los file descriptors de los distribuidores.
        std::vector<Pipe*> distribuidores;
    
    public:
        Productor(int id, std::vector<Pipe*> distribuidores, int ramos_por_cajon, Logger& logger);
        Productor(int id, int ramos_por_cajon, Logger& logger);

        void agregar_distribuidor(Pipe* distribuidor);
        pid_t ejecutar() override;

    private:

        Ramo* producir_ramo();
        void enviar_cajon(std::vector<Ramo*> ramos, Pipe *distribuidor_destino);

        //Comienzo de la ejecución de un productor.
        //El comportamiento actual es armar un cajón y enviarlo por el pipe apenas termina de armarlo.
        void ejecutar_productor();


};

#endif /* PRODUCTOR_H_ */