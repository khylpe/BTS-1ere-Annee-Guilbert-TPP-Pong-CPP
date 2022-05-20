#include "game.h"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL_ttf.h>
#include <string>

using namespace std;

game::game()
	: mWindow(nullptr),
	mRenderer(nullptr),
	mIsRunning(true),
	mPaddlePosPlayer1{ 30.0, 600.0 },
	mBallPos{ 1920 / 2, 1080 / 2 },
	mPaddlePosPlayer2{ 1920.0 - 30.0, 600.0 },
	mPaddleDirPlayer1{ 1.0 },
	mPaddleDirPlayer2{ 1.0 },
	randomForDir{ 40 },
	mBallDir{ (rand() % 10 + 0)-5, (rand() % 10 + 0)-5 },  //https://stackoverflow.com/questions/3830663/random-and-negative-numbers  used this link for negatives/positifs + randoms numbers
	widthPaddles{ 10 },
	heightPaddles{ 100 },
	accelerationPaddle{ 6.0 },
	countInt{ 0 },
	count{ "0" },
	DstRect{ 0 },
	TextSurface{},
	TextColor{ 0 },
	Message{},
	Font{}
{
}

bool game::Initialize()
{
	// Initialise SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Impossible d�initialiser SDL: %s", SDL_GetError());
		return false;
	}
	// Cree une fenetre SDL
	mWindow = SDL_CreateWindow(
		"Premier jeu", // titre de la fenetre
		0,             // Coordonnees X du coin en haut a gauche de la fenetre
		0,             // Coordonnees Y du coin en haut a gauche de la fenetre
		1920,          // Largeur de la fenetre
		1080,          // Hauteur de la fenetre
		0              // Flags (0 flag non positionne)
	);

	if (!mWindow)
	{
		SDL_Log("Impossible de creer une fenetre: %s", SDL_GetError());
		return false;
	}
	//// Cree un renderer SDL
	mRenderer = SDL_CreateRenderer(
		mWindow, // Fenetre pour laquelle on cree le renderer
		-1,      // Generalement -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("impossible de creer le renderer: %s", SDL_GetError());
		return false;
	}

	/* Initialisation de SDL_ttf */
	// https://jeux.developpez.com/faq/sdl/?page=texte#:~:text=Lien%20%3A%20Site%20officiel-,Comment%20afficher%20du%20texte%20avec%20SDL_ttf%20%3F,pour%20en%20faire%20une%20SDL_Surface.
	// https://openclassrooms.com/forum/sujet/sdl2-sdl-blitsurface
	// http://www.resoo.org/docs/c/c/Partie%203/Ecrire%20du%20texte%20avec%20SDL_ttf%20-%20Programmation%20-%20Lecture%20du%20tu.pdf
	// https://www.libsdl.org/projects/SDL_ttf/release/?C=M;O=D
	// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int ttfResult = TTF_Init();

	if (ttfResult < 0) { // Check if TTF was well-initialized
		printf("Impossible d'initialiser SDL_ttf : %s", TTF_GetError());
		return false;
	}

	Font = TTF_OpenFont("calibri.ttf", 50); //Select font family and font size

	if (!Font) { //Check is  there was an issue with the font family and font size
		printf("Erreur de cr�ation de la police : %s", TTF_GetError());
		return false;
	}

	// Text color :
	TextColor.r = 255;
	TextColor.g = 255;
	TextColor.b = 255;

	SDL_Init(SDL_INIT_VIDEO);

	TextSurface = TTF_RenderText_Solid(TTF_OpenFont("calibri.ttf", 50), count, TextColor);

	if (!TextSurface){
		printf("Erreur de rendu du texte : %s", TTF_GetError());
		return false;
	}

	DstRect.x = (1920 / 2) - ((TextSurface->w) / 2);
	DstRect.y = 0;
	DstRect.w = TextSurface->w;
	DstRect.h = TextSurface->h;

	return true; // no issue
}

void game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void game::ProcessInput() {

	SDL_Event event;

	while (SDL_PollEvent(&event))// Tant qu'il y a des event enregistres
	{
		// On traite chaque event a l'aide d'un switch case
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_ESCAPE]) // When esc is pressed
		mIsRunning = false;

	mPaddleDirPlayer1 = 0.0;
	mPaddleDirPlayer2 = 0.0;

	if (state[SDL_SCANCODE_W] && mPaddlePosPlayer1.y > 0) // When W (Z on azerty keyboards) is pressed and left paddle is not out of screen
		mPaddleDirPlayer1 -= accelerationPaddle;

	if (state[SDL_SCANCODE_S] && mPaddlePosPlayer1.y < 1080 - 100) // When S is pressed and left paddle is not out of screen
		mPaddleDirPlayer1 += accelerationPaddle;

	if (state[SDL_SCANCODE_UP] && mPaddlePosPlayer2.y > 0) // When UP arrow is pressed and right paddle is not out of screen
		mPaddleDirPlayer2 -= accelerationPaddle;

	if (state[SDL_SCANCODE_DOWN] && mPaddlePosPlayer2.y < 1080 - 100) // When DOWN arrow is pressed and right paddle is not out of screen
		mPaddleDirPlayer2 += accelerationPaddle;
}

