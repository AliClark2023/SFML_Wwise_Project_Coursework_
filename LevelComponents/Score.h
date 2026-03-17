#pragma once
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/AssetUtilities.h"

class score
{
public:
    score();
    int get_score() const { return score_; }
    sf::Text* get_text() const;
    void render_score(sf::RenderWindow& window) const;
    void add_to_score(const int& val_to_add);
    void sub_from_score(const int& val_to_sub);
private:
    int score_ = 0;
    // text variables
    sf::Font score_font_;
    std::unique_ptr<sf::Text> score_text_;
};
