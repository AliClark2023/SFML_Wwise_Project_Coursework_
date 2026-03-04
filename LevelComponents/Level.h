#pragma once
#include "../Player/Player.h"
#include "../Obstacles/Scenery.h"
#include "../BaseClasses/GameObject.h"
#include "../Utilities/SATDetection.h"

class level
{
public:
    level(const std::shared_ptr<sf::RenderWindow>& win, const std::shared_ptr<sf::View>& v);
    void handle_input(float dt);
    void update(float dt);
    void render();
    
private:
    std::weak_ptr<sf::RenderWindow> window_ref_;
    std::weak_ptr<sf::View> view_ref_;
    
    std::unique_ptr<Player> player_;
    std::unique_ptr<Scenery> ground_;
    std::vector<GameObject> obstacles_;
};
