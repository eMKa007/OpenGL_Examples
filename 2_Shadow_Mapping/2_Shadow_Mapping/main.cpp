#include "game.h"

int main( int argc, char* argv[] )
{
	try
	{
        Game game("2_Bilard_Balls", 
		1280,
		720,
		4,
		4,
		true);
	    game.run();
	}
    catch (std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

	return 0;
}
