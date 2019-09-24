//
// Created by Nacho on 23/9/19.
//


#include <TipoPedido/TipoPedido.h>
#include <sstream>

#include "FileManager.h"

FileManager::FileManager(string fileName, Logger& logger) :
    fileName(fileName), logger(logger){}

std::vector<t_parametros_pedido>  FileManager::ReadFile() {

    inFile.open(fileName);

    std::vector<t_parametros_pedido> config_pedidos_internet;
    if(!inFile.is_open()){
        logger.log("Error al abrir el archivo.");
        logger.log("Cargando pedido default.");
        return this->get_default();
    }

    string cant_rosas;
    string cant_tulipanes;
    bool es_header = true;
    try{
        while(inFile.good()){
            getline(inFile,cant_tulipanes,',');
            getline(inFile,cant_rosas,'\n');
            std::stringstream ss;
            ss<<"PEDIDO ";
            ss<<"Tulipanes: "<<cant_tulipanes;
            ss<<" Rosas: "<<cant_rosas;
            ss<<endl;
            logger.log(ss.str());
            if(es_header){es_header = false;continue;}
            t_parametros_pedido pedido_internet1;
            pedido_internet1.cantTulipanes = std::stoi(cant_tulipanes);
            pedido_internet1.cantRosas = std::stoi(cant_rosas);
            pedido_internet1.origen = INTERNET;
            config_pedidos_internet.push_back(pedido_internet1);

        }
    }catch (std::string &error){
        logger.log("Error al cargar el archivo.");
        logger.log("Cargando pedido default.");
        config_pedidos_internet = this->get_default();
    }
    inFile.close();
    return config_pedidos_internet;
}

std::vector<t_parametros_pedido>  FileManager::get_default(){

    std::vector<t_parametros_pedido> config_pedidos_internet;
    for (int j = 0; j < 10; ++j) {
        t_parametros_pedido pedido_internet1;
        pedido_internet1.cantTulipanes = j+1;
        pedido_internet1.cantRosas = j+1;
        pedido_internet1.origen = INTERNET;
        config_pedidos_internet.push_back(pedido_internet1);
    }
    return config_pedidos_internet;

}