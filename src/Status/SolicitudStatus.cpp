//
// Created by nestor on 24/09/19.
//

#include "SolicitudStatus.h"


string SolicitudStatus::serializar() {
    stringstream  ss;

    if (tipoSolicitud == TIPO_SOLICITUD_INFORME) {
        ss<<std::setw(1)<<this->tipoSolicitud<<std::setw(Ramo::TAM_TOTAL)<<"";
    } else if (tipoSolicitud == TIPO_SOLICITUD_ALTA_VENTA) {
        ss<<std::setw(1)<<this->tipoSolicitud<<std::setw(Ramo::TAM_TOTAL)<<this->ramo.serializar();
    }

    return ss.str();
}

SolicitudStatus::SolicitudStatus(int tipoSolicitud) : tipoSolicitud(tipoSolicitud), ramo(0, TipoFlor::Ninguno) {}

SolicitudStatus::SolicitudStatus(int tipoSolicitud, const Ramo &ramo) : tipoSolicitud(tipoSolicitud), ramo(ramo) {}

SolicitudStatus::SolicitudStatus(string solicitudSerializada): ramo(0, Ninguno) {
    if(stoi(solicitudSerializada) == EOF){
        this->tipoSolicitud = TIPO_SOLICITUD_EOF;
    }else{
        this->tipoSolicitud = atoi(solicitudSerializada.substr(0, 1).c_str());
        if (this->tipoSolicitud == TIPO_SOLICITUD_ALTA_VENTA) {
            Ramo unRamo(solicitudSerializada.substr(1, Ramo::TAM_TOTAL));
            this->ramo = unRamo;
        }
    };

}

int SolicitudStatus::getTipoSolicitud() const {
    return tipoSolicitud;
}

void SolicitudStatus::setTipoSolicitud(int tipoSolicitud) {
    SolicitudStatus::tipoSolicitud = tipoSolicitud;
}

Ramo &SolicitudStatus::getRamo() {
    return ramo;
}

void SolicitudStatus::setRamo(const Ramo &ramo) {
    SolicitudStatus::ramo = ramo;
}

std::string SolicitudStatus::toString() {
    std::stringstream ss;
    std::string tipoSolicitudText;
    switch (this->getTipoSolicitud()) {
        case TIPO_SOLICITUD_INFORME:
            tipoSolicitudText = "SOLICITUD_INFORME";
            break;
        case TIPO_SOLICITUD_ALTA_VENTA:
            tipoSolicitudText = "SOLICITUD_ALTA VENTA";
            break;

    }
    ss << "[tipoSolicitud:" << tipoSolicitudText << " Ramo: "<< this->getRamo().toString()<<"]";
    return ss.str();
}
