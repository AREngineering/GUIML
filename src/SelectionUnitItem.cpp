/*
 * SelectionUnitItem.cpp
 *
 *  Created on: 7 giu 2021
 *      Author: Utente
 */

#include "menulib.h"

SelectionUnitItem::SelectionUnitItem(char *label, char *expl, char **unit_lst, uint8_t unit_lst_len, uint8_t vlen, uint8_t intf, uint8_t charf, uint8_t base_flag):
	SelectionItem(label,expl,vlen,intf,charf,base_flag)
{
	elements_number = 4;
	units_length = unit_lst_len;
	units_list   = unit_lst;
	units = new MenuList((char*) "units", unit_lst, unit_lst_len, &unit_idx, this, NULL, this, NULL);
	units->SetPInterface(Item::CreateTypicalEncoderInterface(units));
	pElements[unit_cursor()] = units;
	for(uint8_t i=1; i<back_cursor(); i++)
		pFHandlers[i] = NULL;
	pFHandlers[back_cursor()] = &(SelectionUnitItem::Reset);
}

/*
 * @draw_value_line
 * Method for draw the interactive line where the valuestring
 * is changed
 */
void SelectionUnitItem::draw_value_line(void)
{
	int16_t display_width = pU8g2->getDisplayWidth();
	int16_t display_height = pU8g2->getDisplayHeight();
	int16_t ascent = pU8g2->getMaxCharHeight();
	int16_t lines_num = display_height/ascent;
	uint16_t dx = pU8g2->getMaxCharWidth();

	uint8_t max_idx = 0;
	uint8_t max = 0;
	for(uint8_t i=0; i<units_length; i++)
	{
		if(strlen(units_list[i]) > max)
		{
			max = strlen(units_list[i]);
			max_idx = i;
		}
	}

	int16_t value_string_width = pU8g2->getStrWidth(value_string);
	int16_t unit_string_width = pU8g2->getStrWidth(units_list[max_idx]);

	int16_t dx1 = (display_width - value_string_width - unit_string_width)/2;

	if(lines_num == 2)
	{
		/* Display can show only two lines */
		pU8g2->drawLine(0, 2*ascent+2, dx*value_string_len+1, 2*ascent+2);
		/* Draw the content of value_string char by char */
		for(uint8_t i = 0; i < value_string_len; i++)
		{
			if(i==cursor_digit)
			{
				/* For the positional digit on which the cursor is situated
				 * the representation change from white on transparent to
				 * transparent to white
				 */

				if(cursor == 0)
					pU8g2->setDrawColor(0);
			}
			pU8g2->drawGlyph(i*dx,2*ascent,value_string[i]);
			pU8g2->setDrawColor(1);
		}

		if(cursor == unit_cursor())
		{
			pU8g2->setDrawColor(0);
		}
		if(unit_idx != 255)
			pU8g2->drawStr(value_string_len*dx,2*ascent,units_list[unit_idx]);
		else
			pU8g2->drawStr(value_string_len*dx,2*ascent,(const char*) "   ");
		pU8g2->setDrawColor(1);

	}
	else if(lines_num >= 3)
	{
		/* Display can show only three lines or more */

		pU8g2->drawLine(dx1-1, (lines_num-1)*ascent+2, dx1+dx*value_string_len+1, (lines_num-1)*ascent+2);

		for(uint8_t i = 0; i < value_string_len; i++)
		{
			if(i==cursor_digit)
			{
				if(cursor == 0)
					pU8g2->setDrawColor(0);
			}
			pU8g2->drawGlyph(dx1 + i*dx, (lines_num-1)*ascent, (char) value_string[i]);
			pU8g2->setDrawColor(1);
		}

		if(cursor == unit_cursor())
		{
			pU8g2->setDrawColor(0);
		}

		if(unit_idx != 255)
			pU8g2->drawStr(dx1 + dx*value_string_len, (lines_num-1)*ascent,units_list[unit_idx]);
		else
			pU8g2->drawStr(dx1 + value_string_len*dx,(lines_num-1)*ascent,(const char*) "   ");
		pU8g2->setDrawColor(1);
	}
}

