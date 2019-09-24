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
#include <Status/Status.h>
#include <Logger/Logger.h>
#include "Informe/Informe.h"
#include <cstring>
class Menu {

private:
    Status & status;
    Logger& logger;

public:
    Menu(Status & status, Logger & logger );

    /**
     * Inicia el menu interactivo para el usuario.
     */
    int iniciar();

    void mostrarInfome();

    Informe realizarConsulta();
};


#endif //INC_7559_TP1_MENU_H
