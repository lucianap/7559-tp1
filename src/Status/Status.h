//
// Created by nestor on 23/09/19.
//

#ifndef INC_7559_TP1_STATUS_H
#define INC_7559_TP1_STATUS_H


#include <Ramo/Ramo.h>
#include "ProcesoHijo/ProcesoHijo.h"
#include "Pipes/Pipe.h"

/**
 * Implementa el proceso que atiende consultas del menu
 * y almacena ramos vendidas que informan los puntos de venta.
 * */
class Status : public ProcesoHijo {

public:
    Status(Logger &logger);

    virtual ~Status();

private:
    Pipe pipeEntrada;

    Pipe pipeSalida;

    pid_t ejecutar() override;

    /**
     * Almacena los ramos vendidos
     */
    vector<Ramo> ramosVendidos;

};


#endif //INC_7559_TP1_STATUS_H
