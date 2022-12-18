// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Timber.h"
#include "Texture.h"

int main()
{
	// Create a video mode objcect
	sf::VideoMode vm(1920, 1080);

	// Create and Open a wnidow for the game 
	sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

	//Create a texture to hold a graphic on the GPU 
	sf::Texture textureBackground;

	// Load a graphic into the texture 
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite 
	sf::Sprite spriteBackground;

	//Attach the texture to the sprite 
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Load Player Texture 
	sf::Texture texturePlayer;
	try
	{
		if (!texturePlayer.loadFromFile("graphics/player.png"));
		throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open player png file " << std::endl;
	}
	//Make a player sprite 
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The Player starts on the left
	side playerSide = side::LEFT;
	// Prepare the gravestone
	sf::Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	sf::Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);
	// Prepare the axe
	sf::Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	sf::Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	// Line the axe up with the tree 

	//Preparw the fyling log 
	sf::Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	sf::Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//  Control the player input 
	bool acceptInput = false;
	// Prepare the sounds
	// The player chopping sound
	sf::SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	sf::Sound chop;
	chop.setBuffer(chopBuffer);
	// The player has met his end under a branch
	sf::SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	sf::Sound death;

	death.setBuffer(deathBuffer);
	// Out of time
	sf::SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	sf::Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);
	// Make a tree sprite
	sf::Texture textureTree;
	try
	{
		if (!textureTree.loadFromFile("graphics/tree.png"))
			throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open tree png file " << std::endl;
	}
	sf::Sprite tree;

	tree.setTexture(textureTree);
	tree.setPosition(810, 0);

	//Make a bee sprite
	sf::Texture textureBee;
	try
	{
		if (!textureBee.loadFromFile("graphics/bee.png"))
			throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open bee png file " << std::endl;
	}
	sf::Sprite bee;
	bee.setTexture(textureBee);
	bee.setPosition(0, 800);

	std::array<bool, NUM_OF_BEES> bool_bee;
	// Is the bee moving ?
	bool_bee.fill(false);

	// Speed of the bee
	std::array<float, NUM_OF_BEES> bee_speed;
	bee_speed.fill(0.0f);

	// make 3 cloud sprites from 1 texture
	sf::Texture textureCloud;
	// Load 1 new texture
	try
	{
		if (!textureCloud.loadFromFile("graphics/cloud.png"))
			throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open cloud png file " << std::endl;
	}
	// 3 New sprites with the same texture
	sf::Sprite spriteCloud;
	// Container for the 3 clouds 
	sf::Sprite array_cloud[NUM_OF_CLOUD]{};
	for (size_t i = 0; i < NUM_OF_CLOUD; i++)
	{
		// 3 New sprites with the same texture
		array_cloud[i].setTexture(textureCloud);
		// Position the clouds on the left of the screen
		// at different heights
		array_cloud[i].setPosition(0, i * 250);
	}

	// Are the clouds currently on screen?
	bool bool_cloud_array[NUM_OF_CLOUD]{ false };
	// How fast is each cloud?
	float cloud_speed_array[NUM_OF_CLOUD]{ 0.0f };

	// Variable to control time itself

	sf::Clock clock;

	// Track wheter the game is running
	bool paused = true;

	// Hold the score of the game
	int score{ 0 };

	// Create the text object to show on the window
	sf::Text messageText;
	sf::Text scoreText;

	//Load the font size 
	sf::Font font;
	try
	{
		if (!font.loadFromFile("fonts/KOMIKAP_.ttf"))
			throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open font size file " << std::endl;
	}

	// Assign the actual message 
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	// Assign a size 
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Set the font to our Text object
	messageText.setFont(font);
	scoreText.setFont(font);

	//Choose a color
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	//Position the text 
	sf::FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Prepare 6 branches
	sf::Texture textureBranch;
	try
	{
		if (!textureBranch.loadFromFile("graphics/branch.png"))
			throw(false);
	}
	catch (bool)
	{
		std::cerr << "Failed to open branch file " << std::endl;

	}

	// Set the texture for each branch sprite
	for (auto& branch : branches)
	{
		branch.setTexture(textureBranch);
		branch.setPosition(-2000, -2000);
		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branch.setOrigin(220, 20);
	}
	//Time Bar 
	sf::RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	sf::Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	int cloud_index = 0;
	int bee_index = 0;
	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		// Start the game 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			paused = false;
			//Reset the Time and the score 
			score = 0;
			timeRemaining = 6;
			// Make all the branchs disapear 
			// Start in the second position 
			for (auto& i : branchPositions)
			{
				i = side::NONE;
			}
			// Make sure the gravestone is hidden 
			spriteRIP.setPosition(675, 2000);

			// Move player into position 
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}
		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{

			// First handle pressinf the right cursor key 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;//adding to the amount of time remaining.We are rewarding the player , HIGHER SCORE less time
				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				// Update the branches
				updateBranches(score);
				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				chop.play();

			}
			// First handle pressinf the right cursor key 
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				// Update the branches
				updateBranches(score);
				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 
					5000;
				logActive = true;
				acceptInput = false;
				chop.play();


			}
		}

		if (!paused)
		{
			/*
			****************************************
			Update the scene
			****************************************
			*/
			//Measure Time
			sf::Time dt = clock.restart();
			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f)
			{
				// Pause the Game 
				paused = true;
				// Change the message shown to the player
				messageText.setString("Out of the time!!");


				//Reposition the text based on its new size 
				sf::FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				// Play the out of time sound
				outOfTime.play();
			}
			//Setup the bee 
			if (!bool_bee[bee_index])
			{
				//How fast is the bee
				std::srand((int)std::time(0));
				bee_speed[bee_index] = (std::rand() % 200) + 200;


				// How high is the bee 
				std::srand((int)std::time(0) * 10);
				float height = (std::rand() % 500) + 500;
				bee.setPosition(2000, height);
				bool_bee[bee_index] = true;
			}
			else
			{
				// Move bee
				int moveInX = bee.getPosition().x - (bee_speed[bee_index] * dt.asSeconds());
				int moveInY = bee.getPosition().y;
				bee.setPosition(moveInX, moveInY);
				bee.setRotation(20);
				//Has the bee reached the left-hand edge of the screen ? 
				if (bee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee next frame
					bool_bee[bee_index] = false;
				}


			}
			// Manage the clouds
			// Cloud 1
			if (!bool_cloud_array[cloud_index])
			{
				// How fast is the cloud
				srand((int)time(0) * 100);
				cloud_speed_array[cloud_index] = (rand() % 200);
				// How high is the cloud
				srand((int)time(0) * 10);
				float height = 0;
				array_cloud[cloud_index].setPosition(-200, height);
				bool_cloud_array[cloud_index] = true;
			}
			else
			{
				array_cloud[cloud_index].setPosition(
					array_cloud[cloud_index].getPosition().x +
					(cloud_speed_array[cloud_index] * dt.asSeconds()),
					array_cloud[cloud_index].getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (array_cloud[cloud_index].getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					bool_cloud_array[cloud_index] = false;
				}
			}
			// Manage the clouds
			// Cloud 2
			if (!bool_cloud_array[cloud_index + 1])
			{
				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud_speed_array[cloud_index + 1] = (rand() % 200);
				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 300) - 150;
				array_cloud[cloud_index + 1].setPosition(-200, height);
				bool_cloud_array[cloud_index + 1] = true;
			}
			else
			{
				array_cloud[cloud_index + 1].setPosition(
					array_cloud[cloud_index + 1].getPosition().x +
					(cloud_speed_array[cloud_index + 1] * dt.asSeconds()),
					array_cloud[cloud_index + 1].getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (array_cloud[cloud_index + 1].getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					bool_cloud_array[cloud_index + 1] = false;
				}
			}
			// Manage the clouds
			// Cloud 3
			if (!bool_cloud_array[cloud_index + 2])
			{
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud_speed_array[cloud_index + 2] = (rand() % 200);
				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				array_cloud[cloud_index + 2].setPosition(-200, height);
				bool_cloud_array[cloud_index + 2] = true;
			}
			else
			{
				array_cloud[cloud_index + 2].setPosition(
					array_cloud[cloud_index + 2].getPosition().x +
					(cloud_speed_array[cloud_index + 2] * dt.asSeconds()),
					array_cloud[cloud_index + 2].getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (array_cloud[cloud_index + 2].getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					bool_cloud_array[cloud_index + 2] = false;
				}
			}
			//Update the score test 
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
			score++;
			// update the branch sprites
			int i = 0;
			for (auto branch : branchPositions)
			{
				float height = i * 150;

				if (branch == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branch == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
				i++;
			}
			// Handle a flying log 
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));
				// Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);


				}
			}
			// Has the player been squished bya branch ? 
			if (branchPositions[5] == playerSide)
			{
				// death 
				paused = true;
				acceptInput = false;

				// Draw the gravestone 
				spriteRIP.setPosition(525, 760);

				// hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message
				messageText.setString("Squished !!");
				// Center it on the screen 
				sf::FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);
				// Play the death sound
				death.play();
			}
			/*
			****************************************
			Draw the scene
			****************************************
			*/

			// Clear everything from the last run frame
			window.clear();
			// Draw our game scene here
			window.draw(spriteBackground);
			// Draw the clouds
			for (auto cloud : array_cloud)
			{
				window.draw(cloud);
			}
			// Draw the branch 
			for (auto i : branches)
			{
				window.draw(i);
			}
			// Draw the tree
			window.draw(tree);
			// Draw the player;
			window.draw(spritePlayer);
			// Draw the axe
			window.draw(spriteAxe);
			// Draw the flying log
			window.draw(spriteLog);
			// Draw the gravestone
			window.draw(spriteRIP);
			// Draw the insect
			window.draw(bee);
			// Draw the messages 
			if (paused)
			{
				window.draw(messageText);
			}

			window.draw(scoreText);
			//Draw the timebar 
			window.draw(timeBar);
			// Show everything we just drew
			window.display();

		}
	}
	return 0;
}

void updateBranches(int seed)
{
	// Movel all the branches down one place 
	for (int j = NUM_OF_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	// Spawn A new branch at position 0 
	//Leftm, Right or None
	srand((int)std::time(0) + seed);
	int r = (rand() % 5);

	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default :
		branchPositions[0] = side::NONE;
		break;
	}
}