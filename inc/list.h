/*
 * Guiml_Menu.h
 *
 *  Created on: 13 Oct 2021
 *      Author: revel
 */

#ifndef LIBRARY_INC_GUIML_LIST_H_
#define LIBRARY_INC_GUIML_LIST_H_

#include <item.h>



/*
 *  Menu Class
 *  Derived from item class
 *  Class that allows the user to select between a list of strings
 */
namespace guiml
{
	class list : public item
	{
	protected:
		// pointer to the list of strings (array of arrays of zero terminated characters)
		char **items=NULL;

		// external pointer provided
		// the list must be considered as const and not modified
		// call of add_element member will stop the machine.
		uint8_t external_pointer_provided = 0;

		// member containing the index of the element of the list to be selected.
		uint8_t cursor = 0;

		uint8_t number_of_items = 0;

		// index of the selected item (255 if not selected)
		uint8_t selected_idx = 255;

		// pointer to the external array of characters where a copy of the selection will be
		// stored
		char **pSelected = NULL;

		// pointer to the uint8_t variable storing the index of the selected element of the list
		uint8_t *pSelected_index = NULL;

		// method called to select an element
		void select(void);

		// methods to increase and decrease the cursor
		void increase(void);
		void decrease(void);

		// empty methods for the forward and backward signals.
		void forward(void){;}
		void backward(void){;}

		// method called when the back option is selected by the user;
		void back(void);

		// method that draws the page.
		void draw(void);



	public:
		// Constructors
		list(const char *name, U8G2 *pGraf , uint8_t* fnt, item* bs=NULL, item* prnt = NULL);
		list(const char *name, U8G2 *pGraf , uint8_t* fnt, char **pSel , item* bs=NULL, item* prnt = NULL);
		list(const char *name, U8G2 *pGraf , uint8_t* fnt, uint8_t *pSel_idx , item* bs=NULL, item* prnt = NULL);
		list(const char *name, U8G2 *pGraf , uint8_t* fnt, char **pSel , const char **pList , uint8_t length,  item* bs=NULL, item* prnt = NULL);
		list(const char *name, U8G2 *pGraf , uint8_t* fnt, uint8_t *pSel_idx , const char** pList, uint8_t length,  item* bs=NULL, item* prnt = NULL);

		// Destructor
		~list();

		// Add an element to the list of strings.
		void add_element(const char *itm);

		// Remove last item of the list.
		void remove_item(void);

		// Remove the item of the list of index i.
		void remove_item(uint8_t i);

		// Get methods for the selected_idx and pSelected members;
		uint8_t get_selected_idx(void) {return selected_idx;}
		//void set_pSelected(char **pSel);

		// Methods called bz the parent object when a forward or backward signal are received and this object in under cursor.
		virtual void receive_forward(void);
		virtual void receive_backward(void);

		// Method called when the selection in done by the user
		virtual void on_selection(void);

		// Method called when the user select to go back to the parent.
		virtual void on_leave(void);


		// Static method used to implement the interface.
		static void trigger_select(item* itm){((list*) itm)->select();}
		static void trigger_increase(item* itm){((list*) itm)->increase();}
		static void trigger_decrease(item* itm){((list*) itm)->decrease();}
		static void trigger_forward(item* itm){((list*) itm)->forward();}
		static void trigger_back(item* itm){((list*) itm)->back();}

		// get method to retrieve the selected content of this page.
		virtual char* get_content(void);
	};
}


#endif /* LIBRARY_INC_GUIML_MENU_H_ */

