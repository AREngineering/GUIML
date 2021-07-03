#include "menulib.h"

Event::Event(eventSignal_t evntSgnl)
{
	pTarget = NULL;
	event_signal = evntSgnl;
}

Event::Event(eventSignal_t evntSgnl, Item* pTgt)
{
	pTarget = pTgt;
	event_signal = evntSgnl;
}

/*
 * ExecHandler
 * Activate the Handler Function.
 */
void Event::ExecHandler(void)
{
	if(pfHandler != NULL && pTarget!=NULL)
	{
		pfHandler(pTarget);
	}
	else
	{
		while(1)
		{
			Serial.println("Error in pfHandler. Locked");
		}
	}
}
