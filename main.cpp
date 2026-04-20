#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "Player/Player.h"
#include "LevelComponents/Menu.h"
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

	//Loading soundbank (project specific soundbank)
	{
		AkBankID mainBankId;
		if (AK::SoundEngine::LoadBank(AKTEXT("ProjectSoundbank"), mainBankId) != AK_Success)
		{
			assert(!"Could not load soundbank.");
			return 1;
		}
	}
	
	// game initialisation
	std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow (sf::VideoMode({ 1280, 720 }), "Audio Coursework"));
	std::unique_ptr<sf::View> view(new sf::View);
	view->setCenter(sf::Vector2f(window->getSize().x/2, window->getSize().y/2));
	view->setSize(sf::Vector2f(1280, 720));
	window->setView(*view);
	window->setFramerateLimit(60);

	// levels
	std::unique_ptr<menu_ui> ui(new menu_ui(*window,*view));
	std::unique_ptr<level> Level(new level(*window,*view, *ui));

	
	// Initialise objects for delta time
	sf::Clock clock;
	
	while (window->isOpen())
	{
		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		float deltaTime = clock.restart().asSeconds();
		
		/*
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				// move input into level ?
				
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window->close();
				}
				
				if (keyPressed->scancode == sf::Keyboard::Scancode::P)
				{
					
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::R)
				{
					
				}
					
			}

		}
		*/

		//--Wwise code----------------------------------------------------------
		//Handle Wwise's audio rendering.
		AK::SoundEngine::RenderAudio();
		//----------------------------------------------------------------------
	
		Level->handle_input(deltaTime);
		Level->update(deltaTime);

		// render cycle
		window->clear();
		ui->render();
		Level->render();
		
		window->display();
	}

	//--Wwise code--------------------------------------------------------------
	wwise.terminateSoundEngine();
	//--------------------------------------------------------------------------

	return 0;
}
