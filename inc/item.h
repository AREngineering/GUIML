/*
 * Guiml_Item.h
 *
 *  Created on: 9 ott 2021
 *      Author: AR_Engineering
 */

#ifndef LIBRARY_INC_GUIML_ITEM_H_
#define LIBRARY_INC_GUIML_ITEM_H_


#include <auxiliary.h>
#include <parameters.h>
#include <event.h>
#include <interface.h>

/*
 * CLASS: ITEM
 * Virtual root class from which all other classes are derived through inheritance.
 * Menus and GUIs are implemented as a connected chain of specific pages with specific functionality.
 * Each specific page has a dedicated class with methods that implement its visualization and functionalities.
 */

namespace guiml
{
	class interface;
	class item
	{
		protected:
		char *label           = NULL; // Name of the objects
		char *content         = NULL; // String that reports the selected content of the page
		uint8_t is_base       = 0;    // Flag that indicates if the object implements the root page of the GUI.
		item *pSelected       = NULL; // Pointer to the object that is being visualized
		item *pBase           = NULL; // Pointer to the root object of the GUI
		item *pParent         = NULL; // Pointer to the object from which the current object has been selected by the userf.

		interface *pInterface = NULL; // Pointer to the interface object that handles the signals

		U8G2 *pU8G2lib; // Pointer to the graphic library U8G2
		uint8_t *font;  // Pointer to the U8G2 font index

		virtual void draw()=0;

		uint8_t ascending = 1;           // flag used in the visualization routines draw_sliding_string() that
		                                 // indicates the direction of the sliding of the string.

		uint16_t start_index = 0;        // member used in the visualization routines draw_sliding_string() that
		                                 // indicates which is the index of the first character to be visualized

		uint8_t update_start_index = 0;  // flag used in the visualization routines draw_sliding_string() that
		                                 // indicates if the start_index must be increased or decreased consistently
		                                 // with the ascending member value.

		/*
		 * Methods that visualizes a string as sliding in the string.
		 */
		void draw_sliding_string(int16_t xmin, int16_t ymin, int16_t xmax, const char* in);
		void draw_sliding_string(int16_t xmin, int16_t ymin, const char* in);
		void draw_sliding_string(const char* in);

		/*
		 * Methods that visualizes a string breaking it on multiple lines
		 */
		void draw_split_string(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, const char *in);
		void draw_split_string(int16_t xmin, int16_t ymin, const char *in);
		void draw_split_string(const char *in);

		/*
		 * Methods that visualizes a string limiting it to a single line
		 */
		void draw_cut_string(int16_t xmin, int16_t ymin, int16_t xmax, const char* in);
		void draw_cut_string(int16_t xmin, int16_t ymin, const char* in);
		void draw_cut_string(const char* in);

		/* Function Pointers to allow the user to run operations on global variables on selection and leave*/
		handler_t pFon_selection = NULL;
		handler_t pFon_leave = NULL;

		/*
		 * Pointers to GUIML objects to be selected on selection and on leave
		 */
		item* pItem_on_selection = NULL;
		item* pItem_on_leave = NULL;

		public:

		/*
		 * Constructor & destructor
		 */
		item(const char *name, U8G2 *pGraf, uint8_t *main_font, item* bs=NULL, item* prnt = NULL);
		virtual ~item();

		/*
		 * Method that returns a string pointer allocated in the heap memory.
		 * This string must be destroyed when no more in use.
		 */
		char* get_label(void);

		/*
		 * Method that sets the pParent member.
		 */
		void set_parent(item *pObj);

		/*
		 * Method that sets the pSelected member
		 */
		void set_selected(item *pObj);

		/*
		 * Method that adds a event handler for a specific signal.
		 */
		void add_event(const char* sig_name, handler_t pHandler);

		/*
		 * Method that the root object uses to signal to the selected object interface
		 * (pBase->pSelected->pInterface member) the arriving of the signal indicated by the string *signal
		 */
		void send_signal(const char *signal);

		/*
		 * Method that the selected object uses to receive the signal.
		 */
		void receive_signal(const char *sgn);


		/*
		 * Virtual methods to activate particular functionalities
		 */
		virtual void receive_forward(void)=0;
		virtual void receive_backward(void)=0;
		virtual void on_selection(void)=0;

		virtual void on_leave(void)=0;


		/*
		 * Method used to activate the visualization
		 */
		void activate(void);

		/*
		 * Virtual Method used to get the content string.
		 */
		virtual char* get_content(void)=0;

		/*
		 * Methods to specify the pointers external functions to
		 * operate changes on global variables when on leave from the object
		 * or on selection of a new object.
		 */
		void set_pF_on_selection(handler_t pFH){pFon_selection = pFH;};
		void set_pF_on_leave(handler_t pFH){pFon_leave = pFH;};

		/*
		 * Methods to set the pointers to other GUIML objects
		 * to be selected on selection or on leave.
		 */
		void set_pItem_on_selection(item *pItm){pItem_on_selection = pItm;};
		void set_pItem_on_leave(item *pItm){pItem_on_selection = pItm;};

	};
}



#endif /* LIBRARY_INC_GUIML_MENU_H_ */
