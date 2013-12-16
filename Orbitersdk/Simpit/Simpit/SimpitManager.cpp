#include "SimpitManager.h"

SimpitManager::SimpitManager(HINSTANCE hDLL) : Module(hDLL)
{
	ModuleFactory::createModules(modules, hDLL);

}

void SimpitManager::handleEvent(Event ev)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->handleEvent(ev);
	}
}
void SimpitManager::clbkSimulationStart(RenderMode mode)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->SimulationStart();
	}
}
void SimpitManager::clbkFocusChanged(OBJHANDLE old_focus, OBJHANDLE new_focus)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->FocusChanged(old_focus,new_focus);
	}
}
void SimpitManager::clbkSimulationEnd(RenderMode mode)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->SimulationEnd();
	}
}

void SimpitManager::clbkPreStep(double simt, double simdt, double mjd)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->PreStep(simt,simdt,mjd);
	}
}

void SimpitManager::clbkPostStep(double simt, double simdt, double mjd)
{
	for (unsigned int i = 0; i < modules.size(); i++)
	{
		modules[i]->PostStep(simt,simdt,mjd);
	}
}