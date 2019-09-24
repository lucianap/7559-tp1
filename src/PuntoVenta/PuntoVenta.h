
#ifndef INC_7559_TP1_PUNTOVENTA_H
#define INC_7559_TP1_PUNTOVENTA_H

#include <Pipes/Pipe.h>
#include <Cajon/Cajon.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class PuntoVenta : public ProcesoHijo {

private:
    int idPuntoVenta;

    Pipe pipeEntrada;

    void iniciarAtencion();

    vector<Ramo> stockRosas;

    vector<Ramo> stockTulipanes;

    Cajon recibirCajon(char *buffer);

    TipoProceso recibirHeader(char *buffer);

    t_parametros_pedido recibirPedido(char *buffer);

    std::string serializar() override;

    void clasificar(Cajon paqueteCajon);

    void manejarPedido(t_parametros_pedido pedido);

    void printStock();

    void cerrarPipe();

public:

    PuntoVenta(Logger& logger, int idPuntoVenta, Pipe* pipeEntrada);
    PuntoVenta(Logger &logger, std::string puntoVentaSerializado);

    ~PuntoVenta() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_PUNTOVENTA_H
