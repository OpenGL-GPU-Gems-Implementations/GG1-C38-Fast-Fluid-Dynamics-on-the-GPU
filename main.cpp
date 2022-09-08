/**
 * @file main.cpp
 * @author Eron Ristich (eron@ristich.com)
 * @brief Open SDL window
 * @version 0.1
 * @date 2022-09-03
 */

#include <iostream>
#include <string>
using std::cout;
using std::string;

#include "util/kernel/kernel.h"
#include "util/handler.h"
#include "GG1-C38-handler.h"
#include "objects/helper.h"

int main(int argc, char* argv[]) {
    Kernel* kernel = new Kernel(string("Fluid"), 800, 800);
    GG1_C38_Handler* handler = new GG1_C38_Handler();

    Handler::registerKernel(kernel);
    Handler::registerHandler(handler);

    kernel->start();
    
    delete kernel;

    return 0;
}