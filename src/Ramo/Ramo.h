#ifndef RAMO_H_
#define RAMO_H_

#include <string>
#include "../TipoFlor/TipoFlor.h"

class Ramo {
    private:
        int id;
        int producido_por;
        TipoFlor tipoFlor;
    public:
        Ramo(int id_productor);
        Ramo(std::string ramo_seralizado);

        int get_productor();
        std::string serializar();        
};

#endif /* RAMO_H_ */