/*
 * @increase
 * Method called in case of an "increase" signal.
 * It calls specific methods that will be defined
 * in children classes.
 */
void SelectionUnitItem::increase(void)
{

	if(!char_activated)
	{
		if(cursor == 0)
		{
			if(cursor_digit == (value_string_len-1))
				cursor = 1;
			else
				cursor_digit++;
		}
		else if(cursor == 1)
		{
			cursor = 2;
		}
		else if(cursor == 2)
		{
			cursor = 3;
		}
		else if(cursor == 3)
		{
			cursor = 0;
			cursor_digit = 0;
		}
	}
	else
	{
		increase_digit_char();
	}
	Serial.println("In SelectionUnitItem::increase");
	Serial.print("cursor: ");Serial.println(cursor);
	Serial.print("cursor_digit: ");Serial.println(cursor_digit);
}

/*
 * @decrease
 * Method called in case of an "decrease" signal.
 * It calls specific methods that will be defined
 * in children classes.
 */
void SelectionUnitItem::decrease(void)
{

	if(!char_activated)
	{
		if(cursor == 0)
		{
			if(cursor_digit == 0)
				cursor = 3;
			else
				cursor_digit--;
		}
		else if(cursor == 1)
		{
			cursor = 0;
			cursor_digit = value_string_len-1;
		}
		else if(cursor == 2)
		{
			cursor = 1;
		}
		else if(cursor == 3)
		{
			cursor = 2;
		}
	}
	else
	{
		decrease_digit_char();
	}
}

/*
 * Content
 * arg[0]: output string
 * Report in output the value selected by the user together
 * with the unit.
 */
void SelectionUnitItem::Content(char *output)
{
	char outstr[24];
	char unitstr[8];
	strcpy(outstr,value_string);
	for(uint8_t i=0; i<value_string_len;i++)
	{
		if(outstr[i]==' ')
		{
			for(uint8_t j=i; j<value_string_len-1;j++)
			{
				outstr[j]=outstr[j+1];
			}
		}
		else
			break;
	}

	for(uint8_t i=0; i<value_string_len;i++)
	{
		if(outstr[i]==' ')
		{
			outstr[i]='\0';
		}
	}

	Unit(unitstr);
	strcat(outstr,unitstr);

	strcpy(output,outstr);
}

/*
 * Unit
 * args[0]: output string pointer where the unit is stored
 */
void SelectionUnitItem::Unit(char *output)
{
	if(unit_idx != 255)
		strcpy(output,units_list[unit_idx]);
	else
		strcpy(output,"");
}

/*
 * Reset
 * static function that reset the choice of the unit
 * arg[0]
 */
void SelectionUnitItem::Reset(SelectionUnitItem* obj)
{
	SelectionItem::Reset(obj);
	obj->unit_idx = 255;
}

/*
 * Static function that creates the typical encoder interface
 */
Interface* SelectionUnitItem::CreateTypicalEncoderInterface(SelectionUnitItem *pItm)
{
	Interface* pIntf = new Interface(); // Goes into dynamic memory otherwise in the stack it would be lost when the function ends
	Event* UpEvent = new Event(CWS);
	UpEvent->SetHandler((void (*)(Item*)) &SelectionUnitItem::Increase);
	pIntf->AddEvent(UpEvent);
	Event* DownEvent = new Event(CCWS);
	DownEvent->SetHandler((void (*)(Item*)) &SelectionUnitItem::Decrease);
	pIntf->AddEvent(DownEvent);
	Event* SelectEvent= new Event(BTN1);
	SelectEvent->SetHandler((void (*)(Item*)) &SelectionUnitItem::Select);
	pIntf->AddEvent(SelectEvent);
	pIntf->SetTarget(pItm);
	return pIntf;
}



