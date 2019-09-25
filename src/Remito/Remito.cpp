//
// Created by root on 25/9/19.
//

#include <sys/stat.h>
#include "Remito.h"

using namespace std;

const std::string Remito::carpeta = "remitos";

void Remito::inicializar(){
    int status = mkdir(Remito::carpeta.c_str(), 0777);
    //TODO manejar errores.
}

void Remito::cleanUp() {
    int status = Utils::remove_directory(Remito::carpeta.c_str());
    //TODO manejar errores.8
}

Remito::Remito(int idPuntoVenta):idPuntoVenta(idPuntoVenta) {
}

void Remito::guardar() {
    string filename = carpeta + "/";
    filename += std::to_string(time(0));
    filename += "remito";
    filename += std::to_string(idPuntoVenta);
    filename += ".txt";
    ofstream output_file(filename);
    output_file << "REMITO" <<endl;
    output_file << "Punto de venta" << idPuntoVenta <<endl;
    output_file << pedido.str();
    output_file << "--------------------------------------------------"<<endl;
    output_file.close();
}

void Remito::agregarRamo(Ramo ramo) {
    pedido << "--------------------------------------------------"<<endl;
    pedido << "Productor: " << ramo.get_productor()<<endl;
    pedido << "TipoFlor: " << (ramo.getTipoFlor()==0?"Tulipan":"Rosa")<<endl;
}
