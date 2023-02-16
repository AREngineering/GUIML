/*
 * Guiml_Generic.cpp
 *
 *  Created on: 23 dic 2021
 *      Author: Utente
 */


#include <generic.h>

/*
 * Constructor to the generic wrapper class
 */
guiml::generic::generic(const char *name, U8G2 *pGraf, uint8_t *font, void(*pFdrw)(void), item* bs, item* prnt):
	item(name, pGraf, font, bs, prnt)
{
	pFdraw = pFdrw;
}

/*
 * drawing method
 */
void guiml::generic::draw(void)
{
	if(pFdraw!=NULL)
		pFdraw();
}

/*
 * void on_selection()
 * reset the cursor to 0 when an object in the array is selected.
 * User can specify a pointer to an external function to operate changes on global variables on selection.
 */
void guiml::generic::on_selection(void)
{
	if(pFon_selection != NULL)
	{
		pFon_selection(this);
	}

	if(pItem_on_selection !=NULL)
		pBase->set_selected(pItem_on_selection);
}

/*
 * void on_leave()
 * reset the cursor to 0 when the user returns to the parent object.
 * User can specify a pointer to an external function to operate changes on global variables on leave.
 */
void guiml::generic::on_leave(void)
{
	if(pFon_leave != NULL)
	{
		pFon_leave(this);
	}

	if(pItem_on_leave != NULL)
			pBase->set_selected(pItem_on_leave);
}
