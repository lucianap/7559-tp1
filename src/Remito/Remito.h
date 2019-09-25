//
// Created by root on 25/9/19.
//

#ifndef INC_7559_TP1_REMITO_H
#define INC_7559_TP1_REMITO_H


#include <vector>
#include <Ramo/Ramo.h>
#include <fstream>

class Remito {
private:
    stringstream pedido;
    int idPuntoVenta;
public:
    Remito(int idPuntoVenta);
    void agregarRamo(Ramo ramo);
    void guardar();

    static void cleanUp();

    static void inicializar();

    static const std::string carpeta ;
};


#endif //INC_7559_TP1_REMITO_H
