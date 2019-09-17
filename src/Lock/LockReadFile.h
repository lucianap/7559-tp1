//
// Created by nestor on 30/04/18.
//

#ifndef ADUANACONCULANDIA_LOCKREADFILE_H
#define ADUANACONCULANDIA_LOCKREADFILE_H

#include <unistd.h>
#include <fcntl.h>
#include <string>

using namespace std;

class LockReadFile {


private:
    struct flock fl;
    int fd;
    std::string nombre;

public:
    LockReadFile ( const std::string nombre );
    ~LockReadFile();

    int tomarLock ();
    int liberarLock ();
    void rebobinar();
    ssize_t leer (void* buffer,const ssize_t buffsize ) const;
    ssize_t leer (char* buffer,const ssize_t buffsize ) const;

};


#endif //ADUANACONCULANDIA_LOCKREADFILE_H
