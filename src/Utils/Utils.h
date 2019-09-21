#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>


using namespace std;
typedef struct parametros {
    int cantProductores;
    int cantPuntosVenta;
    int cantDistribuidores;
    bool debug;
} t_parametros;



class Utils {
    public:

        //hace un join usando el char c de las string pasadas en el vector v y coloca el resultado en s.
        static void join(const std::vector<std::string>& v, char c, std::string& s );

        static void join(const std::vector<std::string>& v, std::string& s );

        static std::string& ltrim(std::string& str, const std::string& chars);

        static std::string& rtrim(std::string& str, const std::string& chars);

        static std::string& trim(std::string& str);

        static t_parametros tomarParametros(int argc,char* argv[]);

private:
        //para hacer completamente estática la clase prohíbo la construcción
        Utils();
};

#endif /* UTILS_H_ */