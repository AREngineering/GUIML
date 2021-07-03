/*
 * MenuList.h
 *
 *  Created on: 23 mar 2021
 *      Author: Utente
 */

#ifndef MENULIST_H_
#define MENULIST_H_

#include "GUIML.h"

/*
 * @MenuList
 * Menu class where you select between
 * the elements of an array of char[] strings.
 */
class MenuList : public MenuItem
{
	protected:
		/*
		 * pointer to the address where
		 * the index of the selected element
		 * is saved
		 */
		tItemHandler pFNext_handler = NULL;
		Item*        pNext_item = NULL;

		uint8_t *pSelected_cursor = NULL;
		virtual void set_parent(Item *pPar);
		virtual void select(void);

	public:
		MenuList(char* label, char** pL, uint8_t len, uint8_t *idx, Item* pBack_itm, tItemHandler back_hdlr, Item* pNext_itm, tItemHandler next_hdlr, uint8_t base_flag=0);
		char* SelectedString(void);
};


#endif /* MENULIST_H_ */
