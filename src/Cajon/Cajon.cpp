#include "Cajon.h"

#include <utility>
#include "Utils/Utils.h"


Cajon::Cajon() {

}

Cajon::Cajon(const Cajon & cajon): id(cajon.id), ramos(std::move(cajon.ramos))  {}


Cajon::Cajon(std::vector<Ramo> ramos): ramos(std::move(ramos)) {}

Cajon::Cajon(const std::string& cajon_serializado, int ramos_por_cajon) {
    for(int i = 0; i < ramos_por_cajon; i++) {
        std::string strRamo = cajon_serializado.substr(i*Ramo::TAM_TOTAL,Ramo::TAM_TOTAL);
        Ramo ramo(strRamo);
        this->ramos.push_back(ramo);
    }
}

//lo que hace es serializar un cajón, concatenando las serializaciones de sus ramos,
//para deserializarlo habría que hacer split por el separador y deserializar cada ramo.
std::string Cajon::serializar(){

    std::vector<std::string> string_vector;

    for (auto it = this->ramos.begin() ; it != this->ramos.end(); ++it) {
        Ramo r = *it;
        string_vector.push_back(r.serializar());
    }

    std::string cajon_serializado;
    Utils::join(string_vector, cajon_serializado);
    return cajon_serializado;
}

vector<Ramo> Cajon::filtrar(TipoFlor tipoFlor) {
    vector<Ramo> filtrado;

    for (auto & ramo : this->ramos) {
        if (ramo.getTipoFlor() == tipoFlor) {
            filtrado.push_back(ramo);
        }
    }
    return filtrado;
}

Cajon::~Cajon() {
    ramos.clear();
}

bool Cajon::estaVacio() {
    return this->ramos.empty();
}

