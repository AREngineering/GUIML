/*
 * Guiml_Generic.h
 *
 *  Created on: 23 dic 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_GENERIC_H_
#define LIBRARY_INC_GUIML_GENERIC_H_


#include <item.h>

/*
 * Generic Class
 * It is a wrapper class that allows to have a interface page integrated with the GUIML GUI
 * and to use its signal management
 */
namespace guiml
{
	class generic : public item
	{
		protected:
		// Function pointer to the external drawing function
		// coded by the implementer of the GUI
		void (*pFdraw)(void) = NULL;

		// Function that calls the pFdraw pointer.
		void draw(void);

		public:
		// Constructor
		generic(const char *name, U8G2 *pGraf, uint8_t *font, void(*pFdrw)(void), item* bs=NULL, item* prnt = NULL);
		// Destructor
		virtual ~generic(void){;}

		// All these virtual functions are non-active.
		virtual void receive_forward(void){;};
		virtual void receive_backward(void){;};
		virtual void on_selection(void);
		virtual void on_leave(void);
		virtual char* get_content(void){return NULL;}

	};
}

#endif /* LIBRARY_INC_GUIML_GENERIC_H_ */
