//Copyright (c) 2013 Christopher Johnstone(meson800)
//The MIT License - See ../../../LICENSE for more info

#ifndef EXTERNAL_OUTPUT_HEADER
#define EXTERNAL_OUTPUT_HEADER

#include <map>
#include <string>

#include "MacroExpander.h"
#include "Event.h"
#include "Output.h"

struct programInfo
{
	char name [255];
	char args [255];
};

class ExternalOutput : public Output
{
public:
	ExternalOutput(){}
	void load(const char * key, const char * value);
	void save(FILEHANDLE outputFile) {}
	void handleEvent(Event ev);

private:
	std::map<Event,programInfo> eventMapping;
};

#endif