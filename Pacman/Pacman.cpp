#include "Pacman.h"
#include <sstream>
#include <time.h>
#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.30f), _cPacmanFrameTime(250), _cCherryFrameTime(500), _cGhostFrameTime(250), _cMunchieFrameTime(500), _cRandomCherry(500), _cRandomMunchie(500), _cStartMenuFrameTime(500), _cStarFrameTime(500)

{
	srand(time(NULL));

	//Pacman Values
	_pacman = new Player();
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->dead = false;
	_pacman->immune = false;
	_pacman->immuneTime = 0.0f;

	// Cherry Values
	Pickup _cherry[CHERRYCOUNT];
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i].frameCount = 0;
		_cherry[i].currentFrameTime = 0;
		_cherry[i].frame = 0;
		_cherry[i].randomFrameCount = 0;
		_cherry[i].keyDown = false;
	}

	//Munchie Values
	Pickup _munchie[MUNCHIECOUNT];
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		//Pickup _munchie[i];
		_munchie[i].frameCount = 0;
		_munchie[i].currentFrameTime = 0;
		_munchie[i].frame = 0;
		_munchie[i].randomFrameCount = 0;
		_munchie[i].keyDown = false;
	}

	// Star Values
	_star = new Pickup();
	_star->frameCount = 0;
	_star->currentFrameTime = 0;
	_star->frame = 0;
	_star->spawnVisible = true;
	_star->keyDown = false;

	//Start Screen 
	_start = new UserInterface();
	_start->gameState = false;
	_start->currentFrameTime = 0;
	_start->frame = 0;

	//Pause Menu
	_pause = new UserInterface();

	//On-Screen Text
	_coordinates = new UserInterface();
	_deathScreen = new UserInterface();
	_deathText = new UserInterface();
	_score = new UserInterface();
	_score->total = 0;

	//Ghost Values
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.20f;
		_ghosts[i]->frameCount = 0;
		_ghosts[i]->currentFrameTime = 0;
		_ghosts[i]->frame = 0;
	}

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

// Decontructor, Memory ManagementS
Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;

	delete _cherry[0].texture;
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		delete _cherry[i].sourceRect;
		delete _cherry[i].position;
		delete &_cherry[i];
	}

	delete _munchie[0].texture;
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie[i].sourceRect;
		delete _munchie[i].position;
		delete &_munchie[i];
	}

	delete _star->texture;
	delete _star->sourceRect;
	delete _star->position;
	delete _star;

	delete _ghosts[0]->texture;
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->sourceRect;
		delete _ghosts[i]->position;
		delete _ghosts[i];
	}

	delete _coordinates->position;
	delete _coordinates;

	delete _start->background;
	delete _start->rectangle;
	delete _start->position;
	delete _start;

	delete _pause->background;
	delete _pause->rectangle;
	delete _pause->position;
	delete _pause;

	delete _deathScreen->background;
	delete _deathScreen->rectangle;
	delete _deathScreen->position;
	delete _deathScreen;

	delete _deathText;

	delete munchieCollect;
	delete deathSound;
	delete inGameSound;
	delete introMusic;

}

// Loads Content for the game, Sprites Audio etc
void Pacman::LoadContent()
{
	//Initialising Audio
	munchieCollect = new SoundEffect();
	munchieCollect->Load("Sounds/MunchieCollect.wav");

	deathSound = new SoundEffect();
	deathSound->Load("Sounds/Death.wav");

	inGameSound = new SoundEffect();
	inGameSound->Load("Sounds/InGameSound.wav");

	introMusic = new SoundEffect();
	introMusic->Load("Sounds/Intro.wav");

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman_v3.png", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Cherry
	Texture2D* cherryTexture = new Texture2D();
	cherryTexture->Load("Textures/CherryMainSprite.png", false);
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i].texture = cherryTexture;
		_cherry[i].position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_cherry[i].sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	// Load Munchie
	Texture2D* munchieTexture = new Texture2D();
	munchieTexture->Load("Textures/MunchieSprite.png", false);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i].texture = munchieTexture;
		_munchie[i].position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_munchie[i].sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}

	// Load Star
	_star->texture = new Texture2D();
	_star->texture->Load("Textures/Star.png", false);
	_star->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	_star->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Ghosts
	Texture2D* ghostTexture = new Texture2D();
	ghostTexture->Load("Textures/Ghost_Sprite.png", false);
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = ghostTexture;
		_ghosts[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	// Set string position
	_coordinates->position = new Vector2(10.0f, 25.0f);
	_score->position = new Vector2(512.0f, 25.0f);
	_deathText->position = new Vector2(450.0f, 300.0f);

	// Set Menu Parameters
	_pause->background = new Texture2D();
	_pause->background->Load("Textures/Transparency.png", false);
	_pause->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_pause->position = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Set Start Menu parameters
	_start->background = new Texture2D();
	_start->background->Load("Textures/Pacman_Title_Screen.png", false);
	_start->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_start->position = new Vector2(0.0f, 0.0f);

	//Set Death Screen Parameteres
	_deathScreen->background = new Texture2D();
	_deathScreen->background->Load("Textures/Transparency.png", false);
	_deathScreen->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_deathScreen->position = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

}

