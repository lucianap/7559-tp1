#ifndef PRODUCTOR_H_
#define PRODUCTOR_H_

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include "../Ramo/Ramo.h"
#include "../Cajon/Cajon.h"

class Productor {
    private:
        int id;
        int ramos_por_cajon;

        //acá pongo los file descriptors de los distribuidores.
        std::vector<int> distribuidores;
    
    public:
        Productor(std::vector<int> distribuidores, const int ramos_por_cajon);
        Ramo producir_ramo();
        void enviar_cajon(std::vector<Ramo> ramos);

        //Comienzo de la ejecución de un productor.
        //El comportamiento actual es armar un cajón y enviarlo por el pipe apenas termina de armarlo.
        void ejecutar_productor();

};

#endif /* PRODUCTOR_H_ */