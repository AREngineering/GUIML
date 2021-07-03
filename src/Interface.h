#ifndef INTERFACE_H
#define INTERFACE_H

#include "GUIML.h"

#define MAX_EVENTS 8

class Item;
class Event;


/* Class that defines members and methods
 * used to handle the various events coming
 * from the input devices (buttons, encoders, etc.)
 */
class Interface
{
  protected:
	// Pointer to the menulib object to
	// which the event signal must be routed
    Item* pTarget;
    // Array of pointers to Event class objects
    // Each one contains the handler for managing
    // a particular signal from the input devices
    Event* pEvents[MAX_EVENTS];
    // Counter of the number of elements of the pEvents array
    uint8_t ev_counter =0; 

  public:
    // Constructor
    Interface(){pTarget=NULL;};
    void SetTarget(Item *pTrgt);
    Event* SelectEvent(eventSignal_t eS);
    void AddEvent(Event *evnt);
    void AddEventList(Event **evnt,uint8_t len);
};




#endif
