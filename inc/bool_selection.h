/*
 * Guiml_Bool.h
 *
 *  Created on: 9 dic 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_BOOL_H_
#define LIBRARY_INC_GUIML_BOOL_H_


#include <item.h>

/*
 * Bool variable selection Class
 * Class that allows the user to select if a particular bool parameter
 * is set or not. The parameter is external to the library, is coded by
 * who is implementing the GUI and is related to the functionality implemented
 * in the GUI itself.
 */
namespace guiml
{
	class bool_selection : public item
	{
		protected:
		// member that is consistent with the value of the
		// variable, that will be written on it at selection.
		uint8_t cursor = 0;
		// pointer to the external variable where the value of the
		// bool parameter is saved.
		uint8_t *pValue;

		// member that toggle the value between the true and false
		void toggle(void);

		// members that set true and false
		void set_true(void);
		void set_false(void);

		// method for the interaction with the page.
		void select(void);
		void increase(void);
		void decrease(void);

		// method for drawing the page.
		void draw(void);



		public:
		// Constructor
		bool_selection(const char *name, U8G2 *pGraf, uint8_t *main_font, uint8_t *pVal, item* bs=NULL, item* prnt = NULL);
		// Destructor
		~bool_selection(){;}

		// Method that receive a signal from the parent object to go forward or backward
		virtual void receive_forward(void);
		virtual void receive_backward(void){receive_forward();}

		// Inactive methods that are called in selection and leave.
		virtual void on_selection(void);
		virtual void on_leave(void);

		// Static trigger methods that are used as handlers in the implementation of the interface
		static void trigger_increase(item* itm){((bool_selection*) itm)->increase();}
		static void trigger_decrease(item* itm){((bool_selection*) itm)->decrease();}
		static void trigger_select(item* itm){((bool_selection*) itm)->select();}

		// Method that reports the get_content
		virtual char* get_content(void);

		/*
		 * Methods to specify the pointers external functions to
		 * operate changes on global variables when on leave from the object
		 * or on selection of a new object.
		 */
		void set_pF_on_selection(handler_t pFH){pFon_selection = pFH;};
		void set_pF_on_leave(handler_t pFH){pFon_leave = pFH;};

	};
}


#endif /* LIBRARY_INC_GUIML_BOOL_H_ */
