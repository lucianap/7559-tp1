#include <iostream>
#include <Utils/Utils.h>
#include <FDRegistry/FDRegistry.h>

#include "ProcesoInicial/ProcesoInicial.h"


int main(int argc, char* argv[]) {
    atexit(FDRegistry::CloseFileDescriptors);

    t_parametros parametros = Utils::tomarParametros(argc, argv);

    if (parametros.cantDistribuidores > 10) {
        cout<<"La cantidad de distribuidores debe ser menor de 10"<<endl;
        return 0;
    }
    if (parametros.cantProductores > 10) {
        cout<<"La cantidad de productore debe ser menor de 10"<<endl;
        return 0;
    }
    if (parametros.cantPuntosVenta > 10) {
        cout<<"La cantidad de puntos de venta debe ser menor de 10"<<endl;
        return 0;
    }
    ProcesoInicial procesoInicial(parametros);

    if(parametros.reanudar) {
        procesoInicial.reanudarEjecucion();
    } else {
        procesoInicial.iniciarEjecucion();
    }

    return 0;
}

