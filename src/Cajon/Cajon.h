#ifndef CAJON_H_
#define CAJON_H_

#include <vector>
#include <string>
#include "../Ramo/Ramo.h"

class Cajon {
    private:
        int id;
        std::vector<Ramo> ramos;
    public:
        Cajon(std::vector<Ramo> ramos);
        Cajon(std::string cajon_serializado);
        std::string serializar(); 
};

#endif /* CAJON_H_ */