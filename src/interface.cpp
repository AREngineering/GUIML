/*
 * Guiml_Interface.cpp
 *
 *  Created on: 24 nov 2021
 *      Author: Utente
 */

#include <interface.h>

/*
 * Item constructor
 * events is allocated dynamically
 */
guiml::interface::interface()
{
	pEvents = (event**) malloc(sizeof(event*));
}

/*
 * Item destructor
 * events is allocated dynamically and therefore
 * heap memory must be freed dynamically.
 */
guiml::interface::~interface()
{
	for(uint8_t i=0; i< event_number; i++)
		delete(pEvents[i]);

	free(pEvents);
}

/*
 * void set_owner(ownr)
 * Method to set the owner member
 * Arguments:
 * - ownr: pointer to the owner object
 */
void guiml::interface::set_owner(item* ownr)
{
	if(pOwner == NULL)
		pOwner = ownr;
	else
		while(1); // Owner cannot be specified more than one time;
}

/*
 * void add_event(evnt)
 * Method that dynamically add an event object pointer to the pEvents array
 * Arguments:
 * - evnt: pointer to the event object to add
 */
void guiml::interface::add_event(event *evnt)
{

	// Check that there is no duplicated event in the
	// list
	for(uint8_t i =0; i<event_number ; i++)
	{
		if(pEvents[i] == evnt)
			while(1);
	}

	event_number++;
	pEvents = (event**) realloc(pEvents,sizeof(event*)*event_number);
	pEvents[event_number-1] = evnt;

}

/*
 * void receive_event(signal)
 * Method that receive a signal as argument and send to all the event objects linked in the
 * pEvents array that signal. If one of the events is the one handling that particular signal,
 * it will activate.
 * Argument:
 * - signal: string identifying the signal
 */
void guiml::interface::receive_event(const char *signal)
{
	for(uint8_t i=0; i<event_number;i++)
	{
		//Serial.println((unsigned int) events[i]);
		if(pEvents[i]->activate(signal))
		{
			break;
			//Serial.print("Signal ");Serial.print(signal);Serial.print(" accepted at ");Serial.println(i);
		}
		else
		{
			//Serial.print("Signal ");Serial.print(signal);Serial.print(" rejected at ");Serial.println(i);
		}
	}
}

 
