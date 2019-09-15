#include "Productor.h"

Productor::Productor(std::vector<int> distribuidores, int ramos_por_cajon): 
    distribuidores(distribuidores), ramos_por_cajon(ramos_por_cajon) {};

void Productor::ejecutar_productor(){
    
    std::vector<Ramo> ramos_a_enviar;
    ramos_a_enviar.reserve(10);

    int ramos_producidos = 0;
    while(true) {
        
        Ramo r = this->producir_ramo();
        ramos_a_enviar.push_back(r);

        ramos_producidos++;
        
        if(ramos_producidos >= this->ramos_por_cajon) {
            this->enviar_cajon(ramos_a_enviar);
            ramos_a_enviar.clear();
        }
    }

}

Ramo Productor::producir_ramo() {
    //Solo para simular la producción del ramo.
    sleep(500);
    return Ramo(this->id);
}

void Productor::enviar_cajon(std::vector<Ramo> ramos) {

    Cajon c(ramos);
    
    std::string cajon_a_enviar = c.serializar();
    
    //TODO código para enviar a los distribuidores, que serían probablemente file descriptors.

}