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
    TipoFlor florMasComprada;


public:

    Informe(int productorMejorVenta, TipoFlor florMasCompada);

    Informe(const Informe & informe);
    Informe(const std::string& informe_serializado);

    int getProductorMejorVenta() const;

    void setProductorMejorVenta(int productorMejorVenta);

    TipoFlor getFlorMasCompada() const;

    void setFlorMasCompada(TipoFlor florMasCompada);

    static const int TAM_CAMPO_PRODUCTOR = 2;
    static const int TAM_CAMPO_TIPOFLOR = 1;
    static const int TAM_TOTAL_BYTES = TAM_CAMPO_PRODUCTOR+TAM_CAMPO_TIPOFLOR;

};


#endif //INC_7559_TP1_INFORME_H
