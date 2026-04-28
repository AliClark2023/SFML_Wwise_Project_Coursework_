#pragma once
#include "../WwiseWrapper.h"
#include  "../Constants/AudioObjects.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../Utilities/FileLoading.h"
#include "../LevelComponents/Score.h"
#include "../LevelComponents/Timer.h"
#include "../LevelComponents/PauseMenu.h"
#include "../LevelComponents/AudioManager.h"


// handles UI aspects of game, incorporates score, timer and pause components
// implemented as a class as it should be able to function within any levels (if implemented)
class menu_ui
{
public:
    menu_ui( sf::RenderWindow& win,  sf::View& v);
    void handle_input(float dt);
    void render() const;
    
    bool is_pause_active() const {return pause_ui_->is_active();}
    bool get_reset_level() const {return reset_level_;}
    void set_reset_level(const bool reset_level) {reset_level_ = reset_level;}
    void reset_UI();
    // UI functions
    // score
    void add_to_score(const int & score_val) const { score_ui_->add_to_score(score_val); }
    int get_score() const {  return score_ui_->get_score(); }
    
    
private:
    sf::RenderWindow& window_ref_;
    sf::View& view_ref_;
   
    //used as flag to reset current level, should change to enum to denote which level to reset
    bool reset_level_ = false;
    
    // text variables (default font used by components)
    sf::Font font_;
    
    // UI components
    std::unique_ptr<score> score_ui_;
    std::unique_ptr<timer> timer_ui_;
    std::unique_ptr<pause_menu> pause_ui_;
    
    // audio events
    AudioObject pause_bg;
    AudioObject resume_bg;
};
