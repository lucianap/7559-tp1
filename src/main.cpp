#include <iostream>
#include <Utils/Utils.h>

#include "ProcesoInicial/ProcesoInicial.h"


int main(int argc, char* argv[]) {

    // TODO: tomar parametros
    t_parametros parametros = Utils::tomarParametros(argc, argv);

    ProcesoInicial procesoInicial(parametros);
    procesoInicial.iniciarEjecucion();

    return 0;
}

