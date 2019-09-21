#ifndef CAJON_H_
#define CAJON_H_

#include <vector>
#include <string>
#include "../Ramo/Ramo.h"

class Cajon {
    private:
        int id;
    public:
        std::vector<Ramo*> ramos;
        Cajon(std::vector<Ramo*> ramos);
        Cajon(const std::string& cajon_serializado, int ramos_por_cajon);
        std::string serializar();
        ~Cajon();
};

#endif /* CAJON_H_ */