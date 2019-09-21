#include "Ramo.h"

Ramo::Ramo(int id_productor): producido_por(id_productor) { }

Ramo::Ramo(int id_productor, TipoFlor f): producido_por(id_productor), tipoFlor(f) { }

Ramo::Ramo(std::string ramo_seralizado){
    std::string strIdProductor = ramo_seralizado.substr(0, 5);
    this->producido_por = std::stoi(Utils::trim(strIdProductor));
    std::string strTipoRamo = ramo_seralizado.substr(5, 20);
    this->tipoFlor = (TipoFlor) std::stoi(strTipoRamo);
    std::cout << "Deserializo un ramo del productor " << strIdProductor << std::endl;
}

int Ramo::get_productor() {
    return this->producido_por;
}

TipoFlor Ramo::getTipoFlor() {
    return this->tipoFlor;
}

//RAMO va a pesar 20 bytes. 5 bytes para el id y 15 para el tipo de flor
std::string Ramo::serializar() {

    std::cout << "Serializo un ramo del productor " << this->get_productor() << std::endl;

    std::stringstream serializado;
    serializado << std::setw(5) << this->get_productor() << std::setw(15) << this->getTipoFlor();
    return serializado.str();
}
   