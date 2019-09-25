//
// Created by nestor on 23/09/19.
//

#ifndef INC_7559_TP1_STATUS_H
#define INC_7559_TP1_STATUS_H


#include <Ramo/Ramo.h>
#include "ProcesoHijo/ProcesoHijo.h"
#include "Pipes/Pipe.h"
#include "SolicitudStatus.h"
#include "RegistroVenta/RegistroVenta.h"

/**
 * Implementa el proceso que atiende consultas del menu
 * y almacena ramos vendidas que informan los puntos de venta.
 * */
class Status : public ProcesoHijo {

public:
    Status(Logger &logger);

    void cargar(std::string statusSerializado);

    virtual ~Status();

    Pipe getPipeEntrada();

    Pipe getPipeSalida();

    pid_t ejecutar() override;

    std::string serializar() override;

    void setCantPipes(int cant);

private:

    Pipe pipeEntrada;

    Pipe pipeSalida;

    RegistroVenta registroVenta;

    void iniciarAtencion();

    SolicitudStatus recibirSolicitud();

    void despacharSolicitud(SolicitudStatus status);

    void enviarInforme();


    int cantPipes;
};


#endif //INC_7559_TP1_STATUS_H
