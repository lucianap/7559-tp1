//
// Created by nestor on 24/09/19.
//

#ifndef INC_7559_TP1_SOLICITUDSTATUS_H
#define INC_7559_TP1_SOLICITUDSTATUS_H


#include <Ramo/Ramo.h>
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

class SolicitudStatus {

private:

    int tipoSolicitud;

    Ramo ramo;

public:

    static const int TIPO_SOLICITUD_INFORME = 1;
    static const int TIPO_SOLICITUD_ALTA_VENTA = 2;
    static const int TIPO_SOLICITUD_EOF = 3;
    static const int TAM_TOTAL = 1 + Ramo::TAM_TOTAL;

    string serializar();

    SolicitudStatus(int tipoSolicitud);

    SolicitudStatus(int tipoSolicitud, const Ramo &ramo);

    SolicitudStatus(string solicitudSerializada);

    int getTipoSolicitud() const;

    void setTipoSolicitud(int tipoSolicitud);

    Ramo &getRamo();

    void setRamo(const Ramo &ramo);

    std::string toString();
};


#endif //INC_7559_TP1_SOLICITUDSTATUS_H
