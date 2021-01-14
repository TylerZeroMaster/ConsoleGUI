#include "..\include\Interface.h"

int Interface::drawTextNode(InterfaceElement * theNode) {
    DWORD count;
    COORD point;
    SHORT val_x;
    SHORT val_y = theNode->leftExtent.Y;
    unsigned newLine = 0;

    if(hStdOut == INVALID_HANDLE_VALUE) return line;

    for(
        unsigned i = 0;
        i < theNode->value.size();
        i++
    ) {
        val_x = theNode->leftExtent.X + i - newLine;
        if(theNode->value[i] == '\\' && theNode->value[i - 1] != '\\') {
			if(theNode->value[i + 1] == 'n') {
				val_y++;
				newLine = i + 2; }
			if(theNode->value[i + 1] == 't') {
				val_x += 4; }
			i++; }
		else {
			point = {val_x, val_y };
			if (!FillConsoleOutputCharacter(hStdOut, theNode->value[i], 1, point, &count))
				return line;

			if (!FillConsoleOutputAttribute(hStdOut, theNode->fontColor, 1, point, &count))
				return line; } }
    return 0; }

