/*
 * MenuItem.h
 *
 *  Created on: 22 mar 2021
 *      Author: Utente
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include "GUIML.h"

/*
 * @MenuItem
 * base menulib class for menu display
 * The interaction allow to select between a
 * list of possible items.
 * virtual class, will be specialized by
 * the children class.
 */
class MenuItem : public Item
{
	protected:
		tItemHandler pFBack_handler = NULL;
		Item*        pBack_item = NULL;
		virtual void get_line_text(uint8_t i, char* output, uint8_t selected = 0);
		virtual void draw(void);

	public:
		MenuItem(char* label,uint8_t base_flag=0):Item(label,base_flag){;};
		MenuItem(char* label,Item* pItm, tItemHandler hdlr, uint8_t base_flag=0);
		void SetBackHandler(tItemHandler hdlr){pFBack_handler = hdlr;};
		void SetBackItem(Item* pItm){pBack_item = pItm;};
		void Content(char *output){strcpy(output,"->");};
		virtual void AddElement(char* in_str, Item *pEl, tItemHandler pHdlr);
		virtual void OnSelection(void){cursor=0;};
};



#endif /* MENUITEM_H_ */
