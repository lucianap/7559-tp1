//
// Created by Nacho on 23/9/19.
//

#ifndef INC_7559_TP1_FILEMANAGER_H
#define INC_7559_TP1_FILEMANAGER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <Logger/Logger.h>

using namespace std;


class FileManager {
private:
    ifstream inFile;
    string fileName;
    Logger& logger;
    void get_default(std::vector<t_parametros_pedido>& config_pedidos_internet);
public:
    FileManager(string fileName, Logger& logger);
    void ReadFile(std::vector<t_parametros_pedido>& config_pedidos_internet);
    ~FileManager(){};
};


#endif //INC_7559_TP1_FILEMANAGER_H
