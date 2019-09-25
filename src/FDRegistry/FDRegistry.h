
#ifndef INC_7559_TP1_FDREGISTRY_H
#define INC_7559_TP1_FDREGISTRY_H

#include <vector>

using namespace std;
/***
 * Clase FDRegistry para cerrar file descriptors automaticamente al finalizar ejecucion
 */
class FDRegistry final {
public:
    static void AddFileDescriptor(int fileDescriptor);
    static void CloseFileDescriptors();

    ~FDRegistry();

private:
    static FDRegistry* instance;
    vector<int> fileDescriptors;

    FDRegistry();
    static FDRegistry* getInstance();
};


#endif //ADUANACONCULANDIA_STATUS_H
