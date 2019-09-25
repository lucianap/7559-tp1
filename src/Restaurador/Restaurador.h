//
// Created by luciana on 9/24/19.
//

#ifndef INC_7559_TP1_RESTAURADOR_H
#define INC_7559_TP1_RESTAURADOR_H


#include <Productor/Productor.h>
#include <Distribuidor/Distribuidor.h>
#include <Guardador/Guardador.h>


class Restaurador {
private:
    std::vector<string> leerProcesoSerializado(std::string prefijo);
    std::multimap<int, int> restaurarAsignaciones(std::string asignaciones);
    std::string leerAsignacionesProductorDistribuidor();
    std::string leerAsignacionesDistribuidorPuntoVenta();
    std::string leerAsignaciones(std::string prefijo);

    map<int, Pipe*> distribuidoresEntradaByDistId;
    map<int, Pipe*> puntosDeVentaEntradaByPuntoVentaId;
    map<int, Pipe*> pipeVentaClientesByIdPVenta;

public:

    std::string getStatusSerializado();
    std::vector<Productor*> restaurarProductores(Logger& logger);
    std::vector<Distribuidor*> restaurarDistribuidores(Logger& logger);
    std::vector<PuntoVenta*> restaurarPuntosDeVenta(Logger &logger);
    std::vector<ProcesoClientes*> restaurarProcesosClientes(Logger &logger);

    void conectarPipes(std::vector<Productor*> productores,
                       std::vector<Distribuidor*> distribuidores,
                       std::vector<PuntoVenta*> vendedores,
                       std::vector<ProcesoClientes*> clientes,
                       Pipe pipeEntradaStatus);

};



#endif //INC_7559_TP1_RESTAURADOR_H
