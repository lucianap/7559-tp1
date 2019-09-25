//
// Created by nestor on 24/09/19.
//

#include "RegistroVenta.h"

void RegistroVenta::contabilizarRamoVendido(Ramo &ramo) {


    std::map<TipoFlor, int>::iterator it = acumuladorFlores.find(ramo.getTipoFlor());
    if (it != acumuladorFlores.end()) {
        it->second++;
    } else {
        acumuladorFlores.insert(std::make_pair(ramo.getTipoFlor(), 1));
    }

    std::map<int, int>::iterator itProductor = acumuladorProductores.find(ramo.get_productor());
    if (itProductor != acumuladorProductores.end()) {
        itProductor->second++;
    } else {
        acumuladorProductores.insert(std::make_pair(ramo.get_productor(), 1));
    }
}

Informe RegistroVenta::generarInformes() {

    int maximo = 0;
    TipoFlor florMasComprada = TipoFlor::Ninguno;
    for (const auto& kv : this->acumuladorFlores) {
        if (kv.second > maximo) {
            florMasComprada = kv.first;
            maximo = kv.second;
        }
    }
    int maximoParaProductor = 0;
    int productorQueMasVendio = 99;
    for (const auto& kv : this->acumuladorProductores) {
        if (kv.second > maximoParaProductor) {
            productorQueMasVendio = kv.first;
            maximoParaProductor = kv.second;
        }
    }

    return Informe(productorQueMasVendio, florMasComprada);
}
