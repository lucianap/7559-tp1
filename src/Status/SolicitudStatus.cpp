//
// Created by nestor on 24/09/19.
//

#include "SolicitudStatus.h"


string SolicitudStatus::serializar() {
    stringstream  ss;

    if (tipoSolicitud == TIPO_SOLICITUD_INFORME) {
        ss<<this->tipoSolicitud<<std::setw(Ramo::TAM_TOTAL)<<"";
    } else if (tipoSolicitud == TIPO_SOLICITUD_ALTA_VENTA) {
        ss<<this->tipoSolicitud<<std::setw(Ramo::TAM_TOTAL)<<this->ramo.serializar();
    }

    return ss.str();
}

SolicitudStatus::SolicitudStatus(int tipoSolicitud) : tipoSolicitud(tipoSolicitud), ramo(0, TipoFlor::Tulipan) {}

SolicitudStatus::SolicitudStatus(int tipoSolicitud, const Ramo &ramo) : tipoSolicitud(tipoSolicitud), ramo(ramo) {}
