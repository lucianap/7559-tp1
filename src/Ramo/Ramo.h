#ifndef RAMO_H_
#define RAMO_H_

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "../TipoFlor/TipoFlor.h"
#include "../Utils/Utils.h"

using std::string;

class Ramo {

    private:

        int id;
        int producido_por;
        TipoFlor tipoFlor;
    public:
        static const int TAM_ID_PRODUCTOR = 4;
        static const int TAM_TIPO_FLOR = 1;
        static const int TAM_TOTAL = TAM_ID_PRODUCTOR + TAM_TIPO_FLOR;

        Ramo(const Ramo & ramo );
        Ramo(int id_productor, TipoFlor f);
        Ramo(std::string ramo_seralizado);

        int get_productor();
        TipoFlor getTipoFlor();
        string serializar();

        string toString();
};

#endif /* RAMO_H_ */