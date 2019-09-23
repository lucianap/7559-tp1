#include "Pedido.h"
using std::string;

Pedido::Pedido(int id_cliente, parametros_pedido pedido ) :
        id_cliente(id_cliente),
        tipoPedido(pedido.origen),
        cantRosas(pedido.cantRosas),
        cantTulipanes(pedido.cantTulipanes) {}


Pedido::Pedido(string pedido_seralizado){
    std::string id_cliente = pedido_seralizado.substr(0, TAM_ID_CLIENTE);
    this->id_cliente = std::stoi(Utils::trim(id_cliente));
    std::string strCantRosas =
            pedido_seralizado.substr(TAM_ID_CLIENTE, TAM_CANT_ROSAS);
    std::string strCantTulipanes =
            pedido_seralizado.substr(TAM_ID_CLIENTE + TAM_CANT_ROSAS, TAM_CANT_TULIPANES);
    std::string strTipoPedido =
            pedido_seralizado.substr(TAM_TOTAL - TAM_TIPO_PEDIDO, TAM_TIPO_PEDIDO);
    this->cantRosas = std::stoi(strCantRosas);
    this->cantTulipanes = std::stoi(strCantTulipanes);
    this->tipoPedido = (TipoPedido) std::stoi(strTipoPedido);
}
int Pedido::get_cliente(){
    return this->id_cliente;
};
int Pedido::get_tulipanes(){
    return this->cantTulipanes;
};
int Pedido::get_rosas(){
    return this->cantRosas;
};
TipoPedido Pedido::getTipoPedido(){
    return this->tipoPedido;
};

/**
 * Pedido va a pesar TAM_ID_PRODUCTOR+TAM_TIPO_FLOR bytes.
 * TAM_ID_PRODUCTOR bytes para el id y TAM_TIPO_FLOR para el tipo de flor
 */
string Pedido::serializar() {
    std::stringstream serializado;
    serializado << std::setw(TAM_ID_CLIENTE) << this->get_cliente()
        << std::setw(TAM_CANT_TULIPANES) << this->get_tulipanes()
        << std::setw(TAM_CANT_ROSAS) << this->get_rosas()
        << std::setw(TAM_TIPO_PEDIDO) << this->getTipoPedido();
    return serializado.str();
}

string Pedido::toString() {
    std::stringstream toString;
    toString << "[idCliente="<<this->get_cliente()
        <<", Rosas="<< this->get_rosas()
        <<", Tulipanes="<< this->get_tulipanes()
        <<", Origen="<<Utils::getTextTipoPedido(this->tipoPedido)<<"]";
    return toString.str();
}
   