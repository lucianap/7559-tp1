#include <iostream>
#include <Utils/Utils.h>
#include <FDRegistry/FDRegistry.h>

#include "ProcesoInicial/ProcesoInicial.h"


int main(int argc, char* argv[]) {
    atexit(FDRegistry::CloseFileDescriptors);

    // TODO: tomar parametros
    t_parametros parametros = Utils::tomarParametros(argc, argv);

    ProcesoInicial procesoInicial(parametros);

    if(parametros.reanudar) {
        procesoInicial.reanudarEjecucion();
    } else {
        procesoInicial.iniciarEjecucion();
    }

    return 0;
}

