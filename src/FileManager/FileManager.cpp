//
// Created by Nacho on 23/9/19.
//


#include <TipoPedido/TipoPedido.h>
#include <sstream>

#include "FileManager.h"

FileManager::FileManager(string fileName, Logger& logger) :
    fileName(fileName), logger(logger){}

void  FileManager::ReadFile(std::vector<t_parametros_pedido>& config_pedidos_internet) {

    inFile.open(fileName);

    if(!inFile.is_open()){
        logger.log("Error al abrir el archivo.");
        logger.log("Cargando pedido default.");
        this->get_default(config_pedidos_internet);
        return;
    }

    string cant_rosas;
    string cant_tulipanes;
    bool es_header = true;
    try{
        while(inFile.good()){
            getline(inFile,cant_tulipanes,',');
            getline(inFile,cant_rosas,'\n');
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
        this->get_default( config_pedidos_internet);
    }
    inFile.close();
}

void FileManager::get_default(std::vector<t_parametros_pedido>& config_pedidos_internet){

    for (int j = 0; j < 10; ++j) {
        t_parametros_pedido pedido_internet1;
        pedido_internet1.cantTulipanes = j+1;
        pedido_internet1.cantRosas = j+1;
        pedido_internet1.origen = INTERNET;
        config_pedidos_internet.push_back(pedido_internet1);
    }

}