/*
 * MainMenu.cpp
 *
 *  Created on: 26 mar 2021
 *      Author: Utente
 */

#include "menulib.h"

/*
 * Constructor
 */
MainMenu::MainMenu(char* label,Item* pBack_itm, tItemHandler back_hdlr, tItemHandler next_hdlr, uint8_t base_flag):
MenuItem(label,pBack_itm, back_hdlr,base_flag)
{
	pFNext_handler = next_hdlr;
}


/*
 * @AddElement
 * Calls Item::AddElement and then updates list_length with elements_number
 */
void MainMenu::AddElement(char* in_str, Item *pEl, tItemHandler pHdlr)
{
	if(pHdlr!=NULL)
	{
		MenuItem::AddElement(in_str, pEl, pHdlr);
	}
	else if(pFNext_handler != NULL)
	{
		MenuItem::AddElement(in_str, pEl, pFNext_handler);
	}
	else
	{
		MenuItem::AddElement(in_str, pEl, NULL);
	}
}

/*
 * @get_line_text
 * Method that produces on the output the label of the
 * menulib item indexed in pElements by i or the "Back"
 * string
 */
void MainMenu::get_line_text(uint8_t i, char* output, uint8_t selected)
{
	(void) selected;

	//Serial.println("In MainMenu::get_line_text");
	if(pElements[i]!=NULL)
	{
		pElements[i]->Text(output);
	}
	else
	{
		while(1);
		// ERROR
	}
}


