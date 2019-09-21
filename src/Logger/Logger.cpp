#include <ctime>

#include "./Logger.h"
#include <cstring>
#include <Utils/Utils.h>

Logger::Logger(bool debug) :
        debug(debug) {}

Logger::~Logger() = default;

void Logger::log(string message) {
    if (!debug) return;

    string logMessage = Utils::formatearMensajeLog(message);

    this->pipe.setearModo(Pipe::ESCRITURA);
    ssize_t tamanioEnviado = this->pipe.escribir(logMessage.c_str(), MENSAGE_LOG_SIZE);

    if (tamanioEnviado == -1) {
        string mensajeError = strerror(errno);
        cerr<<(string(mensajeError))<<endl;
    } else if (tamanioEnviado != MENSAGE_LOG_SIZE) {
        string mensajeError = "No se pudo completar la escritura en el log para pid" + getpid();
        cerr<<(string(mensajeError))<<endl;
    }
}

const Pipe &Logger::getPipe() const {
    return pipe;
}
