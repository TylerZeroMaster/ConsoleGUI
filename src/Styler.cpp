#include "..\include\Styler.h"

void blank() {}

Styler::Styler() {
    //ctor
}

Styler::~Styler() {
    //dtor
}

void Styler::setSource(std::string const& src) {
    std::ifstream::pos_type size;
    char * bytebuffer;
    std::ifstream opened;
    opened.open(src, std::ios::ate);
    if (opened.is_open()) {
        size = opened.tellg();
        bytebuffer = new char[size];
        opened.seekg(0, std::ios::beg);
        opened.read(bytebuffer, size);
        opened.close();
        srcCode = bytebuffer;
        delete[] bytebuffer; } }

Interface::InterfaceElement * Styler::getStyle(std::string name) {
    Interface::InterfaceElement * element = new Interface::InterfaceElement;
    element->name = name;

    std::string
    dataType = ""
	, data = ""
	, parsable = "";

    std::ifstream::pos_type pos;

    name += '#';
    if(srcCode.find(name) == std::string::npos) {
        std::cout<<"Unable to find \""<<element->name<<"\"\n";
        return element; }
    for(
        unsigned i = srcCode.find(name) + name.size() + 2;
        srcCode[i] != '}';
        i++
    ) {
        if(srcCode[i] != '\t' && srcCode[i] != '\n')
            parsable += srcCode[i]; }
    for(
        unsigned i = 0;
        i < ARRAYSIZE(identifiers);
        i++
    ) {
        dataType = identifiers[i];
        pos = parsable.find(dataType);
        if(pos == std::string::npos) {
            if(dataType == "type") {
                std::cout<<"Error in \""<<name<<"\" at statement \""<<dataType<<"\" "<<exceptions[2];
                return element; }
            if(dataType == "dimensions") {
                element->rightExtent.X = element->leftExtent.X + element->rightExtent.X;
                element->rightExtent.Y = element->leftExtent.Y + element->rightExtent.Y; }
			continue; }
        else {
            unsigned p = ((int)pos)+dataType.size();
            while(parsable[p] != ':')
                p++;
            p++;
            for(
                ;
                parsable[p] != ';';
                p++
            ) {
                if(parsable[p] == '\"') {
                    p++;
                    while(parsable[p] != '\"') {
                        data += parsable[p];
                        p++; } }
                else if(parsable[p] != ' ')
                    data += parsable[p]; } }
        try {
				if(dataType == "type")
					element = createDefault(data, element);

				if(dataType == "color")
					element->borderColor = parseOr(data);

				if(dataType == "fontColor")
					element->fontColor = parseOr(data);

				if(dataType == "orig")
					element->leftExtent = parseCoord(data);

				if(dataType == "dimensions") {
					COORD temp = parseCoord(data);
					element->rightExtent.X = element->leftExtent.X + temp.X;
					element->rightExtent.Y = element->leftExtent.Y + temp.Y; }

				if(dataType == "value" && data != "$name")
					element->value = data; }
        catch(int e) {
            std::cout<<"Error in \""<<element->name<<"\" at statement \""<<dataType<<"\" "<<exceptions[e]; }

        data = ""; }
    return element; }

WORD Styler::parseOr(std::string const & data) {
    std::stringstream toWORD;
    std::string parsed = "";
    WORD out = 0;
    WORD temp;
    unsigned iter = 0;
    while(data[iter-1] != '|')
		iter++;
    while(data[iter] != '|' && iter < data.size()) {
        if(data[iter] != ',') {
            parsed += data[iter]; }
        if(data[iter] == ',' || data[iter+1] == '|') {
			if((int)parsed[0] > 57) {
                for(
                    unsigned i = 0;;
                    i++
                ) {
                    if(i > ARRAYSIZE(colors)) {
                        throw 0;
                        return out; }

                    if(colors[i] == parsed) {
                        out = out | Wcolors[i];
                        break; } } }
			else {
				if(parsed.size() > 1) {
					toWORD<<parsed;
					toWORD>>temp;
					out += temp;
					toWORD.clear(); }
				else
					out += (int)parsed[0] - 48; }
            parsed = ""; }
        iter++; }
    return out; }

COORD Styler::parseCoord(std::string const & data) {
    std::stringstream toWORD;
    int cCount = 0 ;
    std::string parsed = "";
    WORD val;
    COORD out = { 0, 0 };
    unsigned iter = 0;
    while(data[iter-1] != '(')
		iter++;
    while(data[iter] != ')' && iter < data.size()) {
        if(data[iter] != ',') {
            parsed += data[iter]; }
        if(data[iter] == ',' || data[iter+1] == ')') {
            if((int)parsed[0] > 57 || (int)parsed[0] < 48 || cCount > 1) {
                throw 3;
                return out; }

            if(parsed.size() > 1) {
                toWORD<<parsed;
                toWORD>>val; }
            else
                val = (int)parsed[0] - 48;

            if(cCount > 0)
                out.Y = val;
            else
                out.X = val;
            cCount++;
            parsed = "";
            toWORD.clear(); }
        iter++; }
    return out; }


void Styler::isvalidID(std::string identifier) {
    for(
        unsigned i = 0;
        i < ARRAYSIZE(identifiers);
        i++
    ) {
        if(identifiers[i] == identifier)
            return; }
    throw 1; }

Interface::InterfaceElement * Styler::createDefault(std::string type, Interface::InterfaceElement * out) {
    for(
        unsigned i = 0;
        i < type.size();
        i++
    ) {
        //make the type lowercase(might need to be modified if types ever contain numbers||spaces)
        if((int)type[i] < 97)
            type[i] = (int)type[i] + 32; }

    out->type = -1;
    out->filler = '.';
    out->onClick = blank;
    if(type == "button") {
        out->borderColor = RED | GREEN | BLUE | bRED | bGREEN | bBLUE;
        out->fontColor = RED | GREEN | BLUE | HUE| bRED | bGREEN | bBLUE;
        out->value = out->name;
        out->leftExtent = { 0, 0 };
        out->rightExtent = { 10, 5 };
        out->type = BUTTON; }
    if(type == "textfield" || type == "textnode") {
        out->borderColor = RED | GREEN | BLUE | HUE | bRED | bGREEN | bBLUE | bHUE;
        out->fontColor = RED | GREEN | BLUE;
        out->leftExtent =  { 0, 0 };
        out->rightExtent = { 30, 3 };
        if(type == "textfield") {
            out->value = "";
            out->type = TEXTFIELD; }
        else {
            out->value = out->name;
            out->type = TEXTNODE; } }
    return out; }
