
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include "../ProcesoHijo/ProcesoHijo.h"

class Distribuidor : public ProcesoHijo {


public:
    Distribuidor(Logger& logger);
    ~Distribuidor() override;

    pid_t ejecutar() override;

private:
    void iniciarAtencion();

};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
