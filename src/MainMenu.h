/*
 * MainMenu.h
 *
 *  Created on: 26 mar 2021
 *      Author: Utente
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "GUIML.h"

/*
 * @MainMenu
 * is the class that allow to create main settings menu whose display
 * allows the user to navigate between menulib objects
 * The menulib objects addresses are stored in the pElements array.
 * When the user select an element of the pElements array the menu goes
 * to the stored address.
 */
class MainMenu : public MenuItem
{
	protected:
		tItemHandler pFNext_handler = NULL;
		virtual void get_line_text(uint8_t i, char* output, uint8_t selected = 0);
		//virtual void draw(void);

	public:
		MainMenu(char* label,Item* pBack_Itm, tItemHandler back_hdlr, tItemHandler next_hdlr, uint8_t base_flag=0);
		virtual void AddElement(char* in_str, Item *pEl, tItemHandler pHdlr);

};

#endif /* MAINMENU_H_ */
