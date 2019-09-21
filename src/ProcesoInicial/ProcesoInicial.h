//
// Created by nestor on 18/09/19.
//

#ifndef INC_7559_TP1_PROCESOINICIAL_H
#define INC_7559_TP1_PROCESOINICIAL_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <map>
#include <Utils/Utils.h>

#include "Productor/Productor.h"
#include "Distribuidor/Distribuidor.h"
#include "Menu/Menu.h"
#include "ProcesoInicial/ProcesoInicial.h"

using std::vector;


class ProcesoInicial {

private:

    t_parametros parametros;
    vector<Distribuidor *> distribuidoresList;
    vector<Pipe *> distribuidoresEntrada;

    void limpiarDistribuidores();

public:
    ProcesoInicial(t_parametros parametros);
    ~ProcesoInicial();

    void iniciarEjecucion();

    void asignar_productor(const int j, Pipe* pipeInDistribuidor, const int cantidad_productores,
                            std::map<int, vector<Pipe*>>* distribuidores_por_productor );

};


#endif //INC_7559_TP1_PROCESOINICIAL_H
