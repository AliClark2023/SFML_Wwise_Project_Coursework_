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
	
	// registering wwise game objects (using IDs and names from AudioObjects.h)
	// only 1 main audio object is required (background music)
	{
		AK::SoundEngine::RegisterGameObj(EVT_PLAY_BG_MUSIC.ID);
		//AK::SoundEngine::RegisterGameObj(EVT_PLAT_LANDING.ID);
		//::SoundEngine::RegisterGameObj(EVT_DESTROY_HAZARD.ID);
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
		
		//--Wwise code----------------------------------------------------------
		//Handle Wwise's audio rendering.
		AK::SoundEngine::RenderAudio();
		//----------------------------------------------------------------------
	
		if (ui->get_reset_level())
		{
			AK::SoundEngine::PostEvent(EVT_STOP_BG_MUSIC.EventName.data(), EVT_STOP_BG_MUSIC.Associated_ID);
			Level = std::make_unique<level>(*window,*view, *ui);
			ui->reset_UI();
		}
		
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
