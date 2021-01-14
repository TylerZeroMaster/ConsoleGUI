#include "..\include\Interface.h"

int Interface::drawButton(InterfaceElement * button) {
    //check the handle to standard out
    if (hStdOut == INVALID_HANDLE_VALUE) return line;

    //position of the value text on the button
    SHORT const val_x = ((button->rightExtent.X + button->leftExtent.X) - button->value.size())/2 ;
    //wdt - string.len(amodes)) / 2
    SHORT const val_y = (button->rightExtent.Y + button->leftExtent.Y) / 2;

    DWORD width = button->rightExtent.X - button->leftExtent.X;

    //the number of used cells
    DWORD count;

    DWORD SIZE = 1;

    SHORT y = button->leftExtent.Y;
    SHORT x = button->leftExtent.X;

    COORD point;

    point = {x, y };

    while(point.Y < button->rightExtent.Y) {
        if (!FillConsoleOutputCharacter(hStdOut, button->filler, width, point, &count))
            return line;

        if (!FillConsoleOutputAttribute(hStdOut, button->borderColor, width, point, &count))
            return line;

        point.Y++; }
    if(button->value != "") {
        short windows_is_stupid;
        for(
            unsigned i = 0;
            i < button->value.size();
            i++
        ) {
            windows_is_stupid = val_x + i;

            if(windows_is_stupid > button->rightExtent.X - 4)
                break;

            point = {windows_is_stupid, val_y };

            if (!FillConsoleOutputCharacter(hStdOut, button->value[i], SIZE, point, &count))
                return line;

            if (!FillConsoleOutputAttribute(hStdOut, button->fontColor, SIZE, point, &count))
                return line; } }
    return 0; }