// Update Method for Game during each frame
void Pacman::Update(int elapsedTime)
{
	Audio::Play(introMusic);
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	UpdateStartMenu(elapsedTime);
	GameStart(keyboardState, Input::Keys::SPACE);
	CheckPaused(keyboardState, Input::Keys::P);

	// Will run when activly playing the game
	if (!_pause->gameState && _start->gameState && !_pacman->dead)
	{
		Audio::Stop(introMusic);
		Input(elapsedTime, keyboardState, mouseState);
		UpdatePacman(elapsedTime);
		UpdateMunchie(_munchie[MUNCHIECOUNT], elapsedTime);
		UpdateCherry(_cherry[CHERRYCOUNT], elapsedTime);
		UpdateStar(elapsedTime);
		CheckViewportCollision();
		UpdateGhosts(elapsedTime, _pacman->position->X, _pacman->position->Y);
		
		// Check for Ghost Collision
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height) && !_pacman->immune)
			{
				Audio::Stop(inGameSound);
				Audio::Play(deathSound);
				for (int i = 0; i < GHOSTCOUNT; i++)
				{
					_ghosts[i]->position->X = rand() % Graphics::GetViewportWidth();
					_ghosts[i]->position->Y = rand() % Graphics::GetViewportHeight();
				}
				_pacman->dead = true;
			}
		}

		// Check for Munchie Collision
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchie[i].position->X, _munchie[i].position->Y, _munchie[i].sourceRect->Width, _munchie[i].sourceRect->Height))
			{
				Audio::Play(munchieCollect);
				_score->total += 5;
				_munchie[i].position->X = rand() % Graphics::GetViewportWidth();
				_munchie[i].position->Y = rand() % Graphics::GetViewportHeight();
			}
		}

		// Check for Cherry Collision
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry[i].position->X, _cherry[i].position->Y, _cherry[i].sourceRect->Width, _cherry[i].sourceRect->Height))
			{
				Audio::Play(munchieCollect);
				_score->total += 10;
				_cherry[i].position->X = rand() % Graphics::GetViewportWidth();
				_cherry[i].position->Y = rand() % Graphics::GetViewportHeight();
			}
		}

		// Check for Star Collision
		if (CheckCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _star->position->X, _star->position->Y, _star->sourceRect->Width, _star->sourceRect->Height) && !_pacman->immune)
		{
			_pacman->immune = true;
			_pacman->immuneTime += 200;
			_star->position->X = rand() % Graphics::GetViewportWidth();
			_star->position->Y = rand() %
			Graphics::GetViewportHeight();
			
		}
		if (_pacman->immune)
		{
			UpdatePacmanImmune();
		}
		if (_score->total % 100 == 0 && _score->total != 0 && _cPacmanSpeed > _ghosts[GHOSTCOUNT]->speed) 
		{
			for (int i = 0; i < GHOSTCOUNT; i++)
			{
				_ghosts[i]->speed += 0.05f;

			}
			_pacman->speedMultiplier += 0.4f;
			_score->total += 5;
		}
	}

	// Will initiate Enter key to be used when dead to revert dead gamestate
	if (_pacman->dead)
	{
		GameStart(keyboardState, Input::Keys::RETURN);
		_pacman->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}
}

// Control the start of the game
void Pacman::GameStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (!_start->gameState)
	{
		if (state->IsKeyDown(Input::Keys::SPACE) && !_start->keyDown)
		{
			Audio::Play(inGameSound);
			_start->keyDown = true;
			_start->gameState = true;
			_score->total = 0;
		}
	}

	if (_pacman->dead && state->IsKeyDown(Input::Keys::RETURN))
	{
		_pacman->dead = false;
		_start->gameState = false;
		_start->keyDown = false;
		Audio::Stop(deathSound);
	}
}

