//Copyright (c) 2013 Christopher Johnstone(meson800)
//The MIT License - See ../../../LICENSE for more info

#include "PanelClickRecorderOutput.h"

void PanelClickRecorderOutput::load(const char * key, const char * value)
{
	//inputFile = this->readToSectionStart(inputFile, "PANEL_CLICK_RECORDER");
	char c_vesselName [256];
	char c_logName [256];
	char c_recordLogName [256];
	//now see if it is our vessel name
	if (strcmp(key, "vessel_name") == 0)
	{
		sscanf(value,"%s", c_vesselName);
		vesselName = c_vesselName;
	}

	//record the starting record id if it is there
	if (strcmp(key, "starting_record_id") == 0)
		sscanf(value,"%i",&currentUserDefId);

	//see if recording should be enabled
	if (strcmp(key, "auto_record") == 0)
	{
		int test_int;
		sscanf(value, "%i", &test_int);
		if (test_int == 1)
		{
			record = true;
		}
	}

	//read log file info, if they want to change the defaults
	if (strcmp(key, "event_log_name") == 0)
	{
		sscanf(value,"\"%254[^\"]\"",c_logName);
		logName = c_logName;
	}

	//print a starter so we can see where a session begins
	FILE * logFile = fopen(logName.c_str(),"a");
	if (logFile != NULL)
	{
		fprintf(logFile,"Event logging started\n");
		fclose(logFile);
	}

	//read record output info
	if (strcmp(key, "recording_log_name") == 0)
	{
		sscanf(value,"\"%254[^\"]\"",c_recordLogName);
		recordLogName = c_recordLogName;
	}


	FILE * recordFile = fopen(recordLogName.c_str(),"a");
	if (recordFile != NULL)
	{
		fprintf(recordFile,"Recording started\n");
		fclose(recordFile);
	}
}

void PanelClickRecorderOutput::SimulationStart()
{
	OBJHANDLE hVessel = oapiGetVesselByName((char *)vesselName.c_str());
	if (oapiIsVessel(hVessel))
	{
		InstallVesselHook((VESSEL2 *)oapiGetVesselInterface(hVessel));
		observer.setUpReciever(this);
		
	}
}

void PanelClickRecorderOutput::handlePanelMouseEvent(int id, int ev, int mx, int my)
{
	if (record)
	{
		recordingQueue.push(new PanelMouseEvent(id,ev,mx,my));
	}
		
	//log it
	FILE * logFile = fopen(logName.c_str(),"a");
	if (logFile != NULL)
	{
		fprintf(logFile,"%d,%d,%d,%d\n",id,ev,mx,my);
		fclose(logFile);
	}
}


void PanelClickRecorderOutput::handleEvent(Event ev)
{
	//pop from queue, write to file
	if (record && recordingQueue.size() > 0)
	{
		FILE * recordFile = fopen(recordLogName.c_str(),"a");
		if (recordFile != NULL)
		{
			//we have to output two things, first a user definition.
				//User def is in the format 
				//defId	(groups of definitions)
				//Each definition is
				//	eventState	mx	my	mouseEvent
			//then we have to output a event mapping
				//mapping is in the format
				//eventId defId clickId

			//we get eventId and eventState from the event from handleEvent
			//we get the four values, clickId, mx, my, and mouseEvent from the queue
			//We need a recorder variable to record a new defId

			//get our top mouseEvent
			PanelMouseEvent * mouseEvent = recordingQueue.front();

			//first set up the user definition
			fprintf(recordFile,"user_def = %i %i %i %i %i\n",currentUserDefId, ev.state, mouseEvent->mx,
				mouseEvent->my, mouseEvent->mouseEvent);

			//now print out the event mapping
			fprintf(recordFile,"event = %i %i %i\n", ev.id, currentUserDefId, mouseEvent->id);
			fclose(recordFile);

			//increment currentUserDefId so we don't overwrite it and clean up
			currentUserDefId++;
			
			delete mouseEvent;
			recordingQueue.pop();
		}
	}


}

void PanelClickRecorderOutput::FocusChanged(OBJHANDLE old_focus, OBJHANDLE new_focus)
{

}

void PanelClickRecorderOutput::SimulationEnd()
{
	CleanUpHooking();
}