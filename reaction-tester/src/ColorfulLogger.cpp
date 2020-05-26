#include "ColorfulLogger.h"

void ColorfulLogger::print(Color color, std::string text)
{
    std::string coloringPhrase = "\x1B[" + colorCode(color);
    std::string backToRegular = "\033[0m";
    std::cout << coloringPhrase << text << backToRegular << std::endl;
}

std::string ColorfulLogger::colorCode(Color color)
{
    return std::to_string(30 + color) + "m";
}
