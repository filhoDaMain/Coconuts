#include <iostream>
#include "coconuts/core.h"
#include "demo.h"

int main(void)
{
    std::cout << "This is a Game - version: " << DEMO_VERSION << std::endl;

    coconuts_version();
    
    return 0;
}