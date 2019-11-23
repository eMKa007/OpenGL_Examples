#include "game.h"

int main( int argc, char* argv[] )
{
	Game game("2_Bilard_Balls", 
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
