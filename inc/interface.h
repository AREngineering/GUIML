/*
 * GUIML_Interface.h
 *
 *  Created on: 24 nov 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_INTERFACE_H_
#define LIBRARY_INC_GUIML_INTERFACE_H_

#include <parameters.h>
#include <event.h>
#include <item.h>

namespace guiml
{
	class event;

	/*
	 * Interface Class
	 * It handles the signals that come from the application using the GUI as interface.
	 */
	class interface
	{
		private:
		item *pOwner = NULL; // pointer the item object owning this interface object
		event **pEvents;     // array of pointers (dyn allocated) to event objects
		uint8_t event_number = 0; // number of event objects in the array

		public:
		interface();
		~interface();
		void set_owner(item* ownr);
		void add_event(event *evnt);
		void receive_event(const char *signal);
	};
}



#endif /* LIBRARY_INC_GUIML_INTERFACE_H_ */
