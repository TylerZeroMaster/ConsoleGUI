#include "..\include\Interface.h"

Interface::Interface()
: maxItems (255)
, focused (-1)
, activeItems (0)
{
	if (hstdin == INVALID_HANDLE_VALUE
    || !GetConsoleMode( hstdin, &mode )
	|| !SetConsoleMode( hstdin, ENABLE_MOUSE_INPUT ))
	{
		std::cout<<"An error occurred when creating the interface.\n";
			throw 1;
			delete this;
	}
}

Interface::~Interface()
{
	for(
		unsigned i = 0;
		i < interVec.size();
		i++
		)
	{
		delete interVec.at(i);
	}
	SetConsoleMode( hstdin, mode );
		delete this;
}

void Interface::regElement(InterfaceElement* element)
{
	if(activeItems < maxItems)
	{
		interVec.push_back(element);
		elByname[element->name] = element;
		activeItems++;
	}
	else
		std::cout<<"There are too many items already!!!!\n";
}

void Interface::erHandle(int const& ercode)
{
	if(ercode > 0)
		std::cout<<"Error at line: "<<ercode<<".\n";
}

void Interface::drawAll()
{
	InterfaceElement* infocus;
	for(
		unsigned i = 0;
		i < interVec.size();
		i++
		)
		{
			infocus = interVec.at(i);
			switch(infocus->type)
			{
				case BUTTON:
					erHandle(drawButton(infocus));
					break;

				case TEXTFIELD:
					erHandle(drawTextField(infocus));
					break;

				case TEXTNODE:
					erHandle(drawTextNode(infocus));
					break;

				default:
					//prints error if type is not defined properly
					erHandle(line);
					break;
			}
		}
}

void Interface::getInput()
{
	//the keycode of the key pressed.
	int keyCode;
	INPUT_RECORD inrec;
	DWORD count;

	//remove previous records from the input buffer.
    FlushConsoleInputBuffer( hstdin );
    //read new information in empty input records;
    ReadConsoleInput( hstdin, &inrec, 1, &count );

    //parse the input records for what we want.
    if ((inrec.EventType == MOUSE_EVENT)
    && inrec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
    && inrec.Event.MouseEvent.dwEventFlags != MOUSE_MOVED)
    {
		mouseHandle
		(
			inrec.Event.MouseEvent.dwMousePosition.X,
			inrec.Event.MouseEvent.dwMousePosition.Y
		);
    }

	if((inrec.EventType == KEY_EVENT)
	&& inrec.Event.KeyEvent.bKeyDown)
	{
		keyCode= (int)inrec.Event.KeyEvent.uChar.UnicodeChar;

		if(focused > -1)
		{
			if(keyCode != 0 && keyCode != 13 && keyCode != 8)
				drawChar(interVec.at(focused), keyCode);
			if(keyCode == 13)
				focused = -1;
			if(keyCode == 8)
				backspace(interVec.at(focused));
		}
		else if(hasKeyHandle)
//			If input is not specific to an item which takes input directly(text fields, etc.)
//			then send the keycode to a user-defined delegate keyhandler.
			this->keyHandle(keyCode);
	}
}

void Interface::mouseHandle(WORD const& x, WORD const& y)
{
	Interface::InterfaceElement * element;
	for(
		unsigned i = 0;
		i < interVec.size();
		i++
		)
			{
				element = interVec.at(i);
				if(x >= element->leftExtent.X
				&& x < element->rightExtent.X
				&& y >= element->leftExtent.Y
				&& y < element->rightExtent.Y)
				{
					element->onClick();
					if(element->type == TEXTFIELD)
						focused = i;
					break;
				}
				else if(focused != -1)
					focused = -1;
			}
}

void Interface::regKeyHandle(void(*function)(int const&))
{
	keyHandle = function;
	hasKeyHandle = 1;
}
