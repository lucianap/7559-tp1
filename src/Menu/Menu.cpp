
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
        cout << "1: Salvar" << endl;
        cout << "0: Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> input;

        switch (input) {
            case '0':
                cout << endl;
                cout << "Se cierran los procesos." << endl;
                break;
            case '1':
                this->salvar();
                cout << endl;
                cout << "Guardando estado y finalizando." << endl;
                break;
            default:
                cout << "Opcion no reconocida" << endl;
                break;
        }
    }
}

void Menu::agregarProcesos(std::vector<Distribuidor*> nuevosProcesos) {
    this->distribuidores = nuevosProcesos;
}

void Menu::agregarProcesos(std::vector<Productor*> nuevosProcesos) {
    this->productores = nuevosProcesos;
}

void Menu::salvar() {
    for(auto it = distribuidores.begin(); it != distribuidores.end(); it++) {
        //mando señales a todos para que guarden y mueran.
        (*it)->guardar();
    }

    for(auto it = productores.begin(); it != productores.end(); it++) {
        //mando señales a todos para que guarden y mueran.
        (*it)->guardar();
    }
}
