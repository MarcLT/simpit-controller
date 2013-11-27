#ifndef TIME_BASED_INPUT
#define TIME_BASED_INPUT

#include <vector>
#include <ctime>

#include "Input.h"
#include "Event.h"

#include "SimpitObserver.h"

//triggers an event when the difference between the time in the struct and the current time is greater than dt
struct TimeSpec
{
	TimeSpec(int _type, double _dt,Event _ev) : type(_type), dt(_dt), time(0), eventToFire(_ev) {}
	int type;	//type 0 = sim start, type 1 = sim time interval, type 2 = actual interval	
	double dt, time;
	Event eventToFire;
};


class TimeBasedInput : public Input
{
public:
	TimeBasedInput () {}
	void load (FILEHANDLE inputFile);
	void SimulationStart();
	void PostStep(double simt, double simdt, double mjd);

private:
	vector<TimeSpec> inputs;
};


#endif