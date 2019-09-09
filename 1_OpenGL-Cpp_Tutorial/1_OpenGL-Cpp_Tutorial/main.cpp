#include "game.h"

int main( int argc, char* argv[] )
{
	Game game("Tutorial_001", 
		640,
		480,
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
