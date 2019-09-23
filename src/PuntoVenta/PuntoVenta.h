
#ifndef INC_7559_TP1_PUNTOVENTA_H
#define INC_7559_TP1_PUNTOVENTA_H

#include <Pipes/Pipe.h>
#include <Cajon/Cajon.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class PuntoVenta : public ProcesoHijo {

private:
    int idPuntoVenta;
    Pipe pipeEntrada;
    std::vector<Ramo> stock_flores;
    void iniciarAtencion();

    Cajon* recibirCajon(char *buffer);
    TipoProceso recibirData(char *buffer);

    TipoProceso recibirHeader(char *buffer);

    t_parametros_pedido recibirPedido(char *buffer);

public:

    PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeEntrada);

    ~PuntoVenta() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_PUNTOVENTA_H
