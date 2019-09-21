
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include <Fifos/FifoLectura.h>
#include <Pipes/Pipe.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class Distribuidor : public ProcesoHijo {


private:
    int idDistribuidor;
    Pipe entradaFlores;

    void iniciarAtencion();

    Ramo recibirPaquetesFlores(char *buffer);

public:
    Distribuidor(Logger& logger, int idDistribuidor);
    ~Distribuidor() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
