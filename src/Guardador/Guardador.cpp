//
// Created by luciana on 9/22/19.
//

#include "Guardador.h"


const std::string Guardador::prefijoProductores = "productor_pid_";
const std::string Guardador::prefijoDistribuidores = "distribuidor_pid_";
const std::string Guardador::prefijoVendedores = "vendedor_pid_";

void Guardador::guardar(ProcesoHijo *proceso, std::string prefijo) {
    std::ofstream out(prefijo);
    out << proceso->serializar();
    out.close();
}


void Guardador::guardar(Productor *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoProductores << proceso->getProcessId();
    Guardador::guardar(proceso, ss.str());

}

void Guardador::guardar(Distribuidor *proceso) {
    std::stringstream ss;
    ss << Guardador::prefijoDistribuidores << proceso->getProcessId();
    Guardador::guardar(proceso, ss.str());

}


Guardador::Guardador() {}

ProcesoHijo* Guardador::restaurar(Logger& logger, std::string procesoSerializado) {
    return ProcesoHijoFactory::crear(procesoSerializado, logger);
}

