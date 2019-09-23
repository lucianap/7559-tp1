
#include "Menu.h"
using std::cin;
using std::cout;
using std::endl;

Menu::Menu() {

}

int Menu::iniciar() {
    char input = 'a';
    while (input != '0' && input != '1') {
        cout << endl;
        cout << "------- MENU -------" << endl;
        cout << "1: Salvar" << endl;
        cout << "0: Salir" << endl;
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
            default:
                cout << "Opcion no reconocida" << endl;
                break;
        }
    }
}



