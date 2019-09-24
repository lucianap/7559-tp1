
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include <TipoProceso/TipoProceso.h>
#include <Ramo/Ramo.h>
#include <Cajon/Cajon.h>
#include "Distribuidor.h"
#include <Pipes/Pipe.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class Distribuidor : public ProcesoHijo {

public:
    static const int CANT_RAMOS_PARA_ENVIO = 100;

private:
    int idDistribuidor;
    Pipe entradaFlores;

    std::vector<Pipe*> ptos_de_venta;
    void iniciarAtencion();

    Cajon recibirCajon(char *buffer);

    std::string serializar() override;

    vector<Ramo> stockRosas;

    vector<Ramo> stockTulipanes;

    vector<Pipe*>::iterator ptsVentaIterator;

    Pipe* puntoVenta_actual;

    int siguientePtoVenta;

    void enviarCajon( Pipe *distribuidor_destino) ;

    void inicializarValores();
public:

    void clasificar(Cajon &cajon);

    bool hayDiponiblidadParaEnvio();

    void enviarAPuntosDeVenta();

    Distribuidor(Logger& logger);
    Distribuidor(Logger& logger, int idDistribuidor, Pipe* pipe);
    Distribuidor(Logger& logger, std::string distribuidorSerializado);
    Distribuidor(Logger& logger, std::vector<Pipe*> ptos_de_venta,int idDistribuidor, Pipe* pipe);

    ~Distribuidor() override;

    pid_t ejecutar() override;

    const vector<Ramo> &getStockRosas() const;

    void setStockRosas(const vector<Ramo> &stockRosas);

    const vector<Ramo> &getStockTulipanes() const;

    void setStockTulipanes(const vector<Ramo> &stockTulipanes);

    void logearStatus();
};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
