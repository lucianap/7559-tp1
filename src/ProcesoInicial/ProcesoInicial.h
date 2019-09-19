//
// Created by nestor on 18/09/19.
//

#ifndef INC_7559_TP1_PROCESOINICIAL_H
#define INC_7559_TP1_PROCESOINICIAL_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>

#include "Productor/Productor.h"
#include "Distribuidor/Distribuidor.h"
#include "Menu/Menu.h"
#include "ProcesoInicial/ProcesoInicial.h"

using std::vector;


class ProcesoInicial {



private:

    vector<Distribuidor *> distribuidoresList;
    vector<Pipe *> distribuidoresEntrada;

    void limpiarDistribuidores();

public:
    ProcesoInicial();
    ~ProcesoInicial();

    void iniciarEjecucion();

};


#endif //INC_7559_TP1_PROCESOINICIAL_H
