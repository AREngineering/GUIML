/*
 * GenericItem.cpp
 *
 *  Created on: 8 giu 2021
 *      Author: Utente
 */

#include "menulib.h"

void GenericItem::Up(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;

	if(obj1->pFHandlers[0])
	{
		obj1->pFHandlers[0](obj);
	}
	obj1->GoToUpItem();
}

void GenericItem::Down(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[1] != NULL)
	{
		obj1->pFHandlers[1](obj);
	}
	obj1->GoToDownItem();
}

void GenericItem::Left(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[2] != NULL)
	{
		obj1->pFHandlers[2](obj);
	}
	obj1->GoToLeftItem();
}

void GenericItem::Right(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	//Serial.print("pFHandlers[3] address - ");Serial.println((unsigned int) obj1->pFHandlers[3]);
	//Serial.print("pElements[3] address - ");Serial.println((unsigned int) obj1->pElements[3]);
	delay(3000);
	if(obj1->pFHandlers[3] != NULL)
	{
		obj1->pFHandlers[3](obj);
	}
	obj1->GoToRightItem();
}

void GenericItem::Btn1(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[4] != NULL)
	{
		obj1->pFHandlers[4](obj);
	}
	obj1->GoToBtn1Item();
}

void GenericItem::Btn2(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[5] != NULL)
	{
		obj1->pFHandlers[5](obj);
	}
	obj1->GoToBtn2Item();
}

void GenericItem::Btn3(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[6] != NULL)
	{
		obj1->pFHandlers[6](obj);
	}
	obj1->GoToBtn3Item();
}

void GenericItem::Btn4(Item *obj)
{
	GenericItem *obj1 = (GenericItem*) obj;
	if(obj1->pFHandlers[7] != NULL)
	{
		obj1->pFHandlers[7](obj);
	}
	obj1->GoToBtn4Item();
}

void GenericItem::draw(void)
{
	if(pFdraw!=NULL)
	{
		pFdraw(this);
	}
}


void GenericItem::SetUpItem(Item* itm, tItemHandler pF)
{
	pElements[0] = itm;
	pFHandlers[0] = pF;
}

void GenericItem::SetDownItem(Item* itm, tItemHandler pF)
{
	pElements[1] = itm;
	pFHandlers[1] = pF;
}

void GenericItem::SetLeftItem(Item* itm, tItemHandler pF)
{
	pElements[2] = itm;
	pFHandlers[2] = pF;
}

void GenericItem::SetRightItem(Item* itm, tItemHandler pF)
{
	pElements[3] = itm;
	pFHandlers[3] = pF;
}

void GenericItem::SetBtn1Item(Item* itm, tItemHandler pF)
{
	pElements[4] = itm;
	pFHandlers[4] = pF;
}

void GenericItem::SetBtn2Item(Item* itm, tItemHandler pF)
{
	pElements[5] = itm;
	pFHandlers[5] = pF;
}

void GenericItem::SetBtn3Item(Item* itm, tItemHandler pF)
{
	pElements[6] = itm;
	pFHandlers[6] = pF;
}

void GenericItem::SetBtn4Item(Item* itm, tItemHandler pF)
{
	pElements[7] = itm;
	pFHandlers[7] = pF;
}

void GenericItem::GoToUpItem(void)
{
	cursor = 0;
	Item::select();
}

void GenericItem::GoToDownItem(void)
{
	cursor = 1;
	Item::select();
}

void GenericItem::GoToLeftItem(void)
{
	cursor = 2;
	Item::select();
}

void GenericItem::GoToRightItem(void)
{
	cursor = 3;
	Item::select();
}

void GenericItem::GoToBtn1Item(void)
{
	cursor = 4;
	Item::select();
}

void GenericItem::GoToBtn2Item(void)
{
	cursor = 5;
	Item::select();
}

void GenericItem::GoToBtn3Item(void)
{
	cursor = 6;
	Item::select();
}

void GenericItem::GoToBtn4Item(void)
{
	cursor = 7;
	Item::select();
}
