/*
 * Guiml_Event.h
 *
 *  Created on: 24 nov 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_EVENT_H_
#define LIBRARY_INC_GUIML_EVENT_H_

#include <parameters.h>
#include <item.h>

/*
 * Event Class
 * Handles the signal activating the handler function when the proper signal is
 * received.
 */
namespace guiml
{
	class event
	{
		public:
		// Constructor
		event(const char* nm, handler_t callback, item *tgt);
		// Destructor
		~event();
		// if signal string is equal to name member activate the handler.
		uint8_t activate(const char *signal);

		private:
		char *name = NULL; //name of the signal
		// Function pointer to the handler for the signal.
		handler_t pFHandler = NULL;
		// GUI object on which the handler will operate on.
		item *target = NULL;

	};
}

#endif /* LIBRARY_INC_GUIML_EVENT_H_ */
