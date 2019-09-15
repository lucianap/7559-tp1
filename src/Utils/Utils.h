#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>

class Utils {
    public:
        //hace un join usando el char c de las string pasadas en el vector v y coloca el resultado en s.
        static void join(const std::vector<std::string>& v, char c, std::string& s );
    private:
        //para hacer completamente estática la clase prohíbo la construcción
        Utils();
};

#endif /* UTILS_H_ */