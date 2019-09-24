
#include <Informe/Informe.h>
#include "Menu.h"
using std::cin;
using std::cout;
using std::endl;

Menu::Menu(Status & status) : status(status) {

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

    Informe informe = this->realizarConsulta();

    cout << "******************************************" << endl;
    cout << "Id del productor que mas vendio : " << informe.getProductorMejorVenta()<< endl;
    cout << "Tipo de flor mas comprada       : " << Utils::getTextTipoFlor(informe.getFlorMasCompada())<< endl;
    cout << "******************************************" << endl;

}

Informe Menu::realizarConsulta() {
    return Informe(0, Tulipan);
}



