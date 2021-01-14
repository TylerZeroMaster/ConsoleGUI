#ifndef INTERFACE_H
#define INTERFACE_H

#define line __LINE__

#include <iostream>
#include <vector>
#include <map>

#include <windows.h>
#define WINDOWS_LEAN_AND_MEAN

#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define HUE FOREGROUND_INTENSITY
#define bRED BACKGROUND_RED
#define bGREEN BACKGROUND_GREEN
#define bBLUE BACKGROUND_BLUE
#define bHUE BACKGROUND_INTENSITY

#define BUTTON 0x0001
#define TEXTFIELD 0x0002
#define TEXTNODE 0x0003

#ifndef UNICODE
    #define UNICODE
#endif // UNICODE


class Interface
{
	public:
		static Interface* grab() { return new Interface(); }
		~Interface();

		unsigned maxItems;
		int focused;
		unsigned activeItems;
		bool hasKeyHandle;

		void getInput();

		struct InterfaceElement
		{
			char filler;
			WORD borderColor;
			WORD fontColor;
			std::string name;
			std::string value;
			COORD leftExtent;
			COORD rightExtent;
			void(*onClick)();
			int type;
		};

		std::map<std::string, InterfaceElement*> elByname;

		void regElement(InterfaceElement* element);
		void regKeyHandle(void(*funcion)(int const&));

		void erHandle(int const& ercode);
		void drawAll();

	protected:
		std::vector<InterfaceElement*>interVec;
		void(*keyHandle)(int const&);
		Interface();
	private:
		HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
		HANDLE hstdin = GetStdHandle( STD_INPUT_HANDLE );
		DWORD mode;

		void mouseHandle(WORD const& x, WORD const& y);

		//TextField
		//{
			void drawChar(InterfaceElement* element, char const& newChar);
			void backspace(InterfaceElement* element);
		//}

		int drawButton(InterfaceElement* button);
		int drawTextField(InterfaceElement* theField);
		int drawTextNode(InterfaceElement* theNode);
};

#endif // INTERFACE_H
