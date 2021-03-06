//Copyright (c) 2013 Christopher Johnstone(meson800)
//The MIT License - See ../../../LICENSE for more info

#include "ModuleFactory.h"

void ModuleFactory::createModules(vector<IO *>& moduleVector, HINSTANCE hDLL)
{
	FILEHANDLE configFile = oapiOpenFile("simpit_config.cfg", FILE_IN, CONFIG);
	char *moduleName;
	while (oapiReadScenario_nextline(configFile, moduleName))
	{
		//when we haven't reached an EOF, then this must be a module name;
		//ignore comments
		if (strcmp(moduleName, "") != 0)
		{
			if (strcmp(moduleName, "BEGIN PANEL_EVENT_OUTPUT") == 0)
			{
				Log::println("[ModuleFactory]Creating PanelEventOutput");
				moduleVector.push_back(new PanelEventOutput());
			}
			else if (strcmp(moduleName, "BEGIN SERIAL_INPUT") == 0)
			{
				Log::println("[ModuleFactory]Creating SerialInput");
				moduleVector.push_back(new SerialInput());
			}
			else if (strcmp(moduleName, "BEGIN PANEL_CLICK_RECORDER") == 0)
			{
				Log::println("[ModuleFactory]Creating PanelClickRecorder");
				moduleVector.push_back(new PanelClickRecorderOutput());
			}
			else if (strcmp(moduleName, "BEGIN SIMPIT_MFD") == 0)
			{
				Log::println("[ModuleFactory]Creating SimpitMFD");
				moduleVector.push_back(new SimpitMFD(hDLL));
			}
			else if (strcmp(moduleName, "BEGIN EXTERNAL_OUTPUT") == 0)
			{
				Log::println("[ModuleFactory]Creating ExternalOutput");
				moduleVector.push_back(new ExternalOutput());
			}
			else if (strcmp(moduleName, "BEGIN TIME_BASED_INPUT") == 0)
			{
				Log::println("[ModuleFactory]Creating TimeBasedInput");
				moduleVector.push_back(new TimeBasedInput());
			}
			else if (strcmp(moduleName, "BEGIN KEY_OUTPUT") == 0)
			{
				Log::println("[ModuleFactory]Creating KeyOutput");
				moduleVector.push_back(new KeyOutput());
			}
			else if (strcmp(moduleName, "BEGIN STATE_SAVER") == 0)
			{
				Log::println("[ModuleFactory]Creating StateSaver");
				StateSaver * module = new StateSaver(hDLL);
				moduleVector.insert(moduleVector.begin(),module);
				//special case!!, do the loading with the front
				StateObserver::setUpManager(module);

				loadModuleFromFile(module, configFile);
				continue;
			}

			//now give it the file to load
			loadModuleFromFile(moduleVector.back(), configFile);
		}
	}
}

void ModuleFactory::loadModuleFromFile(IO * module, FILEHANDLE configFile)
{
	char *line;
	char key[256];
	char value[256];
	//reads until EOF or "END"
	while (oapiReadScenario_nextline(configFile, line))
	{
		//ignores comments
		if (line[0] != ';')
		{
			//read the key and the value, then call the load on the module
			sscanf(line, "%255s = %255[^\n]", key, value);
			string s_key = key;
			std::transform(s_key.begin(), s_key.end(), s_key.begin(), ::tolower);
			//to lowercase

			//send the key and value to the module!
			module->load(s_key.c_str(), value);
		}
	}
}