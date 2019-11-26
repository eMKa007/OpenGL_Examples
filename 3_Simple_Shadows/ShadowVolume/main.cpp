#include "Game.h"

int main()
{
    Game App(new string("TestApp"));

    try
    {
        App.run();
    }
    catch  (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
}