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

    void agregarProcesos(std::vector<Distribuidor*> procesos);
    void agregarProcesos(std::vector<Productor*> procesos);

    /**
     * Inicia el menu interactivo para el usuario.
     */
    void iniciar();

private:

    //TODO extract this
    vector<Distribuidor*> distribuidores;
    vector<Productor*> productores;

    //envía signals de salvar a todos los procesos
    void salvar();

};


#endif //INC_7559_TP1_MENU_H
