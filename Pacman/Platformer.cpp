#include "Platformer.h"
#include <sstream>
#include <time.h>
Platformer::Platformer(int argc, char* argv[]) : Game(argc, argv) 
{
}

Platformer::~Platformer()
{
}


void Platformer::LoadContent()
{
}

void Platformer::Update(int elapsedTime)
{
}

void Platformer::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();
	SpriteBatch::EndDraw(); // Ends Drawing
}