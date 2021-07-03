/*
 * SelectionUnitItem.h
 *
 *  Created on: 7 giu 2021
 *      Author: Utente
 */

#ifndef SELECTIONUNITITEM_H_
#define SELECTIONUNITITEM_H_

#include "GUIML.h"

class SelectionUnitItem : public SelectionItem
{
	protected:
	uint8_t unit_idx=255;
	uint8_t units_length = 0;
	char**  units_list = NULL;

	MenuList *units;
	virtual uint8_t unit_cursor() {return 1;}
	virtual uint8_t ok_cursor() {return 2;}
	virtual uint8_t back_cursor() {return 3;};

	/*
	 * Method to draw the interactive line where the value is
	 * specified
	 */
	virtual void draw_value_line(void);
	virtual void increase(void);
	virtual void decrease(void);

	public:
	SelectionUnitItem(char *label, char *expl, char **unit_lst, uint8_t unit_lsl_len, uint8_t vlen, uint8_t intf, uint8_t charf=1, uint8_t base_flag=0);
	void Unit(char* output);
	virtual void Content(char *output);

	static void Increase(SelectionUnitItem* obj){obj->increase();}
	static void Decrease(SelectionUnitItem* obj){obj->decrease();}
	static void Select(SelectionUnitItem* obj){obj->select();}
	static void Reset(SelectionUnitItem* obj);

	static Interface* CreateTypicalEncoderInterface(SelectionUnitItem *pItm);
};



#endif /* SELECTIONUNITITEM_H_ */
