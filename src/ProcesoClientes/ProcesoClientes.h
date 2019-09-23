//
// Created by root on 22/9/19.
//

#ifndef INC_7559_TP1_PROCESOCLIENTES_H
#define INC_7559_TP1_PROCESOCLIENTES_H

#include <Signal/SignalHandler.h>
#include <Pipes/Pipe.h>
#include <vector>
#include <Cajon/Cajon.h>
#include <ProcesoHijo/ProcesoHijo.h>
#include <TipoPedido/TipoPedido.h>

class ProcesoClientes : public ProcesoHijo {

private:

    std::vector<t_parametros_pedido> paramPedidosInternet;

    Pipe pipePtoVenta;

    int idCliente;

    void iniciarAtencion();


public:

    ProcesoClientes(Logger& logger, int idCliente,Pipe* pipePtoVenta, vector<t_parametros_pedido> paramPedidosInternet);

    ~ProcesoClientes() override;

    pid_t ejecutar() override;

    void enviar_pedido(t_parametros_pedido pedido,TipoPedido tipoPedido);
};


#endif //INC_7559_TP1_PROCESOCLIENTES_H
