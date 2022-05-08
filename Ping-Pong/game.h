#include "SDL.h"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>


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
	int randomForDir;

	unsigned int count;
	
	int widthPaddles;
	int heightPaddles;

	int posAndWidthPlayer1;
	int posAndWidthPlayer2;

	int posAndHeightPlayer1;
	int posAndHeightPlayer2;

	float accelerationPaddle;



	bool mIsRunning;

	void UpdateGame();
	void GenerateOutput();

	VectorPlayer1 mPaddlePosPlayer1;
	VectorPlayer2 mPaddlePosPlayer2;
	VectorBallPos mBallPos;
	VectorBallDir mBallDir;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Surface* fond;
	TTF_Font* Font;
	SDL_Surface* TextSurface;
	SDL_Rect DstRect;
	SDL_Color TextColor;
};
