/*
 * MenuList.cpp
 *
 *  Created on: 23 mar 2021
 *      Author: Utente
 */

#include "menulib.h"

MenuList::MenuList(char* label, char** pL, uint8_t len, uint8_t *idx, Item* pBack_itm, tItemHandler back_hdlr, Item* pNext_itm, tItemHandler next_hdlr, uint8_t base_flag):MenuItem(label,pBack_itm,back_hdlr,base_flag)
{
	pNext_item = pNext_itm;
	pFNext_handler = next_hdlr;

	//Serial.print("pNext_item:");Serial.println((int)pNext_item);
	//Serial.print("pFNext_handler:");Serial.println((int)pFNext_handler);
	//Serial.print("pParent:");Serial.println((int )pParent);
	for(uint8_t i = 0; i < len; i++)
	{
		if(pNext_item != NULL)
			AddElement(pL[i], pNext_item, pFNext_handler);
		else
			AddElement(pL[i], NULL, pFNext_handler);
	}

	//Serial.println("In MenuList: units");
	//Serial.print("elements_number: ");Serial.println(elements_number);

//	for(uint8_t i = 0; i<elements_number; i++)
//	{
//		Serial.print("pElements[] "); Serial.print(i); Serial.print(" : "); Serial.println((unsigned int) pElements[i]);
//	}

	pSelected_cursor = idx;
}


char* MenuList::SelectedString(void)
{
	//Serial.print("*pSelected_cursor: ");Serial.println(*pSelected_cursor);
	//Serial.print("pList[*pSelected_cursor]: "); Serial.println(pList[*pSelected_cursor]);
	return pTextElements[*pSelected_cursor];
}

void MenuList::select(void)
{
	if(IsHome())
		*pSelected_cursor = cursor; //If Home object Back selection is not produced
	else if(cursor!=elements_number-1)
		*pSelected_cursor = cursor; //On Back selection the cursor is not copied.

	Item::select();
}


void MenuList::set_parent(Item *pPar)
{
	Item::set_parent(pPar);
	for(uint8_t i=0;i<elements_number;i++)
	{
		if(pNext_item == NULL && pElements[i] == NULL)
		{
			pElements[i] = pPar;
		}
	}
}
