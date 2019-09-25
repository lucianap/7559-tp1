//
// Created by nestor on 24/09/19.
//

#ifndef INC_7559_TP1_REGISTROVENTA_H
#define INC_7559_TP1_REGISTROVENTA_H

#include <vector>
#include <Ramo/Ramo.h>
#include <Informe/Informe.h>

using std::vector;

class RegistroVenta {

private:
    vector<Ramo> ramosVendidos;

public:
    void contabilizarRamoVendido(Ramo &ramo);

    Informe generarInforme();
};


#endif //INC_7559_TP1_REGISTROVENTA_H
