//
// Created by nestor on 23/09/19.
//

#include "Informe.h"

int Informe::getProductorMejorVenta() const {
    return productorMejorVenta;
}

void Informe::setProductorMejorVenta(int productorMejorVenta) {
    Informe::productorMejorVenta = productorMejorVenta;
}

TipoFlor Informe::getFlorMasCompada() const {
    return florMasComprada;
}

void Informe::setFlorMasCompada(TipoFlor florMasCompada) {
    Informe::florMasComprada = florMasCompada;
}

Informe::Informe(int productorMejorVenta, TipoFlor florMasCompada) : productorMejorVenta(productorMejorVenta),
                                                                     florMasComprada(florMasCompada) {}

Informe::Informe(const Informe &informe) : productorMejorVenta(informe.productorMejorVenta),
                                           florMasComprada(informe.florMasComprada) {}

Informe::Informe(const std::string &informe_serializado) {
    this->productorMejorVenta = atoi(informe_serializado.substr(0, TAM_CAMPO_PRODUCTOR).c_str());
    this->florMasComprada = Utils::getTipoFlor(
            atoi(informe_serializado.substr(TAM_CAMPO_PRODUCTOR, TAM_CAMPO_TIPOFLOR).c_str()));
}


