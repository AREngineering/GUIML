#ifndef EVENT_H
#define EVENT_H

#include "GUIML.h"

// Enumerator defining the
// signals from the input that
// can be handled.
// Encoder: CWS, CCWS, BTN1
// ArrowButton: UP, DOWN, LEFT, RIGHT buttons + 4 other buttons
enum eventSignal_t
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  CWS,
  CCWS,
  BTN1,
  BTN2,
  BTN3,
  BTN4
};

typedef enum eventSignal_t eventSignal_t;



class Item;
class Event
{
  protected: 
    Item *pTarget; // Pointer to the target Menulib object
    void (*pfHandler)(Item*) = NULL; // Pointer to the function that has to handle the event
    eventSignal_t event_signal; // Identifier of the event that has been thrown

  public:
    Event(eventSignal_t evntSgnl);
    Event(eventSignal_t evntSgnl, Item* pTgt);
    void SetHandler(void (*pfHndlr)(Item*)) {pfHandler = pfHndlr;};
    void SetTarget(Item *pTrgt) {pTarget = pTrgt;};
    void ExecHandler(void);
    eventSignal_t EventSignal(void) {return event_signal;};
};

#endif
