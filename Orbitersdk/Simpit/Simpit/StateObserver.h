#ifndef STATE_OBSERVER
#define STATE_OBSERVER

#include "OrbiterAPI.h"
#include "StateSaver.h"



class StateObserver
{
public:
	static void setUpManager(StateSaver * _saver) { saver = _saver; }
public:
	static StateSaver * saver;
};

DLLCLBK void opcSaveState(FILEHANDLE scn); 

DLLCLBK void opcLoadState(FILEHANDLE scn);

#endif