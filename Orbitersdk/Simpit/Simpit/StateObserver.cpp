#include "StateObserver.h"
StateSaver * StateObserver::saver = 0;

void opcLoadState(FILEHANDLE scn)
{
	StateObserver::saver->loadScenarioState(scn);
}

void opcSaveState(FILEHANDLE scn)
{
	StateObserver::saver->saveScenarioState(scn);
}