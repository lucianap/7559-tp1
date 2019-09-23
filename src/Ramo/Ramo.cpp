#include "Ramo.h"
using std::string;

Ramo::Ramo(const Ramo & ramo ) :
        id(ramo.id), producido_por(ramo.producido_por), tipoFlor(ramo.tipoFlor) {}


Ramo::Ramo(int id_productor, TipoFlor f): producido_por(id_productor), tipoFlor(f) { }

Ramo::Ramo(string ramo_seralizado){
    std::string strIdProductor = ramo_seralizado.substr(0, TAM_ID_PRODUCTOR);
    this->producido_por = std::stoi(Utils::trim(strIdProductor));
    std::string strTipoRamo =
            ramo_seralizado.substr(TAM_ID_PRODUCTOR, TAM_TIPO_FLOR);
    this->tipoFlor = (TipoFlor) std::stoi(strTipoRamo);
}

int Ramo::get_productor() {
    return this->producido_por;
}

TipoFlor Ramo::getTipoFlor() {
    return this->tipoFlor;
}

/**
 * RAMO va a pesar TAM_ID_PRODUCTOR+TAM_TIPO_FLOR bytes.
 * TAM_ID_PRODUCTOR bytes para el id y TAM_TIPO_FLOR para el tipo de flor
 */
string Ramo::serializar() {
    std::stringstream serializado;
    serializado << std::setw(TAM_ID_PRODUCTOR) << this->get_productor() << std::setw(TAM_TIPO_FLOR) << this->getTipoFlor();
    return serializado.str();
}

string Ramo::toString() {
    std::stringstream toString;
    toString << "[idProductor="<<this->get_productor()<<", TipoFlor="<<Utils::getTextTipoFlor(this->getTipoFlor())<<"]";
    return toString.str();
}
   