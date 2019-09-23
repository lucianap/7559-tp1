#ifndef CAJON_H_
#define CAJON_H_

#include <vector>
#include <string>
#include "Ramo/Ramo.h"

class Cajon {
    private:
        int id;

    public:

        std::vector<Ramo> ramos;

        /***
         * Capacidad para completar el cajon con ramos necesarios para el envio al distribuidor
         */
        static const int CAPACIDAD_RAMOS = 10;
        static const int CAPACIDAD_RAMOS_DISTRIBUIDOR = 100;

        /***
         * Tamanio del paquete de bytes que se envia desde el productor al distribuidor
         */
        static const int TAM_TOTAL_BYTES = CAPACIDAD_RAMOS * (Ramo::TAM_TIPO_FLOR + Ramo::TAM_ID_PRODUCTOR);
        static const int TAM_TOTAL_BYTES_DISTRIBUIDOR = CAPACIDAD_RAMOS_DISTRIBUIDOR * (Ramo::TAM_TIPO_FLOR + Ramo::TAM_ID_PRODUCTOR);

        Cajon();
        Cajon(const Cajon & cajon);
        Cajon(std::vector<Ramo> ramos);
        Cajon(const std::string& cajon_serializado, int ramos_por_cajon);
        std::string serializar();
        vector<Ramo> filtrar(TipoFlor tipoFlor);
        ~Cajon();

};

#endif /* CAJON_H_ */