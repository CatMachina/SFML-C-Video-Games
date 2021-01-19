#include "pch.h"
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "zombie.h"
#include "Bullet.h"
#include "Pickup.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

void setText(Text *temp, Font font, int cSize, Color color, float posX, float posY, String text);

int main()
{
	TextureHolder textureHolder;
	enum class State
	{
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};
	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	VertexArray background;
	Texture textureBackground = TextureHolder::getTexture("graphics/background_sheet.png");

	int numZombies, numZombiesAlive;
	Zombie* zombies = nullptr;

	Bullet bullets[100];
	int currentBullet = 0, bulletSpare = 24, bulletsInClip = 6, clipSize = 6;
	float fireRate = 1;
	Time lastPressed;

	window.setMouseCursorVisible(false);
	Sprite spriteCrossHair;
	Texture textureCrossHair = TextureHolder::getTexture("graphics/crosshair.png");
	spriteCrossHair.setTexture(textureCrossHair);
	spriteCrossHair.setOrigin(25, 25);

	Pickup healthPickup(1), ammoPickup(2);

	int score = 0, hiScore = 0, wave = 0;

	Sprite spriteGameOver, spriteAmmoIcon;
	Texture textureGameOver = TextureHolder::getTexture("graphics/background.png"), textureAmmoIcon = TextureHolder::getTexture("graphics/ammo_icon.png");
	spriteGameOver.setTexture(textureGameOver); spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteGameOver.setPosition(0, 0); spriteAmmoIcon.setPosition(28, 620);

	View hudView(FloatRect(0, 0, resolution.x, resolution.y));

	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	stringstream levelUpStream, hiScoreStream;
	levelUpStream << "1 - Increased Rate Of Fire" << 
		"\n2 - Increased Clip Size (Next Reload)" << 
		"\n3 - Increased Max Health" << 
		"\n4 - Increased Run Speed" << 
		"\n5 - More And Better Health Pickups" << 
		"\n6 - More and Better Ammo Pickups";
	hiScoreStream << "Hi Score:" << hiScore;

	Text pausedText, gameOverText, levelUpText, ammoText, scoreText, hiScoreText, zombiesRemainingText, waveNumberText;

	pausedText.setFont(font);
	pausedText.setCharacterSize(85);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(437.5, 250);
	pausedText.setString("Press ENTER \n To Continue");

	gameOverText.setFont(font);
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(350, 675);
	gameOverText.setString("Press ENTER To Play");

	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(100, 187.5);
	levelUpText.setString(levelUpStream.str());

	ammoText.setFont(font);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(145, 775);
	ammoText.setString("");

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(25, 0);
	scoreText.setString("");

	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(50);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1225, 0);
	hiScoreText.setString(hiScoreStream.str());

	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(50);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1218.75, 775);
	zombiesRemainingText.setString("Zombies: 100");

	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(50);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(937.5, 775);
	waveNumberText.setString("Wave: 0"); //Text // Texts

	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(375, 775);

	int frameSinceLastHUDUpdate = 0, fpsMeasurementFrameInterval = 1000;

	SoundBuffer hitBuffer, splatBuffer, shootBuffer, reloadBuffer, reloadFailedBuffer, powerUpBuffer, pickupBuffer;
	Sound hit, splat, shoot, reload, reloadFailed, powerUp, pickup;
	hitBuffer.loadFromFile("sounds/hit.wav");
	hit.setBuffer(hitBuffer);
	splatBuffer.loadFromFile("sounds/splat.wav");
	splat.setBuffer(hitBuffer);
	shootBuffer.loadFromFile("sounds/shoot.wav");
	shoot.setBuffer(shootBuffer);
	reloadBuffer.loadFromFile("sounds/reload.wav");
	reload.setBuffer(reloadBuffer);
	reloadFailedBuffer.loadFromFile("sounds/reload_failed.wav");
	reloadFailed.setBuffer(reloadFailedBuffer);
	powerUpBuffer.loadFromFile("sounds/powerup.wav");
	powerUp.setBuffer(powerUpBuffer);
	pickupBuffer.loadFromFile("sounds/pickup.wav");
	pickup.setBuffer(pickupBuffer);

	while(window.isOpen())
	{
		
		// Input
		Event event;
		while(window.pollEvent(event))
		{
			if(event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// No frame jump
					clock.restart();				
				}
				else if(event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;

					wave = 0;
					score = 0;

					currentBullet = 0;
					bulletSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					player.resetPlayerStats();
					healthPickup.reset(1);
					ammoPickup.reset(2);

				}
				else if (state == State::PLAYING)
				{
					if (event.key.code == Keyboard::R)
					{
						if (bulletSpare == 0)
						{
							reloadFailed.play();
						} 
						else
						{
							int t = clipSize - bulletsInClip;
							bulletsInClip += min(bulletSpare, t);
							bulletSpare -= (bulletsInClip==clipSize?t:bulletSpare);
							reload.play();
						}
					}
				}
			}	
		} // End event polling

		if(Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if(state == State::PLAYING)
		{
			if(Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			} 
			else 
			{
				player.stopUp();
			}

			if(Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else 
			{
				player.stopRight();
			}

			if(Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else 
			{
				player.stopLeft();
			}

			if(Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			} 
			else
			{
				player.stopDown();
			}
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if(gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000/fireRate && bulletsInClip > 0)
				{
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet = (currentBullet + 1) % 100;
					lastPressed = gameTimeTotal;
					bulletsInClip--;
					shoot.play();
				}
			}

		} // End Playing
		else if(state == State::LEVELING_UP)
		{
			if(event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				clipSize += clipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// Increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// Increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				// Increase pickup
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if(state == State::PLAYING)
			{
				wave++;
				stringstream ssWave;
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());
				arena.width = 500 + 50 * wave;
				arena.height = 500 + 50 * wave;
				arena.left = 0;
				arena.top = 0;
				
				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);
				
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				numZombies = 5 * pow(1.2, wave);

				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				powerUp.play();

				clock.restart();
			}
		} // End Leveling up

		// Update frame
		if(state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			spriteCrossHair.setPosition(mouseWorldPosition);

			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			
			mainView.setCenter(player.getCenter());

			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dt.asSeconds());
				}
			}

			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							bullets[i].stop();
							if (zombies[j].hit())
							{
								score += 10;
								hiScore = max(score, hiScore);
								numZombiesAlive--;
							}

							if (!numZombiesAlive)
							{
								state = State::LEVELING_UP;
							}
							splat.play();
						}
					}
				}
			} // Zombie got shot

			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive() && player.getPosition().intersects(zombies[i].getPosition()))
				{
					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			} // Player got hit

			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletSpare += ammoPickup.gotIt();
				pickup.play();
			}

			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));
			frameSinceLastHUDUpdate++;
			if (frameSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				stringstream ssAmmo, ssScore, ssHiScore, ssWave, ssZombiesAlive;
				
				ssAmmo << bulletsInClip << "/" << bulletSpare;
				ammoText.setString(ssAmmo.str());

				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());
				
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				frameSinceLastHUDUpdate = 0;
			} // End HUD update

		} // End updating frame

		// Draw frame
		window.clear();
		if(state == State::PLAYING)
		{
			window.setView(mainView);
			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			window.draw(player.getSprite());

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			window.draw(spriteCrossHair);

			window.setView(hudView);

			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if(state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();

	} // End game loop
	delete[] zombies;
	return 0;
}
