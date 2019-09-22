//
// Created by luciana on 9/22/19.
//

#ifndef INC_7559_TP1_SIGUSR1_HANDLER_H
#define INC_7559_TP1_SIGUSR1_HANDLER_H


#include <csignal>
#include <assert.h>
#include "EventHandler.h"

/**
 * Signal destinada a indicarle a los procesos que deben salvar su estado y morir.
 */
class SIGUSR1_Handler : public EventHandler {

private:
    sig_atomic_t saveStateAndQuit;

public:

    SIGUSR1_Handler () : saveStateAndQuit(0) {
    }

    ~SIGUSR1_Handler () {
    }

    virtual int handleSignal ( int signum ) {
        assert ( signum == SIGINT );
        this->saveStateAndQuit = 1;
        return 0;
    }

    sig_atomic_t getSaveAndQuit () const {
        return this->saveStateAndQuit;
    }

};


#endif //INC_7559_TP1_SIGUSR1_HANDLER_H
