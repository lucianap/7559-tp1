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
/**
 * CANT_FLORES | TAM_
 *
 *
 */
string RegistroVenta::serializar() {

    stringstream ss;
    long cantidadFlores = this->acumuladorFlores.size();
    ss<<std::setw(2)<<cantidadFlores;
    for (const auto& kv : this->acumuladorFlores) {
        ss<<std::setw(2)<<kv.first;
        ss<<std::setw(10)<<kv.second;
    }
    long cantidadProductores = this->acumuladorProductores.size();
    ss<<std::setw(2)<<cantidadProductores;
    for (const auto& kv : this->acumuladorProductores) {
        ss<<std::setw(2)<<kv.first;
        ss<<std::setw(10)<<kv.second;
    }

    return ss.str();
}

RegistroVenta::RegistroVenta(string serializado) {
    int TAM_CANT_FLORES = 2;
    int TAM_TIPO_FLOR = 2;
    int TAM_CANT_ACUMULADO = 10;
    long cantidadFlores = atol(serializado.substr(0,TAM_CANT_FLORES).c_str());

    for (int i = 0; i < cantidadFlores; ++i) {
        int posicionFlor = i*(TAM_TIPO_FLOR+TAM_CANT_ACUMULADO)  + TAM_CANT_FLORES;
        TipoFlor tipoFlor = Utils::getTipoFlor(atoi(serializado.substr(posicionFlor,TAM_TIPO_FLOR).c_str()));
        int acumulado = atoi(serializado.substr(posicionFlor+TAM_TIPO_FLOR, TAM_CANT_ACUMULADO).c_str());
        acumuladorFlores.insert(std::make_pair(tipoFlor, acumulado));
    }

    int posInicialProductores = cantidadFlores * (TAM_TIPO_FLOR + TAM_CANT_ACUMULADO) + TAM_CANT_FLORES;
    int TAM_CANT_PRODUCTOR = 2;
    int TAM_ID_PRODUCTOR = 2;
    int TAM_CANT_ACUMULADO_PROD = 10;

    long cantidadProductor = atol(serializado.substr(posInicialProductores,TAM_CANT_PRODUCTOR).c_str());
    for (int i = 0; i < cantidadProductor; ++i) {
        int posicionProd = i*(TAM_ID_PRODUCTOR+TAM_CANT_ACUMULADO_PROD) + TAM_CANT_PRODUCTOR + posInicialProductores;
        int idProductor = atoi(serializado.substr(posicionProd, TAM_ID_PRODUCTOR).c_str());
        int acumulado = atoi(serializado.substr(posicionProd + TAM_ID_PRODUCTOR, TAM_CANT_ACUMULADO_PROD).c_str());
        acumuladorProductores.insert(std::make_pair(idProductor, acumulado));
    }

}

RegistroVenta::RegistroVenta() {}
