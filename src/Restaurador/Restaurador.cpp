//
// Created by luciana on 9/24/19.
//

#include <Guardador/Guardador.h>
#include <FileManager/FileManager.h>
#include "Restaurador.h"

std::string Restaurador::getStatusSerializado() {
    vector<string> v = leerProcesoSerializado(Guardador::archivoStatus);
    return v.at(0);
}


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

std::vector<PuntoVenta *> Restaurador::restaurarPuntosDeVenta(Logger &logger) {
    vector<PuntoVenta*> procesosRestaurados;
    vector<string> procesos = this->leerProcesoSerializado(Guardador::prefijoVendedores);
    for(auto it = procesos.begin(); it != procesos.end(); it++) {
        Pipe* entrada = new Pipe();
        auto* restaurado = new PuntoVenta(logger, (*it), entrada);

        puntosDeVentaEntradaByPuntoVentaId.insert(std::pair<int, Pipe*>(restaurado->getId(), entrada));
        procesosRestaurados.push_back(restaurado);
    }

    return procesosRestaurados;
}

void Restaurador::conectarPipes(std::vector<Productor *> productores,
        std::vector<Distribuidor *> distribuidores,
        std::vector<PuntoVenta*> puntosVentas,
        std::vector<ProcesoClientes*> clientes, Pipe pipeEntradaStatus) {

    //Recupero todas las asignaciones Productor>Distribuidor.
    this->asignacionesProductorDistribuidores =
            this->restaurarAsignaciones(leerAsignacionesProductorDistribuidor());

    for(auto it = asignacionesProductorDistribuidores.begin(); it != asignacionesProductorDistribuidores.end(); it++) {
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

    //Recupero asignaciones Distribuidor>PuntoVenta
    this->asignacionesDistribuidorPuntosDeVenta =
            this->restaurarAsignaciones(leerAsignacionesDistribuidorPuntoVenta());

    for(auto it = asignacionesDistribuidorPuntosDeVenta.begin(); it != asignacionesDistribuidorPuntosDeVenta.end(); it++) {
        int idDistribuidor = it->first;
        int idPtoVenta = it->second;
        for(auto itD = distribuidores.begin(); itD != distribuidores.end(); itD++) {
            Distribuidor *d = *itD;
            if(d->getId() == idDistribuidor) {
                d->agregarPuntoVenta(puntosDeVentaEntradaByPuntoVentaId
                                               .find(idPtoVenta)->second);
            }
        }
    }

    //Asigno punto de venta con clientes.
    for(auto itVenta = puntosVentas.begin(); itVenta != puntosVentas.end(); itVenta++) {

        (*itVenta)->asignarPipeStatus(pipeEntradaStatus);

        for(auto itClientes = clientes.begin(); itClientes != clientes.end(); itClientes++) {
            if((*itVenta)->getId() == (*itClientes)->getId()) {
                (*itClientes)->asignarPtoVenta(puntosDeVentaEntradaByPuntoVentaId.find((*itVenta)->getId())->second);
            }
        }
    }

}

std::string Restaurador::leerAsignacionesDistribuidorPuntoVenta() {
    return this->leerAsignaciones(Guardador::prefijoAsignacionesDistribuidorPuntoDeVenta);
}


std::string Restaurador::leerAsignacionesProductorDistribuidor() {
    return this->leerAsignaciones(Guardador::prefijoAsignacionesProductorDistribuidor);
}

//TODO usar RAAI
std::string Restaurador::leerAsignaciones(std::string prefijo) {
    ifstream inFile;
    std::string retorno;
    inFile.open(Guardador::carpeta + "/" + Guardador::archivoAsignaciones);

    if (!inFile) {
        cout << "No se puede abrir archivo.";
        exit(1);
    }
    std::string line;
    while (std::getline(inFile, line)) {
        if(Utils::startsWith(prefijo.c_str(),
                             line.c_str())){
            //le saco el prefijo
            retorno = line.substr(prefijo.size(), line.size());
        }
    }

    inFile.close();
    return retorno;
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

std::vector<ProcesoClientes *> Restaurador::restaurarProcesosClientes(Logger &logger) {
    vector<std::string> clientes = leerProcesoSerializado(Guardador::prefijoClientes);
    vector<ProcesoClientes*> pcs;
    for(auto it = clientes.begin(); it != clientes.end(); it++) {
        ProcesoClientes* pc = new ProcesoClientes(logger, *it);
        pcs.push_back(pc);
    }
    return pcs;
}

std::multimap<int, int> Restaurador::getAsignacionesProductorDistribuidor() {
    return this->asignacionesProductorDistribuidores;
}

std::multimap<int, int> Restaurador::getAsignacionesDistribuidorPuntoVenta() {
    return this->asignacionesDistribuidorPuntosDeVenta;
}


