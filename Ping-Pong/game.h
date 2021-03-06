#include "SDL.h"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>

using namespace std;

struct VectorPlayer1 {
	float x;
	float y;
};

struct VectorPlayer2 {
	float x;
	float y;
};

struct VectorBallPos {
	float x;
	float y;
};

struct VectorBallDir {
	int x;
	int y;
};

struct zoneStartGame {
	int* x1;
	int* x2;
	int* y1;
	int* y2;
};

class game {

public:
	game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	float mPaddleDirPlayer1;
	float mPaddleDirPlayer2;
	unsigned int countInt;
	const char* count;
	int widthPaddles;
	int heightPaddles;
	int posAndWidthPlayer1;
	int posAndWidthPlayer2;
	int posAndHeightPlayer1;
	int posAndHeightPlayer2;
	float accelerationPaddle;
	bool mIsRunning;

	unsigned int gameState;

	void UpdateGame();
	void GenerateOutput();

	zoneStartGame clickableZone;
	VectorPlayer1 mPaddlePosPlayer1;
	VectorPlayer2 mPaddlePosPlayer2;
	VectorBallPos mBallPos;
	VectorBallDir mBallDir;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Rect DstRect;
	SDL_Rect DstRect2;
	SDL_Rect DstRect2Timer;
	SDL_Surface* TextSurface;
	SDL_Color TextColor;
	SDL_Texture* Message;
	SDL_Texture* Message2;
	SDL_Texture* Message2Timer;
	TTF_Font* Font;
	SDL_Surface* row2;
	SDL_Surface* row2Timer;

	
};
