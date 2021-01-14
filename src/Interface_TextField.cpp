#include "..\include\Interface.h"

int Interface::drawTextField(InterfaceElement * theField) {
    if(hStdOut == INVALID_HANDLE_VALUE) return line;

    DWORD width = theField->rightExtent.X - theField->leftExtent.X;
    DWORD count;
    DWORD SIZE = 1;

    SHORT y = theField->leftExtent.Y;
    SHORT x = theField->leftExtent.X;

    COORD point;

    while(y < theField->rightExtent.Y) {
        if(y == theField->leftExtent.Y || y == theField->rightExtent.Y - 1) {
            x = theField->leftExtent.X;
            SIZE = width; }
        else {
            if(x == theField->leftExtent.X) {
                x = theField->rightExtent.X-1; }
            else {
                x = theField->leftExtent.X; } }

        point = { x, y };

        if (!FillConsoleOutputCharacter
                (
                    hStdOut, // output
                    theField->filler, //character to fill with
                    SIZE, //width
                    point, //WriteCOORD
                    &count //output number of cells written to
                )
           ) return line;

        if (!FillConsoleOutputAttribute(hStdOut, theField->borderColor, SIZE, point, &count))
            return line;

        if(SIZE == width || x != theField->leftExtent.X) {
            if(SIZE != 1) {
                SIZE = 1;
                x = theField->leftExtent.X + 1; }
            y++; } }
    return 0; }

void Interface::drawChar(InterfaceElement * element, char const & newChar) {
    DWORD count;
    COORD point;
    SHORT val_x = element->leftExtent.X + element->value.size() + 1;
    SHORT val_y = element->leftExtent.Y + 1;
    if(val_x + 1 >= element->rightExtent.X) {
        std::cout<<'\a';
        return; }

    if(hStdOut == INVALID_HANDLE_VALUE)
        return;

    point = { val_x, val_y };

    if (!FillConsoleOutputCharacter(hStdOut, newChar, 1, point, &count))
        return;

    if (!FillConsoleOutputAttribute(hStdOut, element->fontColor, 1, point, &count))
        return;

        element->value += newChar; }

void Interface::backspace(InterfaceElement * element) {
    DWORD count;
    COORD point;
    SHORT val_x = element->leftExtent.X + element->value.size();
    SHORT val_y = element->leftExtent.Y + 1;
    if(val_x == element->leftExtent.X) return;

    if(hStdOut == INVALID_HANDLE_VALUE) return;

    point = { val_x, val_y };

    if (!FillConsoleOutputCharacter(hStdOut, ' ', 1, point, &count))
        return;

	if (!FillConsoleOutputAttribute(hStdOut,0, 1, point, &count))
		return;

    element->value.pop_back(); }
