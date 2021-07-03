#include "menulib.h"

/*
 * @AddEvent
 * Add an Event pointer to the pEvents array
 *
 * arg[0]: evnt - event pointer to be added
 */
void Interface::AddEvent(Event *evnt)
{
  if(ev_counter+1 < MAX_EVENTS)
  {
    pEvents[ev_counter] = evnt;
    pEvents[ev_counter]->SetTarget(pTarget);
    ev_counter++;
    //Serial.print("ev_counter: ");Serial.println(ev_counter);
  }
  else
  {
    while(1);  
  }
}

/*
 * @AddEventList
 * Add multiple events contained in an input array, to the
 * pEvents array member.
 *
 * arg[0]: evnts - array of Event pointer to be added
 * arg[1]: len - lenght of the input array
 */
void Interface::AddEventList(Event **evnts,uint8_t len)
{
  if(ev_counter+len < MAX_EVENTS)
  {
    for(uint8_t i=0;i < len;i++)  
    {
      pEvents[ev_counter] = evnts[i];
      pEvents[ev_counter]->SetTarget(pTarget);
      ev_counter++;
    }
  }
  else
  {
    while(1);
  }
}

/*
 * @SelectEvent
 * Member that returns the event that responds to a
 * specific eventSignal_t (enumerator).
 *
 * arg[0]: evSgnl - input signal
 * return: pointer to the Event object handling the input signal
 */
Event* Interface::SelectEvent(eventSignal_t evSgnl)
{ 
	Event* evnt = NULL;
	//Serial.print("pInterface: "); Serial.println((unsigned long) this);
	//Serial.print("evSgnl: ");Serial.println(evSgnl);
	//Serial.print("ev_counter: ");Serial.println(ev_counter);
	for(uint8_t i=0; i < ev_counter;i++)
	{
		//Serial.print("pEvents ");Serial.print(i);Serial.print(" - "); Serial.println(pEvents[i]->EventSignal());
		if(pEvents[i]->EventSignal() == evSgnl)
		{
			evnt = pEvents[i];
		}
	}
	return evnt; // If lbl found return is a valid address otherwise return NULL
}


/*
 * @SetTarget
 * Set the menulib object target for this
 * Interface object, and for all Event object
 * linked in the pEvent array
 *
 * arg[0]: pTrgt - pointer to the target menulib object
 *
 */
void Interface::SetTarget(Item *pTrgt)
{
  pTarget = pTrgt;
  for(uint8_t i=0;i<ev_counter;i++)
  {
    pEvents[i]->SetTarget(pTrgt);
  }
  //Serial.print("pTarget: "); Serial.println((unsigned long) pTarget);
}
