#include "pch.h"
#include "Engine.h"

using namespace sf;
using namespace std;

void Engine::populateEmitters(vector<Vector2f>& vSoundEmitters, int** arrayLevel)
{
	vSoundEmitters.clear();

	FloatRect previousEmitter;

	for (int x = 0; x < (int)m_LM.getLevelSize().x; x++)
	{
		for(int y = 0; y < (int)m_LM.getLevelSize().y; y++)
		{
			if (arrayLevel[y][x] == 2)
			{
				if (!FloatRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE).intersects(previousEmitter))
				{
					vSoundEmitters.push_back(Vector2f(x * TILE_SIZE, y * TILE_SIZE));
					previousEmitter.left = x * TILE_SIZE;
					previousEmitter.top = y * TILE_SIZE;
					previousEmitter.width = 6 * TILE_SIZE;
					previousEmitter.height = 6 * TILE_SIZE;
				}
			}
		}
	}
	return;
}