// Controls Player input for pacman
void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float _pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	//Moves Pacman across X axis, Right
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += _pacmanSpeed;
		_pacman->direction = 0;
	}

	//Move Pacman across X axis, Left
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= _pacmanSpeed;
		_pacman->direction = 2;
	}

	//Move Pacman across Y axis, Up
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= _pacmanSpeed;
		_pacman->direction = 3;
	}

	//Move Pacman across Y axis, down
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += _pacmanSpeed;
		_pacman->direction = 1;
	}

	// Randomise Cherry Position
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i].randomFrameCount += elapsedTime;
		if (_cherry[i].randomFrameCount >= _cRandomCherry)
		{
			if (state->IsKeyDown(Input::Keys::R) && !_cherry[i].keyDown && _score->total >= 10)
			{

				_cherry[i].position->X = rand() % Graphics::GetViewportWidth();
				_cherry[i].position->Y = rand() % Graphics::GetViewportHeight();
				_cherry[i].keyDown = true;
				_cherry[i].randomFrameCount = 0;
				_score->total -= 10;
			}
			else if (state->IsKeyUp(Input::Keys::R) && _cherry[i].keyDown)
			{
				_cherry[i].keyDown = false;
			}
		}
	}

	// Randomise Munchie Position
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i].randomFrameCount += elapsedTime;
		if (_munchie[i].randomFrameCount >= _cRandomMunchie)
		{

			if (state->IsKeyDown(Input::Keys::T) && !_munchie[i].keyDown && _score->total >= 2)
			{
				_munchie[i].position->X = rand() % Graphics::GetViewportWidth();
				_munchie[i].position->Y = rand() % Graphics::GetViewportHeight();
				_munchie[i].keyDown = true;
				_munchie[i].randomFrameCount = 0;
				_score->total -= 2;
			}
			else if (state->IsKeyUp(Input::Keys::T) && _munchie[i].keyDown)
			{
				_munchie[i].keyDown = false;
			}
		}
	}

	
	if (mouseState->LeftButton == Input::ButtonState::PRESSED && !_star->keyDown)
	{
		if (_score->total >= 35) 
		{
			_star->keyDown = true;
			_star->spawnVisible = true;
			_star->position->X = mouseState->X;
			_star->position->Y = mouseState->Y;
			_score->total -= 35;
		}
		
	}
	else if (mouseState->LeftButton == Input::ButtonState::RELEASED)
	{
		_star->keyDown = false;
	}


	// Speed boost Pacman
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT) && _score->total >= 1)
	{
		_score->total -= 0.1f;
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		_pacman->speedMultiplier = 1.0f;
	}
}

// Deals with the immunity of Pacman after collecting a star
void Pacman::UpdatePacmanImmune()
{
	_pacman->immuneTime -= 0.5f;
	if (_pacman->immuneTime == 0)
		_pacman->immune = false;
}

// Method to control Pause Game State
void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_pause->keyDown)
	{
		_pause->keyDown = true;
		_pause->gameState = !_pause->gameState;
	}
	else if (state->IsKeyUp(Input::Keys::P))
	{
		_pause->keyDown = false;
	}
}

// Method to Wrap Pacman around the walls
void Pacman::CheckViewportCollision()
{
	if (_pacman->position->X > Graphics::GetViewportWidth())
	{
		_pacman->position->X = 0;
	}
	else if (_pacman->position->X < 0)
	{
		_pacman->position->X = 1023;
	}
	else if (_pacman->position->Y > Graphics::GetViewportHeight())
	{
		_pacman->position->Y = 0;
	}
	else if (_pacman->position->Y < 0)
	{
		_pacman->position->Y = 767;
	}
}

// Method to control Pacman Updates
void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		if (!_pacman->immune) {
			_pacman->frame++;
			if (_pacman->frame >= 2)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
		}
		else if (_pacman->immune)
		{
			_pacman->frame++;
			if (_pacman->frame == 0 || _pacman->frame == 1)
				_pacman->frame += 2;
			if (_pacman->frame >= 4)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
		}
	}
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
}

// Method to control Cherry Updates
void Pacman::UpdateCherry(Pickup&, int elapsedTime)
{
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i].currentFrameTime += elapsedTime;
		if (_cherry[i].currentFrameTime > _cCherryFrameTime)
		{
			_cherry[i].frameCount++;
			if (_cherry[i].frameCount >= 2)
				_cherry[i].frameCount = 0;
			_cherry[i].currentFrameTime = 0;
		}
		_cherry[i].sourceRect->X = _cherry[i].sourceRect->Width * _cherry[i].frameCount;
	}
}

// Method to Control Munchie Updates
void Pacman::UpdateMunchie(Pickup&, int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i].currentFrameTime += elapsedTime;
		if (_munchie[i].currentFrameTime > _cMunchieFrameTime)
		{
			_munchie[i].frameCount++;
			if (_munchie[i].frameCount >= 2)
				_munchie[i].frameCount = 0;
			_munchie[i].currentFrameTime = 0;
		}
		_munchie[i].sourceRect->X = _munchie[i].sourceRect->Width * _munchie[i].frameCount;
	}

}

// Method to Control Start Menu Updates
void Pacman::UpdateStartMenu(int elapsedTime)
{
	_start->currentFrameTime += elapsedTime;
	if (_start->currentFrameTime > _cStartMenuFrameTime)
	{
		_start->frame++;
		if (_start->frame >= 2)
			_start->frame = 0;
		_start->currentFrameTime = 0;
	}
	_start->rectangle->X = _start->rectangle->Width* _start->frame;
}

