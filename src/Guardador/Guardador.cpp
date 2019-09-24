//
// Created by luciana on 9/22/19.
//

#include <cstring>
#include "Guardador.h"

const std::string Guardador::carpeta = "data";
const std::string Guardador::prefijoProductores = "data/productor_pid_";
const std::string Guardador::prefijoDistribuidores = "data/distribuidor_pid_";
const std::string Guardador::prefijoVendedores = "data/vendedor_pid_";
const std::string Guardador::archivoAsignaciones = "data/asignaciones";

const std::string Guardador::separadorAsignaciones = "|";
const std::string Guardador::simboloAsignadoA = ">";
const std::string Guardador::prefijoClientes = "data/Clientes_pid_";

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

void Guardador::guardar_cliente(ProcesoClientes *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoClientes << getpid();
    guardar(proceso, ss.str());

}

void Guardador::guardar_ptoVenta(PuntoVenta *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoVendedores << getpid();
    guardar(proceso, ss.str());

}



Guardador::Guardador() {}

bool Guardador::isCantidadDeArchivosGuardadosOk(int cantidadEsperada) {
    return (Utils::countFiles(Guardador::carpeta) == cantidadEsperada);
}


void Guardador::guardarAsignaciones(std::multimap<int, int> distribuidores_por_productor) {
    std::ofstream out(Guardador::archivoAsignaciones);
    out << "distribuidores_por_productor:";
    out << formatearAsignacion(distribuidores_por_productor);
    out.close();
}

/**
 * Formatea una asignación de tipo x1 -> [n1,n2,...], x2 -> [p1,p2,...] ... xn -> [z1,z2,..] a string.
 * @param mapaDeAsignaciones
 * @return Ejemplo: 1>1|1>2|1>3|2>4|
 */
std::string Guardador::formatearAsignacion(std::multimap<int, int> mapaDeAsignaciones) {
    stringstream ss;
    for(auto it = mapaDeAsignaciones.begin(); it != mapaDeAsignaciones.end(); it++) {
        int izquierdo = it->first;
        int derecho = it->second;

        ss << izquierdo << Guardador::simboloAsignadoA << derecho <<separadorAsignaciones; // |
    }
    return ss.str();
}

std::multimap<int, int> Guardador::restaurarAsignaciones(std::string asignaciones) {
    std::multimap<int, int> m;
    vector<string> asignacionesVector = Utils::split(asignaciones , Guardador::separadorAsignaciones);
    for(auto it = asignacionesVector.begin(); it != asignacionesVector.end(); it++){
        vector<string> par = Utils::split((*it), Guardador::simboloAsignadoA);
        std::pair<int, int> asignacion(stoi(par[0]), stoi(par[1]));
        m.insert(asignacion);
    }

    return m;
}






