#include "../Constants/FileAssets.h"

const sf::Font& FileAssets::get_ui_font()
{
    return ui_font_;
}

sf::Font FileAssets::ui_font_ = []
{
    sf::Font f;
    if (!f.openFromFile("Assets/Fonts/Montserrat-Regular.ttf"))
    {
        std::cerr << "Could not open font file.\n";
    }
    return f;
}();