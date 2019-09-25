
#include <Informe/Informe.h>
#include <Status/SolicitudStatus.h>
#include "Menu.h"
using std::cin;
using std::cout;
using std::endl;

Menu::Menu(Status & status, Logger & logger) : status(status), logger(logger) {
}

int Menu::iniciar() {
    char input = 'a';
    while (input != '0' && input != '1') {
        cout << endl;
        cout << "------- MENU -------" << endl;
        cout << "0: Salir" << endl;
        cout << "1: Salvar" << endl;
        cout << "2: Ver informe" << endl;
        cout << "Seleccione una opcion: ";
        cin >> input;

        switch (input) {
            case '0':
                cout << endl;
                cout << "Se cierran los procesos." << endl;
                return 0;
            case '1':
                cout << endl;
                cout << "Guardando estado y finalizando." << endl;
                return 1;
            case '2':
                this->mostrarInfome();
                break;
            default:
                cout << "Opcion no reconocida" << endl;
                break;
        }
    }
}

void Menu::mostrarInfome() {
    try {
        Informe informe = this->realizarConsulta();
        string productorQueMasVendioText = "Ninguno";
        if (informe.getProductorMejorVenta() != 99) {
            productorQueMasVendioText = std::to_string(informe.getProductorMejorVenta());
        }
        cout << "******************************************" << endl;
        cout << "Id del productor que mas vendio : "<< productorQueMasVendioText << endl;
        cout << "Tipo de flor mas comprada       : "<< Utils::getTextTipoFlor(informe.getFlorMasComprada())<< endl;
        cout << "******************************************" << endl;
    } catch (std::string &error) {
        logger.log("Error consultando informe: " + error);
    }

}

Informe Menu::realizarConsulta() {

    SolicitudStatus solicitudStatus(SolicitudStatus::TIPO_SOLICITUD_INFORME);
    ssize_t status = this->status.getPipeEntrada().escribir(solicitudStatus.serializar().c_str(),
            SolicitudStatus::TAM_TOTAL);
    if (status != SolicitudStatus::TAM_TOTAL) {
        string mensajeError;
        if (status == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al enviar una solicitud del menu al status";
        throw(std::string(mensajeError));
    }

    char buffer[Informe::TAM_TOTAL_BYTES];
    status = this->status.getPipeSalida().leer(buffer, Informe::TAM_TOTAL_BYTES);
    if (status != Informe::TAM_TOTAL_BYTES) {
        string mensajeError;
        if (status == -1)
            mensajeError = strerror(errno);
        else
            mensajeError = "Error al leer el informe del status";
        throw(std::string(mensajeError));
    }

    return Informe(buffer);
}



