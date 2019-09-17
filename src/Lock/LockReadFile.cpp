//
// Created by nestor on 30/04/18.
//

#include "LockReadFile.h"

LockReadFile :: LockReadFile ( const std::string nombre ) {

    this->nombre = nombre;
    this->fl.l_type = F_RDLCK;
    this->fl.l_whence = SEEK_SET;
    this->fl.l_start = 0;
    this->fl.l_len = 0;
    this->fd = open ( this->nombre.c_str(),O_CREAT|O_RDONLY,0777 );
}

int LockReadFile :: tomarLock () {
    this->fl.l_type = F_RDLCK;
    return fcntl ( this->fd,F_SETLKW,&(this->fl) );
}

int LockReadFile :: liberarLock () {
    this->fl.l_type = F_UNLCK;
    return fcntl ( this->fd,F_SETLK,&(this->fl) );
}

void LockReadFile :: rebobinar() {
    lseek ( this->fd,0,SEEK_SET );
}

ssize_t LockReadFile ::leer(void *buffer, const ssize_t buffsize) const {
    return read( this->fd, buffer,buffsize );
}

ssize_t LockReadFile ::leer(char *buffer, const ssize_t buffsize) const {
    return read( this->fd, buffer,buffsize );
}

LockReadFile :: ~LockReadFile () {
    close ( this->fd );
}