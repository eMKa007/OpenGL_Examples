#include "game.h"

int main( int argc, char* argv[] )
{
	Game game("Tutorial_001", 
		1280,
		720,
		4,
		4,
		true);

	/* Main Loop */
	while( !game.getWindodShouldClose() )
	{
		game.update();
		game.render();
	}

	return 0;
}
