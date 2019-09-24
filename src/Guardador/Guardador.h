//
// Created by luciana on 9/22/19.
//

#ifndef INC_7559_TP1_GUARDADOR_H
#define INC_7559_TP1_GUARDADOR_H

#include <fstream>
#include <sstream>
#include <ProcesoHijo/ProcesoHijo.h>
#include <Productor/Productor.h>
#include <ProcesoHijoFactory/ProcesoHijoFactory.h>
#include <Distribuidor/Distribuidor.h>
#include <PuntoVenta/PuntoVenta.h>
#include <ProcesoClientes/ProcesoClientes.h>

class Guardador {

    static const std::string carpeta;
    static const std::string prefijoProductores;
    static const std::string prefijoDistribuidores;
    static const std::string prefijoVendedores;
    static const std::string prefijoClientes;

    public:

        Guardador();

        static void inicializar();
        static void cleanUp();

        void guardar(Productor *proceso);
        void guardar(Distribuidor *proceso);
        void guardar_ptoVenta(PuntoVenta *proceso);
        void guardar_cliente(ProcesoClientes *proceso);

        static bool isCantidadDeArchivosGuardadosOk(int cantidadEsperada);

    private:

        void guardar(ProcesoHijo *proceso, std::string prefijo);

};


#endif //INC_7559_TP1_GUARDADOR_H
