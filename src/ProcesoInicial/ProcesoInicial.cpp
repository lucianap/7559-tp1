
#include "ProcesoInicial.h"


using std::vector;


ProcesoInicial::ProcesoInicial(t_parametros parametros):
        parametros(parametros), logger(true), loggerProcess("log.txt", logger) {
}

void ProcesoInicial::iniciarEjecucion() {

    int productores = parametros.cantProductores;
    int distribuidores = parametros.cantDistribuidores;
    int puntos_de_venta = parametros.cantPuntosVenta;

    //todo leer archivo de pedidos y cargarlo a partir del mismo
    std::vector<t_parametros_pedido> config_pedidos_internet;
    for (int j = 0; j < 10; ++j) {
        t_parametros_pedido pedido_internet1;
        pedido_internet1.cantTulipanes = j+1;
        pedido_internet1.cantRosas = j+1;
        config_pedidos_internet.push_back(pedido_internet1);
    }


    int ramos_por_cajon = 10;
    int ramos_por_envio = 100;

    /***** inicializamos los pipes para todo el sistema *****/

    //Mapa de asignación de productores a distribuidores.
    //La key es el número del productor, el valor es el vector de Pipes que tienen su salida en los distintos distribuidores.
    std::map<int, vector<Pipe*>> distribuidores_por_productor;
    for (int j = 0; j < distribuidores; ++j) {
        Pipe* pipeInDistribuidor1 = new Pipe();
        this->distribuidoresEntrada.push_back(pipeInDistribuidor1);
        this->asignar_pipes(j, pipeInDistribuidor1, productores, &distribuidores_por_productor);
    }
    std::map<int, vector<Pipe*>> p_ventas_por_distribuidor;
    for (int j = 0; j < puntos_de_venta; ++j) {
        Pipe* pipeClientes = new Pipe();
        this->pVentasEntradaClientes.push_back(pipeClientes);
        Pipe* pipeInPVenta = new Pipe();
        this->pVentasEntrada.push_back(pipeInPVenta);
        this->asignar_pipes(j, pipeInPVenta, puntos_de_venta, &p_ventas_por_distribuidor);
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
        std::vector<Pipe*> pts_de_venta_escuchando = p_ventas_por_distribuidor.at(j);
        Distribuidor* distribuidor = new Distribuidor(logger, pts_de_venta_escuchando, j, this->distribuidoresEntrada.at(j));
        this->distribuidores.push_back(distribuidor);
        distribuidor->ejecutar();
    }

    for (int j = 0; j < puntos_de_venta; ++j) {
        ProcesoClientes* procesoClientes = new ProcesoClientes(logger,this->pVentasEntradaClientes.at(j),config_pedidos_internet);
        PuntoVenta* pto_venta = new PuntoVenta(logger, j, this->pVentasEntrada.at(j), this->pVentasEntradaClientes.at(j));
        this->puntosVenta.push_back(pto_venta);
        this->procesosClientes.push_back(procesoClientes);
        procesoClientes->ejecutar();
        pto_venta->ejecutar();
    }
    Menu menu;
    menu.iniciar();

}

ProcesoInicial::~ProcesoInicial() {
    // liberar recursos de memoria e ipc
    this->limpiar();
}

void ProcesoInicial::asignar_pipes(const int j, Pipe* pipeIn, const int cantidad_a_asignar,
        std::map<int, vector<Pipe*>>* pipe_map ) {

    //Se asigna el distribuidor j a un productor N y se guarda el resultado en el mapa distribuidores_por productor.
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

}

void ProcesoInicial::limpiar() {

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

    for (int j = 0; j < productores.size(); ++j) {
        delete(productores.at(j));
    }

    for (int j = 0; j < distribuidores.size(); ++j) {
        delete(distribuidores.at(j));
    }

    for (int j = 0; j < puntosVenta.size(); ++j) {
        delete(puntosVenta.at(j));
    }

    this->loggerProcess.terminar(); // tiene que ser el ultimo siempre

}

