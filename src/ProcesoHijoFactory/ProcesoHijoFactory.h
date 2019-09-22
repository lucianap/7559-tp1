//
// Created by luciana on 9/22/19.
//

#ifndef INC_7559_TP1_PROCESOHIJOFACTORY_H
#define INC_7559_TP1_PROCESOHIJOFACTORY_H


#include <ProcesoHijo/ProcesoHijo.h>
#include <TipoProceso/TipoProceso.h>
#include <Productor/Productor.h>

class ProcesoHijoFactory {

    public:
        static ProcesoHijo* crear(std::string procesoSerializado, Logger& logger) {
            //El tipo de proceso siempre viene en los primeros 5 bytes.
            auto t = (TipoProceso) std::stoi(procesoSerializado.substr(0,5));
            switch (t) {
                case vendedor:
                    //TODO
                    break;
                case distribuidor:
                    //TODO
                    break;
                case productor:
                    return new Productor(logger, procesoSerializado);
            }
        }
};


#endif //INC_7559_TP1_PROCESOHIJOFACTORY_H
