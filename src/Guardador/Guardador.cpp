//
// Created by luciana on 9/22/19.
//

#include <cstring>
#include <sys/stat.h>
#include "Guardador.h"

const std::string Guardador::carpeta = "data";
const std::string Guardador::prefijoProductores = "productor_pid_";
const std::string Guardador::prefijoDistribuidores = "distribuidor_pid_";
const std::string Guardador::prefijoVendedores = "vendedor_pid_";
const std::string Guardador::archivoAsignaciones = "asignaciones";

const std::string Guardador::separadorAsignaciones = "|";
const std::string Guardador::simboloAsignadoA = ">";
const std::string Guardador::prefijoClientes = "Clientes_pid_";


const std::string Guardador::prefijoAsignacionesProductorDistribuidor = "distribuidores_por_productor:";
const std::string Guardador::prefijoAsignacionesDistribuidorPuntoDeVenta = "pvta_por_productor:";


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
    ss << Guardador::carpeta << "/" << Guardador::prefijoProductores << getpid();
    guardar(proceso, ss.str());

}

void Guardador::guardar(Distribuidor *proceso) {
    std::stringstream ss;
    ss << Guardador::carpeta << "/" << Guardador::prefijoDistribuidores << getpid();
    guardar(proceso, ss.str());

}

void Guardador::guardar_cliente(ProcesoClientes *proceso) {
    std::stringstream ss;
    ss << Guardador::carpeta << "/" << Guardador::prefijoClientes << getpid();
    guardar(proceso, ss.str());

}

void Guardador::guardar_ptoVenta(PuntoVenta *proceso) {
    std::stringstream ss;
    ss << Guardador::carpeta << "/" << Guardador::prefijoVendedores << getpid();
    guardar(proceso, ss.str());

}

Guardador::Guardador() {}

bool Guardador::isCantidadDeArchivosGuardadosOk(int cantidadEsperada) {
    return (Utils::countFiles(Guardador::carpeta) == cantidadEsperada);
}


void Guardador::guardarAsignaciones(std::multimap<int, int> mapaDistribuidoresPorProductor,
                                    std::multimap<int, int> mapaPvtaPorDistribuidor ) {
    stringstream ss;
    ss << Guardador::carpeta << "/" << Guardador::archivoAsignaciones;
    std::ofstream out(ss.str());
    out << Guardador::prefijoAsignacionesProductorDistribuidor;
    out << formatearAsignacion(mapaDistribuidoresPorProductor) << endl;

    out << Guardador::prefijoAsignacionesDistribuidorPuntoDeVenta;
    out << formatearAsignacion(mapaPvtaPorDistribuidor) << endl;
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







