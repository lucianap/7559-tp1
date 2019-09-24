#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <TipoPedido/TipoPedido.h>
#include "../TipoFlor/TipoFlor.h"


using namespace std;
typedef struct parametros {
    bool reanudar;
    int cantProductores;
    int cantPuntosVenta;
    int cantDistribuidores;
    bool debug;
} t_parametros;


class Utils {

public:

    static const int TAM_HEADER = 5;

    //hace un join usando el char c de las string pasadas en el vector v y coloca el resultado en s.
    static void join(const std::vector<std::string>& v, char c, std::string& s );

    static void join(const std::vector<std::string>& v, std::string& s );

    static std::string& ltrim(std::string& str, const std::string& chars);

    static std::string& rtrim(std::string& str, const std::string& chars);

    static std::string& trim(std::string& str);

    static std::vector<std::string> split(const std::string& str, const std::string& delim);

    static t_parametros tomarParametros(int argc,char* argv[]);

    static std::string formatearMensajeLog(std::string mensaje);

    static string getTextTipoFlor(TipoFlor tipoFlor);

    static string getTextTipoPedido(TipoPedido tipoPedido);

    static int remove_directory(const char* path);

    static int countFiles(std::string dir);

    static bool startsWith(const char *pre, const char *str);

    static vector<string> listarArchivosConPrefijo(const char* path, string prefijo);

    static TipoFlor getTipoFlor(int tipoFlor);

private:
        //para hacer completamente estática la clase prohíbo la construcción
        Utils();

};

#endif /* UTILS_H_ */