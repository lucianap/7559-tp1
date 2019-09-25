#include <cstdio>
#include <unistd.h>

#include "./FDRegistry.h"

FDRegistry* FDRegistry::instance = nullptr;

FDRegistry* FDRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new FDRegistry();
    }

    return instance;
}

void FDRegistry::AddFileDescriptor(int fileDescriptor) {
    FDRegistry::getInstance()->fileDescriptors.push_back(fileDescriptor);
}

void FDRegistry::CloseFileDescriptors() {
    for(auto &fileDescriptor : FDRegistry::getInstance()->fileDescriptors) {
        close(fileDescriptor);
    }
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
}

FDRegistry::FDRegistry() = default;
FDRegistry::~FDRegistry() = default;