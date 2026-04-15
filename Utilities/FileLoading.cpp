#include "FileLoading.h"

void FileLoading::load_font(sf::Font& font, const std::string& font_name)
{
    if (!font.openFromFile(font_name))
    {
        std::cerr << "Could not open font file: " << font_name << "!!" << '\n';
    }
}
