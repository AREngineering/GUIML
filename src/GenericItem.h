/*
 * GenericItem.h
 *
 *  Created on: 8 giu 2021
 *      Author: Utente
 */

#ifndef GENERICITEM_H_
#define GENERICITEM_H_

#include "GUIML.h"

class GenericItem : public Item
{
protected:
	void (*pFdraw)(GenericItem*)   = NULL;

	uint8_t cursor_lr = 0;

	void draw(void);

public:
	GenericItem(char* label,uint8_t base_flag=0):Item(label,base_flag){;}

	void SetPFDraw(void (*pF)(GenericItem*)){pFdraw = pF;};

	void SetUpItem   (Item* itm, tItemHandler pF=NULL);
	void SetDownItem (Item* itm, tItemHandler pF=NULL);
	void SetLeftItem (Item* itm, tItemHandler pF=NULL);
	void SetRightItem(Item* itm, tItemHandler pF=NULL);
	void SetBtn1Item (Item* itm, tItemHandler pF=NULL);
	void SetBtn2Item (Item* itm, tItemHandler pF=NULL);
	void SetBtn3Item (Item* itm, tItemHandler pF=NULL);
	void SetBtn4Item (Item* itm, tItemHandler pF=NULL);

	void GoToUpItem(void);
	void GoToDownItem(void);
	void GoToLeftItem(void);
	void GoToRightItem(void);
	void GoToBtn1Item(void);
	void GoToBtn2Item(void);
	void GoToBtn3Item(void);
	void GoToBtn4Item(void);

	static void Up(Item *);
	static void Down(Item *);
	static void Left(Item *);
	static void Right(Item *);
	static void Btn1(Item *);
	static void Btn2(Item *);
	static void Btn3(Item *);
	static void Btn4(Item *);

	virtual void Content(char *output){strcpy(output,"");};
};



#endif /* GENERICITEM_H_ */
