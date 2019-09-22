//
// Created by nestor on 21/09/19.
//

#include "LoggerProcess.h"
#include <string>
#include <cstring>
#include <Signal/SignalHandler.h>
#include <Utils/Utils.h>


LoggerProcess::LoggerProcess(string output, Logger & logger) :
        fs(output, std::ofstream::out), ProcesoHijo(logger), entradaMensajes(logger.getPipe()) {}



pid_t LoggerProcess::ejecutar() {

    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    this->log(Utils::formatearMensajeLog("Proceso LoggerProcess inicializado!"));

    entradaMensajes.setearModo(Pipe::LECTURA);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

    this->iniciarAtencion();

    SignalHandler::destruir();
    exit(0);

}

void LoggerProcess::iniciarAtencion() {

    char buffer[Logger::MENSAGE_LOG_SIZE];

    while (sigint_handler.getGracefulQuit() == 0) {
        try {
            string mensaje = this->recibirMensaje(buffer);
            this->log(mensaje);
        } catch (std::string &error) {
            cerr<<"Error atendiendo mensajes de log: " << error << endl;
            break;
        }
    }
    this->log(Utils::formatearMensajeLog("Proceso LoggerProcess finalizado!"));
    fs.close();
    entradaMensajes.cerrar();

}

string LoggerProcess::recibirMensaje(char *buffer) {
    string mensajeError;
    string mensaje;
    ssize_t bytesleidos = entradaMensajes.leer(static_cast<void*>(buffer), Logger::MENSAGE_LOG_SIZE);
    if (bytesleidos != Logger::MENSAGE_LOG_SIZE) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error de tamaño al leer el mensaje para loguear";
        throw(std::string(mensajeError));
    }

    return mensaje.append(buffer);
}

void LoggerProcess::log(string mensaje) {
    fs << mensaje << endl;
};