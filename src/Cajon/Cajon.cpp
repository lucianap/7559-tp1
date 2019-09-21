#include "Cajon.h"

#include <utility>
#include "Utils/Utils.h"

const char SEPARADOR_RAMOS = '|';

Cajon::Cajon(std::vector<Ramo*> ramos): ramos(std::move(ramos)) {}

Cajon::Cajon(const std::string& cajon_serializado, int ramos_por_cajon) {
    for(int i = 0; i < ramos_por_cajon; i++) {
        std::string strRamo = cajon_serializado.substr(0,20);
        Ramo *r = new Ramo(strRamo);
        this->ramos.push_back(r);
    }
}

//lo que hace es serializar un cajón, concatenando las serializaciones de sus ramos,
//para deserializarlo habría que hacer split por el separador y deserializar cada ramo.
std::string Cajon::serializar(){

    std::vector<std::string> string_vector;

    for (auto it = this->ramos.begin() ; it != this->ramos.end(); ++it) {
        Ramo* r = *it;
        string_vector.push_back(r->serializar());
    }

    std::string cajon_serializado;
    Utils::join(string_vector, cajon_serializado);
    return cajon_serializado;
}

Cajon::~Cajon() {
    std::cout << "Se descarta el cajón." << std::endl;
    for (auto r : ramos){
        delete r;
    }
    ramos.clear();
}
