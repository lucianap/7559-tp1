//
// Created by nestor on 23/09/19.
//

#include "Status.h"

Status::Status(Logger &logger) : ProcesoHijo(logger) {}

Status::~Status() {

}

pid_t Status::ejecutar() {
    return 0;
}
