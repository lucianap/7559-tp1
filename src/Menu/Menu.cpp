
#include "Menu.h"
using std::cin;
using std::cout;
using std::endl;

Menu::Menu() {

}

void Menu::iniciar() {
    char input = 'a';
    while (input != '0') {
        cout << endl;
        cout << "------- MENU -------" << endl;
        cout << "0: Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> input;

        switch (input) {
            case '0':
                cout << endl;
                cout << "Se cierran los procesos." << endl;
                break;
            default:
                cout << "Opcion no reconocida" << endl;
                break;
        }
    }
}