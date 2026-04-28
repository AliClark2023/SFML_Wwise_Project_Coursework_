#pragma once
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/FileLoading.h"

/* Score UI component (used in conjunction with Menu UI)
 * Updates score text based on function activations
 */
class score
{
public:
    score();
    score(sf::Font& font);
    int get_score() const { return score_; }
    sf::Text* get_text() const;
    void render_score(sf::RenderWindow& window) const;
    void add_to_score(const int& val_to_add);
    void sub_from_score(const int& val_to_sub);
    void set_pos(const sf::Vector2f& pos) const {score_text_->setPosition(pos); }
    void reset_score(){ score_ = 0; }
private:
    int score_ = 0;
    // text variables
    sf::Font score_font_;
    std::unique_ptr<sf::Text> score_text_;
};
