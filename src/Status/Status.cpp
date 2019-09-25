//
// Created by nestor on 23/09/19.
//

#include <Signal/SignalHandler.h>
#include <Informe/Informe.h>
#include "Status.h"
#include "SolicitudStatus.h"

Status::Status(Logger &logger) : ProcesoHijo(logger) {}

Status::~Status() {

}

void Status::cargar(std::string statusSerializado) {

    //TODO
    //implementar leer el proceso del status serializado.

    // implementar constructor copia
    //this->registroVenta =
}

void Status::setCantPipes(int cant){
    this->cantPipes = cant;
}

pid_t Status::ejecutar() {

    logger.log("Ejecutamos un Proceso Status");
    pid = fork();

    // en el padre devuelvo el process id
    if (pid != 0) return pid;

    // siendo distribuidor, me seteo y ejecuto lo que quiero
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1_handler);

    logger.log("Naci como Status y tengo el pid: "+to_string(getpid()));

    this->iniciarAtencion();
    logger.log("Termino la tarea del Status");
    SignalHandler::destruir();

    exit(0);
}

Pipe Status::getPipeEntrada()  {
    return pipeEntrada;
}

Pipe Status::getPipeSalida() {
    return pipeSalida;
}

void Status::iniciarAtencion() {
    int cantEofs = 0;
    while (sigint_handler.getGracefulQuit() == 0) {
        try {
            if(cantEofs>=2 && sigusr1_handler.getSaveAndQuit() != 0)break;
            SolicitudStatus unaSolicitud = recibirSolicitud();
            if(unaSolicitud.getTipoSolicitud() != SolicitudStatus::TIPO_SOLICITUD_EOF){
                despacharSolicitud(unaSolicitud);
            }else{
                cantEofs++;
            }

        } catch (std::string &error) {
            logger.log("Error atendiendo solicitudes en el status: " + error);
            break;
        }
    }

    this->getPipeEntrada().cerrar();
    this->getPipeSalida().cerrar();
}

void Status::despacharSolicitud(SolicitudStatus solicitud) {
    if (solicitud.getTipoSolicitud() == SolicitudStatus::TIPO_SOLICITUD_ALTA_VENTA) {
        registroVenta.contabilizarRamoVendido(solicitud.getRamo());
    } else if (solicitud.getTipoSolicitud() == SolicitudStatus::TIPO_SOLICITUD_INFORME) {
        this->enviarInforme();
    }
}

SolicitudStatus Status::recibirSolicitud() {

    char buffer[SolicitudStatus::TAM_TOTAL];
    string mensajeError;
    std::stringstream ss;

    ssize_t bytesleidos = pipeEntrada.leer(static_cast<void*>(buffer), SolicitudStatus::TAM_TOTAL);

    if (bytesleidos != SolicitudStatus::TAM_TOTAL) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer una solicitud en el Status. solo se lee bytes: " + bytesleidos;
        throw(std::string(mensajeError));
    }
    ss << buffer;
    SolicitudStatus solicitudStatus(ss.str());

    std::stringstream mensajelog;
    mensajelog << "Solicitud recibida: " << solicitudStatus.toString();
    logger.log(mensajelog.str());

    return solicitudStatus;
}

void Status::enviarInforme() {
    Informe informe = this->registroVenta.generarInformes();
    string mensajeError;
    ssize_t bytesleidos = pipeSalida.escribir(informe.serializar().c_str(),
            Informe::TAM_TOTAL_BYTES);

    if (bytesleidos != Informe::TAM_TOTAL_BYTES) {
        if (bytesleidos == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al escribir informe en el Status. Bytes escritos: " + std::to_string(bytesleidos);
        throw(std::string(mensajeError));
    }

}

std::string Status::serializar() {
    return this->registroVenta.serializar();
}


