
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include "../ProcesoHijo/ProcesoHijo.h"
#include "../Pipes/Pipe.h"

class Distribuidor : public ProcesoHijo {

    Pipe* pipeEntrada;

public:
    Distribuidor(Logger& logger, Pipe* pipeEntrada);
    ~Distribuidor() override;

    pid_t ejecutar() override;

private:
    void iniciarAtencion();

};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
