//
// Created by luciana on 9/22/19.
//

#include <cstring>
#include "Guardador.h"

const std::string Guardador::carpeta = "data";
const std::string Guardador::prefijoProductores = "data/productor_pid_";
const std::string Guardador::prefijoDistribuidores = "data/distribuidor_pid_";
const std::string Guardador::prefijoVendedores = "data/vendedor_pid_";

void Guardador::inicializar(){
    int status = mkdir(Guardador::carpeta.c_str(), 0777);
    //TODO manejar errores.
}

void Guardador::cleanUp() {
    int status = Utils::remove_directory(Guardador::carpeta.c_str());
    //TODO manejar errores.8
}

void Guardador::guardar(ProcesoHijo *proceso, std::string prefijo) {
    std::ofstream out(prefijo);
    out << proceso->serializar();
    out.close();
}


void Guardador::guardar(Productor *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoProductores << getpid();
    guardar(proceso, ss.str());

}

void Guardador::guardar(Distribuidor *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoDistribuidores << getpid();
    guardar(proceso, ss.str());

}

Guardador::Guardador() {}

bool Guardador::isCantidadDeArchivosGuardadosOk(int cantidadEsperada) {
    return (Utils::countFiles(Guardador::carpeta) == cantidadEsperada);
}




