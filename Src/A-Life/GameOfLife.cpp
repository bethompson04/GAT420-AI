#include "GameOfLife.h"
#include "Random.h"
#include <algorithm>
#include "Texture.h"
#include <iostream>

bool GameOfLife::Initialize()
{
	bufferA.resize(size.x * size.y);
	bufferB.resize(size.x * size.y);
	
	return true;
}

void GameOfLife::Step()
{
	frame++;
	std::vector<uint8_t>& readBuffer = (frame % 2) ? bufferA : bufferB;

	std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;

	// Update Buffer
	// Clear Write Buffer
	std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {

			int weight = 0;

			// read surrounding 8 cells, accumulate weight
			/*
			* XXX
			* XOX
			* XXX
			*/

			weight += Read<uint8_t>(readBuffer, x, y - 1);
			weight += Read<uint8_t>(readBuffer, x, y + 1);
			weight += Read<uint8_t>(readBuffer, x - 1, y);
			weight += Read<uint8_t>(readBuffer, x + 1, y);

			weight += Read<uint8_t>(readBuffer, x + 1, y + 1);
			weight += Read<uint8_t>(readBuffer, x + 1, y - 1);
			weight += Read<uint8_t>(readBuffer, x - 1, y + 1);
			weight += Read<uint8_t>(readBuffer, x - 1, y - 1);
			//weight += Read<uint8_t>(readBuffer, x, y);


			// Game of Life Rules
			// If cell is alive, update
			if (Read<uint8_t>(readBuffer, x, y) == 1) {
				if (weight < 2) Write<uint8_t>(writeBuffer, x, y, 0);
				else if (weight > 3) Write<uint8_t>(writeBuffer, x, y, 0);
				else if (weight == 2 || weight == 3) Write<uint8_t>(writeBuffer, x, y, 1);
			}
			else {
				// If cell is dead, update
				if (weight == 3) Write<uint8_t>(writeBuffer, x, y, 1);
			}
		}
	}

	// Convert buffer data format to color buffer
	std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
		{
			{ return v ? 0xffffffff : 0; }
		});

	// Copy color buffer to texture
	texture->Copy(color_buffer);
}

void GameOfLife::KeyDown(SDL_Keycode keycode)
{
	if (keycode == SDLK_r)
	{
		// Write random 'Alive' cells to buffer

		std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
		for (int i = 0; i < writeBuffer.size(); i++)
		{
			writeBuffer[i] = (random(0, 10) == 0) ? 1 : 0;
		}
	}
}
