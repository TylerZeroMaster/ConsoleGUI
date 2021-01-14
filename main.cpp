#include ".\include\Interface.h"
#include ".\include\Styler.h"

Interface * inter = Interface::grab();
bool loop = 1;
Styler sty = Styler::open("test.cgui");

void out() {
    loop = 0; }

void printVal() {
	std::cout<<inter->elByname.find("tfield")->second->value; }

//old way
//void testbutton()
//{
//	Interface::InterfaceElement * thebutton = new Interface::InterfaceElement;
//	thebutton->filler = '.';
//	thebutton->borderColor = GREEN | bGREEN;
//	thebutton->fontColor = BLUE | RED | GREEN | bGREEN;
//	thebutton->name = "Quit";
//	thebutton->value = thebutton->name;
//	thebutton->leftExtent = {0, 0};
//	thebutton->rightExtent = {10, 5};
//	thebutton->onClick = out;
//	thebutton->type = 1;
//	inter->regElement(thebutton);
//}

//new way
void testbutton2() {
    Interface::InterfaceElement * thebutton = sty.getStyle("rQuit");
    thebutton->onClick = out;
    inter->regElement(thebutton); }

/**things to do:
		Add a way to base an element's values off of another's
		Add a way to draw every element defined in the style files
		Fix stuff
**/

std::string const elements[] = {
    "Quit"
    , "redQ"
    , "tfield"
    , "tfield2"
    };

void init() {
    Interface::InterfaceElement * element;
    for(
        unsigned i = 0;
        i < ARRAYSIZE(elements);
        i++
    ) {
        element = sty.getStyle(elements[i]);
        if(elements[i] == "Quit")
            element->onClick = out;
		if(elements[i] == "redQ")
			element->onClick = printVal;
        inter->regElement(element); } }

void testfield2() {
    Interface::InterfaceElement * thebutton = sty.getStyle("tfield2");
    inter->regElement(thebutton); }

void myHandle(int const & keyCode) {
    if(keyCode == 27)
        loop = 0;
    else if(keyCode != 0)
        std::cout<<"You pressed "<<char(keyCode)<<'\n'; }

int main() {
//	testbutton2();
//	testfield2();
    init();
    inter->drawAll();
    inter->regKeyHandle(myHandle);
//    std::cout<<inter->elByname.find("tfield2")->second->fontColor;
    while(loop)
        inter->getInput();
    return 0; }
