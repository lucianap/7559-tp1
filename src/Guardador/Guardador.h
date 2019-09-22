//
// Created by luciana on 9/22/19.
//

#ifndef INC_7559_TP1_GUARDADOR_H
#define INC_7559_TP1_GUARDADOR_H


#include <ProcesoHijo/ProcesoHijo.h>

class Guardador {

    public:

        Guardador();

        void guardar(ProcesoHijo &proceso);

        ProcesoHijo& restaurar(std::string procesoSerializado);

};


#endif //INC_7559_TP1_GUARDADOR_H
