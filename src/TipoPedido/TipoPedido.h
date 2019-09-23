//
// Created by luciana on 9/22/19.
//

#ifndef INC_7559_TP1_TIPOPEDIDO_H
#define INC_7559_TP1_TIPOPEDIDO_H

enum TipoPedido {
    INTERNET,
    LOCAL
};

typedef struct parametros_pedido{
    int cantTulipanes;
    int cantRosas;
    TipoPedido origen;
} t_parametros_pedido;





#endif //INC_7559_TP1_TIPOPROCESO_H
