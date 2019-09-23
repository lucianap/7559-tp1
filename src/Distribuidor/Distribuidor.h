
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include <TipoProceso/TipoProceso.h>
#include <Ramo/Ramo.h>
#include <Cajon/Cajon.h>
#include "Distribuidor.h"
#include <Fifos/FifoLectura.h>
#include <Pipes/Pipe.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class Distribuidor : public ProcesoHijo {

private:
    int idDistribuidor;
    Pipe entradaFlores;

    void iniciarAtencion();

    Cajon* recibirCajon(char *buffer);

    std::string serializar() override;

public:
    Distribuidor(Logger& logger, int idDistribuidor, Pipe* pipe);
    Distribuidor(Logger& logger, std::string distribuidorSerializado);
    ~Distribuidor() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
