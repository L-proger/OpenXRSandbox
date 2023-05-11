
#include <iostream>


#include "OpenXRContext.h"

int main() {

    RendererD3D11 renderer;

    OpenXRContext context(renderer.getDevice());

    std::cout << "Test" << std::endl;
    return 0;
}