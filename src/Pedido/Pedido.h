#ifndef PEDIDO_H_
#define PEDIDO_H_

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "../TipoPedido/TipoPedido.h"
#include "../Utils/Utils.h"

using std::string;

class Pedido {

    private:

        int id_cliente;
        int cantRosas;
        int cantTulipanes;
        TipoPedido tipoPedido;
    public:
        static const int TAM_ID_CLIENTE = 2;
        static const int TAM_CANT_ROSAS = 2;
        static const int TAM_CANT_TULIPANES = 2;
        static const int TAM_TIPO_PEDIDO = 1;
        static const int TAM_TOTAL = TAM_ID_CLIENTE + TAM_CANT_ROSAS + TAM_CANT_TULIPANES + TAM_TIPO_PEDIDO;

        Pedido(int id_cliente, parametros_pedido pedido );
        Pedido(std::string ramo_seralizado);

        int get_cliente();
        int get_tulipanes();
        int get_rosas();
        TipoPedido getTipoPedido();

        string serializar();

        string toString();
};

#endif /* RAMO_H_ */