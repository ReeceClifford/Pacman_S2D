#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#define MUNCHIECOUNT 10
#define GHOSTCOUNT 4
#define CHERRYCOUNT 2
// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

struct Player
{ //Pacman Struct
	Vector2* position; 
	Rect* sourceRect; 
	Texture2D* texture; 
	int direction; 
	int frame; 
	int currentFrameTime; 
	float speedMultiplier; 
	bool dead;
	bool immune;
	float immuneTime;
};
struct Pickup
{//Struct For Munchies and Cherries
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int frame;
	int frameCount;
	int currentFrameTime;
	int randomFrameCount;
	bool keyDown;
	bool spawnVisible;
};
struct UserInterface
{//Struct For Menus and On Screen Text
	Texture2D* background;
	Rect* rectangle;
	Vector2* position;
	bool gameState;
	bool keyDown;
	int total;
	int currentFrameTime;
	int frame;
};

struct MovingEnemy
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int frameCount;
	int currentFrameTime;
	int randomFrameCount;
	float speed;
};

class Pacman : public Game
{
private:

	Player * _pacman;
	Pickup _cherry[CHERRYCOUNT];
	Pickup _munchie[MUNCHIECOUNT];
	Pickup * _star;
	MovingEnemy* _ghosts[GHOSTCOUNT];
	UserInterface * _start;
	UserInterface * _pause;
	UserInterface * _coordinates;
	UserInterface * _deathScreen;
	UserInterface * _score;
	UserInterface * _deathText;
	SoundEffect * deathSound;
	SoundEffect * munchieCollect;
	SoundEffect * inGameSound;
	SoundEffect * introMusic;
	

	//Constant data for game variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cCherryFrameTime;
	const int _cMunchieFrameTime;
	const int _cGhostFrameTime;
	const int _cRandomMunchie;
	const int _cRandomCherry;
	const int _cStartMenuFrameTime;
	const int _cStarFrameTime;

	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void CheckPaused(Input::KeyboardState* state, Input::Keys
		pauseKey); 
	void GameStart(Input::KeyboardState* state, Input::Keys startKey);
	void CheckViewportCollision();
	void UpdatePacman(int elapsedTime); 
	void UpdateCherry(Pickup& _cherry, int elapsedTime);
	void UpdateMunchie(Pickup& _munchie,int elapsedTime);
	void UpdateStar(int elpapsedTime);
	void UpdateGhosts(int elapsedTime, float pacmanX, float pacmanY);
	void UpdateStartMenu(int elapsedTime);
	void UpdatePacmanImmune();
	bool CheckCollision(float pacmanX, float pacmanY, float pacmanWidth, float pacmanHeight, float colliderX, float colliderY, float colliderWidth, float colliderHeight);
	

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};