void Pacman::UpdateStar(int elapsedTime)
{
	_star->currentFrameTime += elapsedTime;
	if (_star->currentFrameTime > _cStarFrameTime)
	{
		_star->frame++;
		if (_star->frame >= 2)
			_star->frame = 0;
		_star->currentFrameTime = 0;
	}
	_star->sourceRect->X = _star->sourceRect->Width* _star->frame;
}
// Method to control Ghost Updates
void Pacman::UpdateGhosts(int elapsedTime, float pacmanX, float pacmanY)
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (pacmanX < _ghosts[i]->position->X)
		{
			_ghosts[i]->position->X -= 1;
		}
		if (pacmanX > _ghosts[i]->position->X)
		{
			_ghosts[i]->position->X += 1;
		}
		if (pacmanY < _ghosts[i]->position->Y)
		{
			_ghosts[i]->position->Y -= 1;
		}
		if (pacmanY > _ghosts[i]->position->Y)
		{
			_ghosts[i]->position->Y += 1;
		}
		if (CheckCollision(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height, _ghosts[i - 1]->position->X, _ghosts[i - 1]->position->Y, _ghosts[i - 1]->sourceRect->Width, _ghosts[i - 1]->sourceRect->Height))
		{
			_ghosts[i]->position->X += 5;
			_ghosts[i]->position->Y += 5;
		}
		if (_ghosts[i]->direction == 0)
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime; // Moves Right
		}
		else if (_ghosts[i]->direction == 1)
		{
			_ghosts[i]->position->X -= _ghosts[i]->speed * elapsedTime; // Moves Left
		}
		if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits right edge
		{
			_ghosts[i]->direction = 1;
		}
		else if (_ghosts[i]->position->X <= 0) // Hits Left Edge
		{
			_ghosts[i]->direction = 0;
		}
	}
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->currentFrameTime += elapsedTime;
		if (_ghosts[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_ghosts[i]->frameCount++;
			if (_ghosts[i]->frameCount >= 2)
				_ghosts[i]->frameCount = 0;
			_ghosts[i]->currentFrameTime = 0;
		}
		_ghosts[i]->sourceRect->X = _ghosts[i]->sourceRect->Width * _ghosts[i]->frameCount;
	}
}

// Collision Check
bool Pacman::CheckCollision(float pacmanX, float pacmanY, float pacmanWidth, float pacmanHeight, float colliderX, float colliderY, float colliderWidth, float colliderHeight)
{
	if ((pacmanY + pacmanHeight) < colliderY)
		return false;
	if (pacmanY > (colliderY + colliderHeight))
		return false;
	if ((pacmanX + pacmanWidth) < colliderX)
		return false;
	if (pacmanX > (colliderX + colliderWidth))
		return false;

	return true;
}

// Controlling Drawing Sprites and Texts onto Screen
void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();
	// Draws Elements when pacman alive
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

		// Draws Score Count
		std::stringstream scoreStream;
		scoreStream << "Score: " << _score->total;
		SpriteBatch::DrawString(scoreStream.str().c_str(), _score->position, Color::Red);

		//Draws the Cherry 
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			SpriteBatch::Draw(_cherry[i].texture, _cherry[i].position, _cherry[i].sourceRect);
		}

		//Draws the munchie
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchie[i].texture, _munchie[i].position, _munchie[i].sourceRect);
		}

		//Draws Ghost
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
		}

		if (_star->spawnVisible = true)
			SpriteBatch::Draw(_star->texture, _star->position, _star->sourceRect);

		// Draws on screen Coorindates
		std::stringstream coordinates;
		coordinates << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
		SpriteBatch::DrawString(coordinates.str().c_str(), _coordinates->position, Color::Green);
	}
	// Will only draw once pacman is dead
	else
	{
		std::stringstream deadStream;
		deadStream << "You have died!\nYou Scored: " << _score->total << "\nPress Enter To Continue";
		SpriteBatch::Draw(_deathScreen->background, _deathScreen->position, _deathScreen->rectangle);
		SpriteBatch::DrawString(deadStream.str().c_str(), _deathText->position, Color::Green);
	}
	// Will only draw if game paused
	if (_pause->gameState)
	{
		std::stringstream menuStream;
		menuStream << "Game Paused!";
		SpriteBatch::Draw(_pause->background, _pause->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pause->position, Color::Red);
	}

	// Will only draw when game not started
	if (!_start->gameState)
	{
		SpriteBatch::Draw(_start->background, _start->position, _start->rectangle);
	}


	// Ends Drawing
	SpriteBatch::EndDraw();
}