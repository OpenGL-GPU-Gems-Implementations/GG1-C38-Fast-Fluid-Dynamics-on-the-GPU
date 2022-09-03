/**
 * @file handler.cpp
 * @author Eron Ristich (eron@ristich.com)
 * @brief Set of handlers for a kernel object
 * @version 0.1
 * @date 2022-07-21
 */

#include "handler.h"

Kernel* Handler::kernel = NULL;
Handler* Handler::handler = NULL;

Handler::Handler() {}
Handler::~Handler() {}

/**
 * @brief Handles all events via registered handler object
 */
void Handler::eventHandler() {
    handler->objEventHandler();
}

/**
 * @brief Handles rendering of all objects in the scene via registered handler object
 */
void Handler::rendererHandler() {
    handler->objRendererHandler();
}

/**
 * @brief Handles updates to all objects in the scene via registered handler object
 */
void Handler::updateHandler() {
    handler->objUpdateHandler();
}

/**
 * @brief Handles updates to all objects in the scene via registered handler object
 */
void Handler::preLoopStep() {
    handler->objPreLoopStep();
}

// Stand-ins
void Handler::objEventHandler() {}
void Handler::objRendererHandler() {}
void Handler::objUpdateHandler() {}
void Handler::objPreLoopStep() {}

/**
 * @brief Register handler object. Also registers handler functions in registered kernel. Kernel MUST be registered before handler.
 * 
 * @param h handler object to be registered
 * @return boolean representing the success of the operation
 */
bool Handler::registerHandler(Handler* h) {
    if (kernel == NULL) {
        cout << "ERROR: Kernel must be registered before handler in order to register kernel functions with the handler" << endl;
        return false;
    }
    handler = h;
    
    kernel->registerEventHandler(eventHandler);
    kernel->registerRendererHandler(rendererHandler);
    kernel->registerUpdateHandler(updateHandler);
    kernel->registerPreLoopStep(preLoopStep);

    cout << "Handler successfully registered" << endl;

    return true;
}

/**
 * @brief Register kernel object
 * 
 * @param k kernel object to be registered
 * @return boolean representing the success of the operation
 */
bool Handler::registerKernel(Kernel* k) {
    kernel = k;

    cout << "Kernel successfully registered" << endl;

    return true;
}