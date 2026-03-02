#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

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

	sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}
		}

		//--Wwise code----------------------------------------------------------
		//Handle Wwise's audio rendering.
		AK::SoundEngine::RenderAudio();
		//----------------------------------------------------------------------

		window.clear();
		window.draw(shape);
		window.display();
	}

	//--Wwise code--------------------------------------------------------------
	wwise.terminateSoundEngine();
	//--------------------------------------------------------------------------

	return 0;
}
