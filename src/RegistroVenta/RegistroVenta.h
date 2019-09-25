//
// Created by nestor on 24/09/19.
//

#ifndef INC_7559_TP1_REGISTROVENTA_H
#define INC_7559_TP1_REGISTROVENTA_H

#include <Ramo/Ramo.h>
#include <Informe/Informe.h>
#include <iostream>
#include <iterator>
#include <map>

class RegistroVenta {

private:
    std::map<TipoFlor , int> acumuladorFlores;
    std::map<int , int> acumuladorProductores;

public:
    void contabilizarRamoVendido(Ramo &ramo);

    Informe generarInformes();
};


#endif //INC_7559_TP1_REGISTROVENTA_H
