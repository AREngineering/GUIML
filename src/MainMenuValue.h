/*
 * MenuListValue.h
 *
 *  Created on: 27 mar 2021
 *      Author: Utente
 */

#ifndef MAINMENUVALUE_H_
#define MAINMENUVALUE_H_

#include "GUIML.h"

class MainMenuValue : public MainMenu
{
protected:
	virtual void get_line_text(uint8_t i, char* output, uint8_t selected = 0);

public:
	MainMenuValue(char* label,Item* pBack_Itm, tItemHandler back_hdlr, tItemHandler next_hdlr, uint8_t base_flag=0):MainMenu(label,pBack_Itm,back_hdlr,next_hdlr,base_flag){;};

};




#endif /* MAINMENUVALUE_H_ */
