
#ifndef INC_7559_TP1_PUNTOVENTA_H
#define INC_7559_TP1_PUNTOVENTA_H

#include <Pipes/Pipe.h>
#include <Cajon/Cajon.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class PuntoVenta : public ProcesoHijo {

private:
    int idPuntoVenta;
    Pipe pipeDistribuidor;
    Pipe pipeClientes;
    std::vector<Ramo> stock_flores;
    void iniciarAtencion();

    Cajon* recibirCajon(char *buffer);

public:

    PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeDistribuidor, Pipe* pipeClientes);

    ~PuntoVenta() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_PUNTOVENTA_H
