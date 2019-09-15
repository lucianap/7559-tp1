#include "Cajon.h"
#include "Utils/Utils.h"

const char SEPARADOR_RAMOS = '|';

Cajon::Cajon(std::vector<Ramo> ramos): ramos(ramos) {}

Cajon::Cajon(std::string cajon_serializado) {
    //TODO
}

//lo que hace es serializar un cajón, concatenando las serializaciones de sus ramos,
//para deserializarlo habría que hacer split por el separador y deserializar cada ramo.
std::string Cajon::serializar(){
    std::vector<std::string> string_vector;
    for (std::vector<Ramo>::iterator it = this->ramos.begin() ; it != this->ramos.end(); ++it) {
        Ramo r = *it;
        string_vector.push_back(r.serializar());
    }
    std::string cajon_serializado;
    Utils::join(string_vector, SEPARADOR_RAMOS, cajon_serializado);
    return cajon_serializado;
}