#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "Player/Player.h"
#include "Utilities/SATDetection.h"
#include "LevelComponents/Level.h"
#include "Obstacles/Scenery.h"
#include "WwiseWrapper.h"

int main()
{
	WwiseWrapper wwise;

	//--Wwise code--------------------------------------------------------------
	//TODO: We pass in the soundbank path here. You will probably need to update
	//		this for your code.
	//Note: Wwise expects wide strings when loading soundbanks on Windows. The
	//		AKTEXT macro converts our soundbank path to the correct string type
	//		for the current platform.
	if (!wwise.initSoundEngine(AKTEXT("SFML Wwise Project/GeneratedSoundBanks/Windows")))
	{
		std::cout << "Could not initialise Wwise. Exiting." << std::endl;
		return 1;
	}

	//TODO: This code loads the "MainSoundbank" from the included Wwise project;
	//		you will probably want to replace it with your own soundbank.
	{
		AkBankID mainBankId;
		if (AK::SoundEngine::LoadBank(AKTEXT("MainSoundbank"), mainBankId) != AK_Success)
		{
			assert(!"Could not load soundbank.");
			return 1;
		}
	}

	//TODO: This code tests event posting using the Loop event from the included
	//		Wwise project. You will want to remove it for your own projects.
	{
		const uint64_t gameObjectId = 1;

		//In order to post an event in Wwise it must be associated with a game
		//object. To Wwise, game objects are just a set of unique integer IDs,
		//but you will need to register each game object with Wwise so that it
		//is aware of them.

		//For testing purposes we just register a single ID of 1 here, but in
		//your own projects you will probably want to implement your own unique
		//ID system.

		//Note that the Wwise listener is also a game object. This project
		//assigns the listener with an ID of 0, hence the ID of 1 for our loop.
		AK::SoundEngine::RegisterGameObj(gameObjectId);

		AK::SoundEngine::PostEvent(AKTEXT("Loop"), gameObjectId);
	}
	//--------------------------------------------------------------------------

	// game initialisation
	std::shared_ptr<sf::RenderWindow> window(new sf::RenderWindow (sf::VideoMode({ 1280, 720 }), "Audio Coursework"));
	std::shared_ptr<sf::View> view(new sf::View);
	view->setCenter(sf::Vector2f(window->getSize().x/2, window->getSize().y/2));
	view->setSize(sf::Vector2f(1280, 720));
	window->setView(*view);
	window->setFramerateLimit(60);

	std::unique_ptr<level> Level(new level(window,view));
	
	// Initialise objects for delta time
	sf::Clock clock;
	
	while (window->isOpen())
	{
		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		float deltaTime = clock.restart().asSeconds();
		
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window->close();
				}
			}

		}

		//--Wwise code----------------------------------------------------------
		//Handle Wwise's audio rendering.
		AK::SoundEngine::RenderAudio();
		//----------------------------------------------------------------------
	
		Level->handle_input(deltaTime);
		Level->update(deltaTime);

		// render cycle
		window->clear();

		Level->render();
		
		window->display();
	}

	//--Wwise code--------------------------------------------------------------
	wwise.terminateSoundEngine();
	//--------------------------------------------------------------------------

	return 0;
}
