/*
 * SelectionItem.h
 *
 *  Created on: 20 mar 2021
 *      Author: Utente
 */

#ifndef INC_SELECTIONITEM_H_
#define INC_SELECTIONITEM_H_

#include "GUIML.h"

/*
 * @SelectionItem
 * Virtual class that handles the interactive definition of a value
 * integer or float
 */
class SelectionItem: public Item
{
	protected:
		char    *explanation_line;
		char    *value_string;
		uint8_t  cursor_digit = 0;
		uint8_t  value_string_len = 0;
		uint8_t  char_activation = 1;
		uint8_t  char_activated = 0;
		uint8_t  integer = 1;
		virtual uint8_t ok_cursor() {return 1;}
		virtual uint8_t back_cursor() {return 2;};

		virtual void draw(void);

		/*
		 * Method to draw the explanatory line
		 * (e.g. "Input value for xyz variable")
		 */
		void draw_description(void);

		/*
		 * Method to draw the interactive line where the value is
		 * specified
		 */
	    virtual void draw_value_line(void);

	    /*
	     * Method to draw the interactive line where
	     * the "OK" and "BACK" selections are available
	     */
	    virtual void draw_ok_back(void);

	    /*
	     *
	     */
		virtual void increase(void);
		virtual void decrease(void);
		virtual void increase_digit_char(void);
		virtual void decrease_digit_char(void);
		virtual void activate_char(void);

	public:
		SelectionItem(char *label, char *expl, uint8_t vlen, uint8_t intf, uint8_t charf=1, uint8_t base_flag=0);
		~SelectionItem(void);
		void SetOK(Item* pOk_itm, tItemHandler pfOk);
		void SetBack(Item* pBack_itm, tItemHandler pfBack);
		uint16_t iValue(void);
		double   dValue(void);
		uint8_t  Integer(void){return integer;};

		static void Increase(SelectionItem* obj){obj->increase();}
		static void Decrease(SelectionItem* obj){obj->decrease();}
		static void Select(SelectionItem* obj){obj->select();}

		virtual void Content(char *output);

		/*
		 * Static Member that will be passed as handler reference to activate and deactivate the
		 * changing character mode.
		 */
		static void ActivateChar(SelectionItem *obj);
		static void Reset(SelectionItem* obj);
		static Interface* CreateTypicalEncoderInterface(SelectionItem *pItm);

};



#endif /* INC_SELECTIONITEM_H_ */
