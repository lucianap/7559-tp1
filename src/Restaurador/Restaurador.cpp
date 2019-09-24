//
// Created by luciana on 9/24/19.
//

#include <Guardador/Guardador.h>
#include "Restaurador.h"

std::vector<Productor *> Restaurador::restaurarProductores(Logger& logger) {
    vector<Productor*> procesosRestaurados;
    vector<string> procesos = this->leerProcesoSerializado(Guardador::prefijoProductores);
    for(auto it = procesos.begin(); it != procesos.end(); it++) {
        auto* restaurado = new Productor(logger, (*it));
        procesosRestaurados.push_back(restaurado);
    }
    return procesosRestaurados;
}

std::vector<Distribuidor *> Restaurador::restaurarDistribuidores(Logger &logger) {
    vector<Distribuidor*> procesosRestaurados;
    vector<string> procesos = this->leerProcesoSerializado(Guardador::prefijoDistribuidores);
    for(auto it = procesos.begin(); it != procesos.end(); it++) {
        Pipe* entrada = new Pipe();
        auto* restaurado = new Distribuidor(logger, (*it), entrada);
        distribuidoresEntradaByDistId.insert(std::pair<int, Pipe*>(restaurado->getId(), entrada));
        procesosRestaurados.push_back(restaurado);
    }
    return procesosRestaurados;
}

void Restaurador::conectarPipes(std::vector<Productor *> productores,
        std::vector<Distribuidor *> distribuidores) {
    //Recupero todas las asignaciones Productor>Distribuidor.
    multimap<int, int> asignacionesProdDist =
            this->restaurarAsignaciones(leerAsignacionesProductorDistribuidor());

    for(auto it = asignacionesProdDist.begin(); it != asignacionesProdDist.end(); it++) {
        int idProductor = it->first;
        int idDistribuidor = it->second;
        for(auto itP = productores.begin(); itP != productores.end(); itP++) {
            Productor *p = *itP;
            if(p->getId() == idProductor) {
                p->agregarDistribuidor(distribuidoresEntradaByDistId
                .find(idDistribuidor)->second);
            }
        }
    }
}

std::string Restaurador::leerAsignacionesProductorDistribuidor() {
    ifstream inFile;
    inFile.open(Guardador::carpeta + "/" + Guardador::archivoAsignaciones);

    if (!inFile) {
        cout << "No se puede abrir archivo.";
        exit(1);
    }
    std::string line;
    while (std::getline(inFile, line)) {
        if(Utils::startsWith(Guardador::prefijoAsignacionesProductorDistribuidor.c_str(),
                line.c_str())){
            //le saco el prefijo
            return line.substr(Guardador::prefijoAsignacionesProductorDistribuidor.size(), line.size());
        }
    }
}

std::multimap<int, int> Restaurador::restaurarAsignaciones(std::string asignaciones) {
    std::multimap<int, int> m;
    vector<string> asignacionesVector = Utils::split(asignaciones , Guardador::separadorAsignaciones);
    for(auto it = asignacionesVector.begin(); it != asignacionesVector.end(); it++){
        vector<string> par = Utils::split((*it), Guardador::simboloAsignadoA);
        std::pair<int, int> asignacion(stoi(par[0]), stoi(par[1]));
        m.insert(asignacion);
    }

    return m;
}

std::vector<string> Restaurador::leerProcesoSerializado(std::string prefijo) {
    vector<string> procesosSerializados;
    vector<string> files = Utils::listarArchivosConPrefijo(Guardador::carpeta.c_str(), prefijo);
    for(auto it = files.begin(); it != files.end(); it++) {
        ifstream inFile;
        inFile.open(Guardador::carpeta + "/" + (*it));
        if (!inFile) {
            cout << "No se puede abrir archivo.";
            exit(1); // terminate with error
        }

        std::string line;
        while (std::getline(inFile, line)) {
            procesosSerializados.push_back(line);
        }

        inFile.close();
    }

    return procesosSerializados;

}

