
#include <Guardador/Guardador.h>
#include <Restaurador/Restaurador.h>
#include <FileManager/FileManager.h>
#include <FDRegistry/FDRegistry.h>
#include "ProcesoInicial.h"
#include <Remito/Remito.h>


using std::vector;
const string ProcesoInicial::RUTA_ARCHIVO_PEDIDOS = "pedidos.csv";

ProcesoInicial::ProcesoInicial(t_parametros parametros):
        parametros(parametros), logger(true), loggerProcess("log.txt", logger), status(logger) {

    FDRegistry::AddFileDescriptor(logger.getPipe().getFdLectura());
    FDRegistry::AddFileDescriptor(logger.getPipe().getFdEscritura());
    FDRegistry::AddFileDescriptor(status.getPipeEntrada().getFdEscritura());
    FDRegistry::AddFileDescriptor(status.getPipeSalida().getFdEscritura());

}

void ProcesoInicial::reanudarEjecucion() {
    Restaurador r;
    this->productores = r.restaurarProductores(logger);
    this->distribuidores = r.restaurarDistribuidores(logger);
    this->puntosVenta = r.restaurarPuntosDeVenta(logger);
    this->procesosClientes = r.restaurarProcesosClientes(logger);

    r.conectarPipes(this->productores, this->distribuidores,
            this->puntosVenta, this->procesosClientes, this->status.getPipeEntrada());

    this->asignacionesDistribuidorPuntosDeVenta = r.getAsignacionesDistribuidorPuntoVenta();
    this->asignacionesProductorDistribuidores = r.getAsignacionesProductorDistribuidor();

    loggerProcess.ejecutar();
    logger.log("-----------Restaurando sistema-------------");

    this->status.setCantPipes(puntosVenta.size());

    Guardador::truncar();
    Guardador::inicializar();

    //damos play a todos los procesos.
    vector<Productor*> pAsignados;
    for(auto it = this->productores.begin(); it != this->productores.end(); it++) {
        if((*it)->getCantidadDistribuidoresAsignados() >= 1){
            (*it)->ejecutar();
            pAsignados.push_back(*it);
        }
    }
    this->productores = pAsignados;

    for(auto it = this->distribuidores.begin(); it != this->distribuidores.end(); it++) {
        (*it)->ejecutar();
    }

    for(auto it = this->puntosVenta.begin(); it != this->puntosVenta.end(); it++) {
        (*it)->ejecutar();
    }

    for(auto it = this->procesosClientes.begin(); it != this->procesosClientes.end(); it++) {
        (*it)->ejecutar();
    }

    this->iniciarMenu();

}

void ProcesoInicial::iniciarEjecucion() {

    int productores = parametros.cantProductores;
    int distribuidores = parametros.cantDistribuidores;
    int puntos_de_venta = parametros.cantPuntosVenta;
    this->status.setCantPipes(puntos_de_venta);
    //todo leer archivo de pedidos y cargarlo a partir del mismo


    if(puntos_de_venta < distribuidores) {
        distribuidores = puntos_de_venta;
    }

    if(distribuidores < productores) {
        productores = distribuidores;
    }

    Guardador::truncar();
    Guardador::inicializar();
    Remito::cleanUp();
    Remito::inicializar();
    int ramos_por_cajon = 10;

    /***** inicializamos los pipes para todo el sistema *****/

    //Mapa de asignación de productores a distribuidores.
    //La key es el número del Productor, el valor es el vector de Pipes que tienen su salida en los distintos distribuidores.
    std::map<int, vector<Pipe*>> distribuidores_por_productor;
    for (int j = 0; j < distribuidores; ++j) {
        Pipe* pipeInDistribuidor1 = new Pipe();
        this->distribuidoresEntrada.push_back(pipeInDistribuidor1);
        int productorAsignado = this->asignar_pipes(j, pipeInDistribuidor1, productores, &distribuidores_por_productor);
        this->asignacionesProductorDistribuidores.insert(std::pair<int,int>(productorAsignado, j));
    }
    std::map<int, vector<Pipe*>> p_ventas_por_distribuidor;
    for (int j = 0; j < puntos_de_venta; ++j) {
        Pipe* pipeInPVenta = new Pipe();
        this->pVentasEntrada.push_back(pipeInPVenta);
        int distribuidorAsignado = this->asignar_pipes(j, pipeInPVenta, distribuidores, &p_ventas_por_distribuidor);
        this->asignacionesDistribuidorPuntosDeVenta.insert(std::pair<int,int>(distribuidorAsignado, j));
    }

    /**** hasta este punto se deben inicializar todos los pipes ****/
    loggerProcess.ejecutar();
    logger.log("-----------Iniciando sistema-------------");

    //Creación de los productores en procesos separados..
    for(int i = 0; i < productores; i++) {
        if(distribuidores_por_productor.find(i) == distribuidores_por_productor.end()) {
            logger.log("No hay distribuidores para el productor " + to_string(i));
        } else {
            std::vector<Pipe*> distribuidores_escuchando = distribuidores_por_productor.at(i);
            Productor* p = new Productor(i, distribuidores_escuchando, ramos_por_cajon, logger);
            this->productores.push_back(p);
            p->ejecutar();
        }
    }

    for (int j = 0; j < distribuidores; ++j) {
        if(p_ventas_por_distribuidor.find(j) == p_ventas_por_distribuidor.end()) {
            logger.log("No hay ptos de venta para el distribuidor " + to_string(j));
        } else {
            std::vector<Pipe *> pts_de_venta_escuchando = p_ventas_por_distribuidor.at(j);
            Distribuidor *distribuidor = new Distribuidor(logger, pts_de_venta_escuchando, j,
                                                          this->distribuidoresEntrada.at(j));
            this->distribuidores.push_back(distribuidor);
            distribuidor->ejecutar();
        }
    }

    FileManager file_manager(RUTA_ARCHIVO_PEDIDOS,logger);
    vector<t_parametros_pedido>config_pedidos_internet ;
    file_manager.ReadFile(config_pedidos_internet);

    for (int j = 0; j < puntos_de_venta; ++j) {
        ProcesoClientes* procesoClientes = new ProcesoClientes(logger,j, this->pVentasEntrada.at(j),config_pedidos_internet);
        PuntoVenta* pto_venta = new PuntoVenta(logger, j, status.getPipeEntrada(), this->pVentasEntrada.at(j));
        this->puntosVenta.push_back(pto_venta);
        this->procesosClientes.push_back(procesoClientes);
        procesoClientes->ejecutar();
        pto_venta->ejecutar();
    }


    this->iniciarMenu();
}



ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiarMemoria();
}

