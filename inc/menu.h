/*
 * Guiml_Menu.h
 *
 *  Created on: 13 Oct 2021
 *      Author: revel
 */

#ifndef LIBRARY_INC_GUIML_MENU_H_
#define LIBRARY_INC_GUIML_MENU_H_


#include <item.h>

/*
 *  Menu Class
 *  Derived from item class
 *  Class that allows the user to select between a set of sub-pages of the GUI.
 */
namespace guiml
{
	class menu : public item
	{
	protected:
		// Array of GUI pages objects, dynamically allocated.
		item **items;

		// Number of GUI pages in the array
		uint8_t number_of_items = 0;

		// Cursor that indicates which object will be selected when the select signal is
		// sent.
		uint8_t cursor = 0;

		// Method that operates the selection of the sub GUI page
		void select(void);

		// Method that increases the cursor.
		void increase(void);

		// Method the decrease the cursor.
		void decrease(void);

		// Method that gives to a sub GUI page under cursor the command to step up its cursor if it has it.
		void forward(void);

		// Method that gives to a sub GUI page under cursor the command to step down its cursor if it has it.
		void backward(void);

		// Method that moves the GUI to the parent page
		void back(void);

		// Method that draw this menu page.
		virtual void draw(void);


	public:
		// Constructor
		menu(const char *name, U8G2 *pGraf, uint8_t* fnt, item* bs=NULL, item* prnt = NULL);

		// Destructor
		~menu();

		// Method that add a GUI page link to the menu
		void add_element(item* pItem);
		void add_element(uint8_t i, item* pItem);

		// Methods that remove a GUI page link from the menu
		void remove_element(void);
		void remove_element(uint8_t i);
		void remove_element(const char *lbl);

		// Methods used to receive the forward and backward command (do nothing)
		virtual void receive_forward(void){;}
		virtual void receive_backward(void){;};

		// Methods called when a selection is done used to set or reset
		// private member.
		virtual void on_selection(void);
		virtual void on_leave(void);

		// Static methods used as handlers function for the significant operations of the
		// menu page.
		static void trigger_select(item* itm){((menu*) itm)->select();}
		static void trigger_increase(item* itm){((menu*) itm)->increase();}
		static void trigger_decrease(item* itm){((menu*) itm)->decrease();}
		static void trigger_forward(item* itm){((menu*) itm)->forward();}
		static void trigger_backward(item* itm){((menu*) itm)->backward();}
		static void trigger_back(item* itm){((menu*) itm)->back();}

		// No content is provided by this class (used only for navigation)
		virtual char* get_content(void){return NULL;};

		uint8_t get_cursor(){return cursor;}
	};
}


#endif /* LIBRARY_INC_GUIML_MENU_H_ */

