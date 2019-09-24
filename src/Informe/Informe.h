//
// Created by nestor on 23/09/19.
//

#ifndef INC_7559_TP1_INFORME_H
#define INC_7559_TP1_INFORME_H


#include <TipoFlor/TipoFlor.h>
#include "Utils/Utils.h"

class Informe {

private:

    /**
     * Productor que mas flores vendio
     */
    int productorMejorVenta;

    /**
     * La flor mas comprada
     */
    TipoFlor florMasCompada;


public:

    Informe(int productorMejorVenta, TipoFlor florMasCompada);

    Informe(const Informe & informe);

    int getProductorMejorVenta() const;

    void setProductorMejorVenta(int productorMejorVenta);

    TipoFlor getFlorMasCompada() const;

    void setFlorMasCompada(TipoFlor florMasCompada);
};


#endif //INC_7559_TP1_INFORME_H
