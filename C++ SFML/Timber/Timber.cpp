#include "pch.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	// Create a video mode object
	VideoMode vm(1600, 900);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	// Create a sprite
	Sprite spriteBackground;
	// Attach texture to sprite
	spriteBackground.setTexture(textureBackground);
	spriteBackground.scale(0.833333f, 0.833333f);
	// Set sprite background to cover screen;
	spriteBackground.setPosition(0, 0);

	// Add tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.scale(0.833333f, 0.833333f);
	spriteTree.setPosition(675, 0);

	// Add bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.scale(0.833333f, 0.833333f);
	spriteBee.setPosition(0, 640);
	bool beeActive = 0;
	float beeSpeed = 0.0f;

	// Add clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1, spriteCloud2, spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.scale(0.833333f, 0.833333f);
	spriteCloud2.scale(0.833333f, 0.833333f);
	spriteCloud3.scale(0.833333f, 0.833333f);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 120);
	spriteCloud3.setPosition(0, 240);
	bool cloud1Active = 0, cloud2Active = 0, cloud3Active = 0;
	float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;

	// SCORE
	int score = 0;

	// TEXT
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text messageText, scoreText, replayText;
	messageText.setFont(font);
	messageText.setString("Press ENTER To Start!");
	messageText.setCharacterSize(60);
	messageText.setFillColor(Color::White);
	scoreText.setFont(font);
	scoreText.setString("Score = 0");
	scoreText.setCharacterSize(80);
	scoreText.setFillColor(Color::White);
	replayText.setFont(font);
	replayText.setString("Press ENTER to play again!");
	replayText.setCharacterSize(40);
	replayText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1600 / 2.0f, 900 / 2.0f);
	scoreText.setPosition(16, 16);

	// Control time
	Clock clock;

	RectangleShape timeBar;
	float timeBarStartWidth = 320, timeBarHeight = 64;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1600 / 2) - timeBarStartWidth / 2, 784);
	
	Time gameTimeTotal;
	float timeRemaining = 5.0f, timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Game is Running?!?!?!
	bool paused = 1;

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) 
	{
		branches[i].setTexture(textureBranch);
		branches[i].scale(0.8333333f, 0.833333333f);
		branches[i].setPosition(-1600, -1600);
		branches[i].setOrigin(176, 16);
	}
	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);


	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.scale(0.833333f, 0.833333333f);
	spritePlayer.setPosition(483, 600);
	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.scale(0.833333f, 0.833333333f);
	spriteRIP.setPosition(500, 717);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.scale(0.833333f, 0.833333333f);
	spriteAxe.setPosition(583, 692);
	const float AXE_POSITION_LEFT = 583, AXE_POSITION_RIGHT = 896;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.scale(0.833333f, 0.833333333f);
	spriteLog.setPosition(675, 600);
	bool logActive = 0;
	float logSpeedX = 1000, logSpeedY = -1500;

	// SOUND
	SoundBuffer chopBuffer, deathBuffer, ootBuffer;
	chopBuffer.loadFromFile("sounds/chop.wav");
	deathBuffer.loadFromFile("sounds/death.wav");
	ootBuffer.loadFromFile("sounds/out_of_time.wav");
	Sound chop, death, oot;	
	chop.setBuffer(chopBuffer);
	death.setBuffer(deathBuffer);
	oot.setBuffer(ootBuffer);

	bool acceptInput = 0, replay = 0;

	while (window.isOpen())
	{
		// Handle Input

		Event event;
		while(window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = 1;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = 0;

			score = 0;
			timeRemaining = 5.0f;

			for(int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			spriteRIP.setPosition(675, 2000);
			spritePlayer.setPosition(483, 600);
			acceptInput = 1;
			replay = 0;
		}

		if (acceptInput) 
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (1.0f / score) + 0.10;
				
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				
				spritePlayer.setPosition(1000, 600);
				
				updateBranches(score);

				spriteLog.setPosition(675, 600);
				logSpeedX = -5000;
				logActive = 1;

				acceptInput = 0;

				chop.play();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (1.0f / score) + 0.10;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(483, 600);

				updateBranches(score);

				spriteLog.setPosition(675, 600);
				logSpeedX = 5000;
				logActive = 1;

				acceptInput = 0;

				chop.play();
			}
		}

		// Update scene
		if (!paused) {
			// Measure Time
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			// Time Ran Out?
			if (timeRemaining <= 0.0f)
			{
				paused = 1;

				messageText.setString("Out Of Time!");

				FloatRect textRect = messageText.getLocalBounds(), textRect2 = replayText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(800, 450);
				replayText.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
				replayText.setPosition(800, 450 + textRect.height / 2.0f + 20);
				acceptInput = 0;
				replay = 1;
				oot.play();
			}

			// BEE
			if (!beeActive)
			{
				// How fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 400) + 300;
				spriteBee.setPosition(1650, height);
				beeActive = 1;
			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = 0;
				}
			}

			// CLOUD 1
			if (!cloud1Active)
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 150) + 50;

				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = 1;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1600)
				{
					cloud1Active = 0;
				}
			}

			// CLOUD 2
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 150) + 50;

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = 1;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1600)
				{
					cloud2Active = 0;
				}
			}

			// CLOUD 3
			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 150) + 50;

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = 1;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1600)
				{
					cloud3Active = 0;
				}
			}

			// LOG
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 1650) 
				{
					logActive = 0;
					spriteLog.setPosition(675, 600);
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = 1;
				acceptInput = 0;

				spriteRIP.setPosition(437.5f, 633);
				spritePlayer.setPosition(2000, 600);
				
				messageText.setString("SQUISHED!!!");
				FloatRect textRect = messageText.getLocalBounds(), textRect2 = replayText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(800, 450);
				replayText.setOrigin(textRect2.left + textRect2.width / 2.0f, textRect2.top + textRect2.height / 2.0f);
				replayText.setPosition(800, 450 + textRect.height / 2.0f + 20);
				replay = 1;
				death.play();
			}

			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Position Branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 125;
				switch (branchPositions[i])
				{
				case side::LEFT:
					branches[i].setPosition(528, height);
					branches[i].setRotation(180);
					break;
				case side::RIGHT:
					branches[i].setPosition(1072, height);
					branches[i].setRotation(0);
					break;
				default:
					branches[i].setPosition(3000, height);
					break;

				}
			}
		}
		// Draw scene

		window.clear();

		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);
		for(int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteBee);

		window.draw(spritePlayer);
		window.draw(spriteRIP);
		window.draw(spriteAxe);
		window.draw(spriteLog);

		window.draw(timeBar);

		window.draw(scoreText);
		if(paused)
		{
			window.draw(messageText);
		}
		if(replay)
		{
			window.draw(replayText);
		}

		window.display();
	}

	return 0;
}

void updateBranches(int seed)
{
	for(int i = NUM_BRANCHES - 1; i > 0; i--)
	{
		branchPositions[i] = branchPositions[i - 1];
	}
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}