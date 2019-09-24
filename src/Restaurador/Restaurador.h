//
// Created by luciana on 9/24/19.
//

#ifndef INC_7559_TP1_RESTAURADOR_H
#define INC_7559_TP1_RESTAURADOR_H


#include <Productor/Productor.h>
#include <Distribuidor/Distribuidor.h>

class Restaurador {

public:

    std::vector<Productor*> restaurarProductores();
    std::vector<Distribuidor*> restaurarDistribuidores();

    void conectarPipes(std::vector<Productor*> productores,
                       std::vector<Distribuidor*> distribuidores);

};


#endif //INC_7559_TP1_RESTAURADOR_H
