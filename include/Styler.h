#ifndef STYLER_H
#define STYLER_H

#include ".\Interface.h"
#include <fstream>
#include <sstream>

#define ARRAYSIZE( array ) (sizeof( array ) / (sizeof( array[0] )))


class Styler {
public:
    Styler();

    static Styler open(std::string src) {
        Styler out;
        out.setSource(src);
        return out; }

    ~Styler();

    Interface::InterfaceElement * getStyle(std::string name);

    std::string srcCode;

    void isvalidID(std::string identifier);
    void setSource(std::string const& src);
protected:
    std::string const identifiers[6] = {
        "type"
        , "orig"
        , "dimensions"
        , "value"
        , "color"
        , "fontColor" };

    //it is important that the positions of colors and Wcolors match.
    std::string const colors[10] = {
        "RED"
        , "GREEN"
        , "BLUE"
        , "HUE"
        , "<white>"
        , "bRED"
        , "bGREEN"
        , "bBLUE"
        , "bHUE"
        , "<bwhite>" };

    WORD const Wcolors[10] = {
        RED
        , GREEN
        , BLUE
        , HUE
        , RED|GREEN|BLUE
        , bRED
        , bGREEN
        , bBLUE
        , bHUE
        , bRED|bGREEN|bBLUE };

    //"Error in \""<<name<<"\" at statement "<<statement<<": "<<exceptions[e];
    std::string const exceptions[4] = {
        "invalid color definition.\n" 										//0
        , "invalid identifier.\n" 												//1
        , "interface type not defined.\n"								//2
        , "invalid coordinate value.\n"									//3
    };

    WORD parseOr(std::string const & data);
    COORD parseCoord(std::string const & data);

private:
    Interface::InterfaceElement * createDefault(std::string type, Interface::InterfaceElement * out); };

#endif // STYLER_H
