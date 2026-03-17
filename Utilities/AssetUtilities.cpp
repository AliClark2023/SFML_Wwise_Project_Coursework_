#include "../Utilities/AssetUtilities.h"

 void asset_utilities::LoadFont(sf::Font& font, const std::string& font_name)
{
    if (!font.openFromFile(font_name))
    {
        std::cerr << "Could not open font file: " << font_name << "!!" << '\n';
    }
}
