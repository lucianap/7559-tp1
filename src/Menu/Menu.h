//
// Created by nestor on 18/09/19.
//

#ifndef INC_7559_TP1_MENU_H
#define INC_7559_TP1_MENU_H


#include <iostream>
#include <string>
#include <ProcesoHijo/ProcesoHijo.h>
#include <vector>
#include <Distribuidor/Distribuidor.h>
#include <Productor/Productor.h>

class Menu {

public:
    Menu();

    /**
     * Inicia el menu interactivo para el usuario.
     */
    int iniciar();

};


#endif //INC_7559_TP1_MENU_H
