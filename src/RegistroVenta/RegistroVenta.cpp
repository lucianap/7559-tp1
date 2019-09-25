//
// Created by nestor on 24/09/19.
//

#include "RegistroVenta.h"

void RegistroVenta::contabilizarRamoVendido(Ramo &ramo) {
    this->ramosVendidos.push_back(ramo);
}

Informe RegistroVenta::generarInforme() {
    return Informe(0, Ninguno);
}
