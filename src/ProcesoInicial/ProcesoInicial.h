//
// Created by nestor on 18/09/19.
//

#ifndef INC_7559_TP1_PROCESOINICIAL_H
#define INC_7559_TP1_PROCESOINICIAL_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <map>
#include <Utils/Utils.h>
#include <Distribuidor/Distribuidor.h>
#include <Productor/Productor.h>

#include <ProcesoClientes/ProcesoClientes.h>
#include "Productor/Productor.h"
#include "Distribuidor/Distribuidor.h"
#include "PuntoVenta/PuntoVenta.h"
#include "Menu/Menu.h"
#include "ProcesoInicial/ProcesoInicial.h"
#include "Logger/LoggerProcess.h"
#include "Logger/Logger.h"
using std::vector;


class ProcesoInicial {

private:

    t_parametros parametros;
    vector<Distribuidor*> distribuidores;
    vector<Productor*> productores;
    vector<PuntoVenta*> puntosVenta;
    vector<ProcesoClientes*> procesosClientes;
    vector<Pipe *> distribuidoresEntrada;
    vector<Pipe *> pVentasEntrada;
    void limpiar();
    void terminarProcesos();
    void limpiarMemoria();

    Logger logger;
    LoggerProcess loggerProcess;

    std::multimap<int,int> asignacionesProductorDistribuidores;
    std::multimap<int,int> asignacionesDistribuidorPuntosDeVenta;


public:
    ProcesoInicial(t_parametros parametros);
    ~ProcesoInicial();

    void iniciarEjecucion();

    int asignar_pipes(const int j, Pipe* pipeInDistribuidor, const int cantidad_productores,
                            std::map<int, vector<Pipe*>>* distribuidores_por_productor );

    void guardar();

    void reanudarEjecucion();

};


#endif //INC_7559_TP1_PROCESOINICIAL_H
