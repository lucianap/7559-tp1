#ifndef RAMO_H_
#define RAMO_H_

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "../TipoFlor/TipoFlor.h"
#include "../Utils/Utils.h"

class Ramo {
    private:
        int id;
        int producido_por;
        TipoFlor tipoFlor;
    public:
        Ramo(int id_productor, TipoFlor f);
        Ramo(std::string ramo_seralizado);

        int get_productor();
        TipoFlor getTipoFlor();
        std::string serializar();
};

#endif /* RAMO_H_ */