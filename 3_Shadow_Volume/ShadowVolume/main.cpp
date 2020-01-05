#include "Game.h"

int main()
{
    try
    {
        Game App(new string("OpenGL - Shadow Volume Implementation."));
        App.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
}