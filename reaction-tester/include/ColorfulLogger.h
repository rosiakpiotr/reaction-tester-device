#ifndef COLORFULLOGGER_H
#define COLORFULLOGGER_H

#include <iostream>
#include <string>

class ColorfulLogger
{
public:

    enum Color { Black, Red, Green, Yellow, Blue, Magenta, Cyan, White };

    /*
    Color           fg  bg
    Black           30  40
    Red             31  41
    Green           32  42
    Yellow          33  43
    Blue            34  44
    Magenta         35  45
    Cyan            36  46
    White           37  47
    */

    static void print(Color color, std::string text);

private:

    static std::string colorCode(Color color);
};

#endif // COLORFULLOGGER_H