void game::UpdateGame() {

	posAndWidthPlayer1 = mPaddlePosPlayer1.x + widthPaddles;
	posAndWidthPlayer2 = mPaddlePosPlayer2.x + widthPaddles;

	posAndHeightPlayer1 = mPaddlePosPlayer1.y + heightPaddles;
	posAndHeightPlayer2 = mPaddlePosPlayer2.y + heightPaddles;

	if (mBallPos.x <= 0 or mBallPos.x >= 1920) {
		if (mBallPos.x <= 0)
			cout << "Le joueur rouge a gagn�" << '\n';

		if (mBallPos.x >= 1920)
			cout << "Le joueur vert a gagn�" << '\n';
		mIsRunning = false;
	}

	else {
		if ((mBallPos.y <= 0) or (mBallPos.y >= 1080) or ((mBallPos.y > mPaddlePosPlayer1.y && mBallPos.y < posAndHeightPlayer1) && (mBallPos.x <= mPaddlePosPlayer1.x + widthPaddles)) or ((mBallPos.y > mPaddlePosPlayer2.y && mBallPos.y < posAndHeightPlayer2) && (mBallPos.x >= mPaddlePosPlayer2.x - widthPaddles))) {
			if (mBallPos.y <= 0 or mBallPos.y >= 1080) {
				if (mBallPos.y <= 0) {
					mBallPos.y = 1;
					mBallDir.y *= (-1);
				}

				if (mBallPos.y >= 1080) {
					mBallPos.y = 1079;
					mBallDir.y *= (-1);
				}
			}

			if ((mBallPos.y > mPaddlePosPlayer1.y && mBallPos.y < posAndHeightPlayer1) && (mBallPos.x <= mPaddlePosPlayer1.x + widthPaddles)) {
				mBallPos.x = 41;

				if (mBallDir.x <= 10) {
					countInt++;

					// https://www.cplusplus.com/reference/string/to_string/
					// https://prograide.com/pregunta/52144/Comment-convertir-int-en-char

					std::string countString = std::to_string(countInt);

					count = countString.c_str(); //toChar  
					TextSurface = TTF_RenderText_Solid(TTF_OpenFont("calibri.ttf", 50), count, TextColor);

					mBallDir.x *= (-1.5);
					mBallDir.y *= 1.5;
				}

				else
					mBallDir.x *= (-1);

				if (accelerationPaddle <= 10)
					accelerationPaddle += 0.6;
			}

			if ((mBallPos.y > mPaddlePosPlayer2.y && mBallPos.y < posAndHeightPlayer2) && (mBallPos.x >= mPaddlePosPlayer2.x - widthPaddles)) {
				mBallPos.x = 1920 - 41;
				if (mBallDir.x <= 10) {

					countInt++;
					std::string countString = std::to_string(countInt);
					count = countString.c_str(); //toChar
					TextSurface = TTF_RenderText_Solid(TTF_OpenFont("calibri.ttf", 50), count, TextColor);
					mBallDir.x *= (-1.5);
					mBallDir.y *= 1.5;
				}
				else
					mBallDir.x *= (-1);
				if (accelerationPaddle <= 10)
					accelerationPaddle += 0.6;
			}
		}
		else {
			mBallPos.x += mBallDir.x;
			mBallPos.y += mBallDir.y;
			mPaddlePosPlayer1.y += mPaddleDirPlayer1;
			mPaddlePosPlayer2.y += mPaddleDirPlayer2;
		}


	}
	cout << countInt;
}

void game::GenerateOutput() {

	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		0,
		0);

	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	SDL_Rect Paddle1{ mPaddlePosPlayer1.x, mPaddlePosPlayer1.y, widthPaddles, heightPaddles };
	SDL_RenderFillRect(mRenderer, &Paddle1);

	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
	SDL_Rect Paddle2{ mPaddlePosPlayer2.x, mPaddlePosPlayer2.y, widthPaddles, heightPaddles };
	SDL_RenderFillRect(mRenderer, &Paddle2);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect ball{ mBallPos.x, mBallPos.y, 10, 10 };
	SDL_RenderFillRect(mRenderer, &ball);

	Message = SDL_CreateTextureFromSurface(mRenderer, TextSurface);
	SDL_RenderCopy(mRenderer, Message, NULL, &DstRect);

	SDL_RenderPresent(mRenderer);
}

void game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	TTF_CloseFont(Font);
	TTF_Quit();
}