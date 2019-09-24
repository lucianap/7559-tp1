//
// Created by Nacho on 23/9/19.
//

#include "FileManager.h"

FileManager::FileManager(string fileName, Logger& logger) :
    fileName(fileName), logger(logger){}

std::string FileManager::ReadFile(){
    string str = "";
    string x;
    inFile.open(fileName);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (inFile >> x) {
        try{
            str = str + x;
        }catch(std::string &error) {
            logger.log("Error al leer el archivo " + error);
            break;
        }
    }

    inFile.close();
    return str;
}

