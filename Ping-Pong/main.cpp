#include "game.h"
using namespace std;

int main(int argc, char** argv)
{
	srand(time(nullptr)); // use current time as seed for random generator
	game game;
	
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}