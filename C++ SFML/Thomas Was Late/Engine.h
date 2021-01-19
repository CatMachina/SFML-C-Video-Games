#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Thomas.h"
#include "Bob.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "HUD.h"
#include "ParticleSystem.h"

using namespace sf;

class Engine
{
private:
	TextureHolder th;
	
	ParticleSystem m_PS;

	Thomas m_Thomas;
	Bob m_Bob;

	LevelManager m_LM;
	SoundManager m_SM;

	HUD m_HUD;
	int m_FramesSinceLastHUDUpdate = 0;
	int m_TargetFramesPerHUDUpdate = 500;

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;
	const int GRAVITY = 300;

	RenderWindow m_Window;

	View m_MainView, m_LeftView, m_RightView;
	View m_BGMainView, m_BGLeftView, m_BGRightView;
	View m_HudView;

	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;

	Shader m_RippleShader;

	bool m_Playing = 0;
	bool m_Character1 = 1;
	bool m_SplitScreen = 0;

	float m_TimeRemaining = 10; 
	Time m_GameTimeTotal;

	bool m_NewLevelRequired = 1;

	VertexArray m_VALevel;
	int** m_ArrayLevel = NULL;

	Texture m_TextureTiles;

	void input();
	void update(float dtAsSeconds);
	void draw();

	void loadLevel();

	bool detectCollisions(PlayableCharacter& character);

	void populateEmitters(vector<Vector2f>& vSoundEmitters, int** arrayLevel);
	vector<Vector2f> m_FireEmitters;

public:
	Engine();
	void run();

};