int ProcesoInicial::asignar_pipes(const int j, Pipe* pipeIn, const int cantidad_a_asignar,
        std::map<int, vector<Pipe*>>* pipe_map ) {

    //Se asigna el Distribuidor j a un Productor N y se guarda el resultado en el mapa distribuidores_por Productor.
    int n = j % cantidad_a_asignar;

    if(pipe_map->find(n) == pipe_map->end()) {
        std::vector<Pipe*> pipe_vector{pipeIn};
        pipe_map->insert(std::make_pair(n, pipe_vector));
    } else {
        std::vector<Pipe*> pipe_vector = pipe_map->at(n);
        pipe_vector.push_back(pipeIn);
        pipe_map->erase(n);
        pipe_map->insert(std::make_pair(n, pipe_vector));
    }

    return n;

}

void ProcesoInicial::guardar() {

    for(auto it = productores.begin(); it != productores.end(); it++) {
        (*it)->guardar();
    }

    for(auto it = procesosClientes.begin(); it != procesosClientes.end(); it++) {
        (*it)->guardar();
    }

    //Controlo que todos los procesos se hayan guardado.
    while(!Guardador::isCantidadDeArchivosGuardadosOk(distribuidores.size() + productores.size()
                + puntosVenta.size() + procesosClientes.size())){}

    Guardador g;
    g.guardarAsignaciones(this->asignacionesProductorDistribuidores , this->asignacionesDistribuidorPuntosDeVenta);
    g.guardar(&status);
    this->terminarProcesos();

}



void ProcesoInicial::terminarProcesos() {

    for (int i = 0; i < this->productores.size(); ++i) {
        ProcesoHijo* proceso = this->productores.at(i);
        proceso->terminar();
    }

    for (int i = 0; i < this->distribuidores.size(); ++i) {
        ProcesoHijo* proceso = this->distribuidores.at(i);
        proceso->terminar();
    }

    for (int i = 0; i < this->puntosVenta.size(); ++i) {
        ProcesoHijo* proceso = this->puntosVenta.at(i);
        proceso->terminar();
    }

    for (int i = 0; i < this->procesosClientes.size(); ++i) {
        ProcesoHijo* proceso = this->procesosClientes.at(i);
        proceso->terminar();
    }
    this->status.terminar();
    this->loggerProcess.terminar(); // tiene que ser el ultimo siempre

}

void ProcesoInicial::limpiarMemoria() {

    for (int j = 0; j < productores.size(); ++j) {
        delete(productores.at(j));
    }

    for (int j = 0; j < distribuidores.size(); ++j) {
        delete(distribuidores.at(j));
    }

    for (int j = 0; j < puntosVenta.size(); ++j) {
        delete(puntosVenta.at(j));
    }

    for (int j = 0; j < procesosClientes.size(); ++j) {
        delete(procesosClientes.at(j));
    }

}

void ProcesoInicial::iniciarMenu() {
    status.ejecutar();
    Menu menu(status, logger);
    int status = menu.iniciar();
    if (status == 1) {
        this->guardar();
    } else if (status == 0) {
        this->terminarProcesos();
    }
}
