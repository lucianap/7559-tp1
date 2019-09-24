//
// Created by luciana on 9/24/19.
//

#ifndef INC_7559_TP1_RESTAURADOR_H
#define INC_7559_TP1_RESTAURADOR_H


#include <Productor/Productor.h>
#include <Distribuidor/Distribuidor.h>

class Restaurador {
private:
    std::vector<string> leerProcesoSerializado(std::string prefijo);
    std::multimap<int, int> restaurarAsignaciones(std::string asignaciones);
    std::string leerAsignacionesProductorDistribuidor();

    map<int, Pipe *> distribuidoresEntradaByDistId;

public:

    std::vector<Productor*> restaurarProductores(Logger& logger);
    std::vector<Distribuidor*> restaurarDistribuidores(Logger& logger);

    void conectarPipes(std::vector<Productor*> productores,
                       std::vector<Distribuidor*> distribuidores);

};


#endif //INC_7559_TP1_RESTAURADOR_H
