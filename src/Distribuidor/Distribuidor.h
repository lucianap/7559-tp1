
#ifndef INC_7559_TP1_DISTRIBUIDOR_H
#define INC_7559_TP1_DISTRIBUIDOR_H

#include <Ramo/Ramo.h>
#include <Cajon/Cajon.h>
#include "Distribuidor.h"
#include <Fifos/FifoLectura.h>
#include <Pipes/Pipe.h>
#include "../ProcesoHijo/ProcesoHijo.h"

class Distribuidor : public ProcesoHijo {

private:
    int idDistribuidor;
    Pipe entradaFlores;

    std::vector<Pipe*> ptos_de_venta;
    void iniciarAtencion();

    Cajon* recibirCajon(char *buffer);

    std::string serializar() override;

    void enviarCajon(std::vector<Ramo*> ramos, Pipe *distribuidor_destino) ;
public:
    Distribuidor(Logger& logger, std::vector<Pipe*> ptos_de_venta,int idDistribuidor, Pipe* pipe);

    ~Distribuidor() override;

    pid_t ejecutar() override;

};


#endif //INC_7559_TP1_DISTRIBUIDOR_H
