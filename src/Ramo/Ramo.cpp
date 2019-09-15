#include "Ramo.h"


Ramo::Ramo(int id_productor): producido_por(id_productor) { }

Ramo::Ramo(std::string ramo_seralizado){

    //TODO leer de un ramo serializado.
    this->producido_por = std::stoi(ramo_seralizado);
    
}

int Ramo::get_productor() {
    return this->producido_por;
}


//TODO charlar con los chicos de cómo serían las serializaciones.
std::string Ramo::serializar() {
    return std::to_string(this->producido_por);
}
   