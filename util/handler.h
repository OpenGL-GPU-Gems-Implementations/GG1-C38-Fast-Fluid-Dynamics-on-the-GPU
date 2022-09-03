/**
 * @file handler.h
 * @author Eron Ristich (eron@ristich.com)
 * @brief Set of handlers for a kernel object
 * @version 0.1
 * @date 2022-07-21
 */

#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "kernel/kernel.h"

class Handler {
    public:
        Handler();
        ~Handler();

        // the way this works is that classes that extend Handler 
        // reimplement obj handlers, which is called by their respective
        // static handler
        // as they are static, the registered kernel must be coupled with
        // a registered handler object, and the static handler will call the
        // specified function

        virtual void objEventHandler();
        virtual void objRendererHandler();
        virtual void objUpdateHandler();
        virtual void objPreLoopStep();

        static void eventHandler();
        static void rendererHandler();
        static void updateHandler();
        static void preLoopStep();

        static bool registerKernel(Kernel* k);
        static bool registerHandler(Handler* h);

    protected:
        static Kernel* kernel;
        static Handler* handler;
};

#endif