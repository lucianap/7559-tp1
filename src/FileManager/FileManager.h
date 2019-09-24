//
// Created by Nacho on 23/9/19.
//

#ifndef INC_7559_TP1_FILEMANAGER_H
#define INC_7559_TP1_FILEMANAGER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <Logger/Logger.h>

using namespace std;


class FileManager {
private:
    ifstream inFile;
    string fileName;
    Logger& logger;
public:
    FileManager(string fileName, Logger& logger);
    string ReadFile();
    ~FileManager();
};


#endif //INC_7559_TP1_FILEMANAGER_H
