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
    return florMasCompada;
}

void Informe::setFlorMasCompada(TipoFlor florMasCompada) {
    Informe::florMasCompada = florMasCompada;
}

Informe::Informe(int productorMejorVenta, TipoFlor florMasCompada) : productorMejorVenta(productorMejorVenta),
                                                                     florMasCompada(florMasCompada) {}

Informe::Informe(const Informe &informe) : productorMejorVenta(informe.productorMejorVenta),
                                           florMasCompada(informe.florMasCompada) {}


