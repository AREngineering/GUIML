/*
 * SelectionItem.cpp
 *
 *  Created on: 20 mar 2021
 *      Author: Utente
 */

#include "menulib.h"

SelectionItem::SelectionItem(char *label, char *expl, uint8_t vlen, uint8_t intf, uint8_t charf, uint8_t base_flag):Item(label,base_flag)
{
	char_activation = charf;
	pElements[0] = this;
	pTextElements[0] = (char*) malloc(sizeof(char)*(strlen(label)+1));
	strcpy(pTextElements[0],label);
	value_string_len = vlen;
	value_string = (char*) malloc(sizeof(char)*(value_string_len+1));
	for(uint8_t i=0;i<value_string_len;i++)
		value_string[i] = ' ';
	value_string[value_string_len] = '\0';
	integer = intf;
	elements_number = 2;
	explanation_line = (char*) malloc(sizeof(char)*(strlen(expl)+1));
	strcpy(explanation_line,expl);
	pFHandlers[0] = (tItemHandler) &(SelectionItem::ActivateChar);
	pFHandlers[back_cursor()] = (tItemHandler) &(SelectionItem::Reset);
}

SelectionItem::~SelectionItem(void)
{
	free(value_string);
	free(explanation_line);
}


/*
 * @draw description
 * Method to draw the explanatory line
 * (e.g. "Input value for xyz variable")
 */
void SelectionItem::draw_description(void)
{
	//Serial.println("In SelectionItem::draw_description");
	int16_t display_width   = pU8g2->getDisplayWidth();
	int16_t display_height  = pU8g2->getDisplayHeight();
	int16_t ascent          = pU8g2->getMaxCharHeight();
	int16_t char_width      = pU8g2->getMaxCharWidth();
	uint8_t expl_line_len   = strlen(explanation_line);
	int16_t expl_line_width = ((int16_t) expl_line_len)*char_width;
	int16_t lines_num       = display_height/ascent;

	//Serial.println(explanation_line);
	if(lines_num == 2 || lines_num == 3 )
	{
		/*
		 * The lines that can be represented on the screen
		 * are 2 or 3
		 */
		if(expl_line_width > display_width)
		{
			/*
			 *  The explanatory line is longer than what can
			 *  be represented on the screen. It is displayed
			 *  as a sliding string.
			 */

			draw_sliding_string(explanation_line,ascent);
		}
		else
		{
			pU8g2->drawStr(0, ascent, explanation_line);
		}
	}
	else if(lines_num > 3)
	{
		/*
		 * The lines that can be represented on the screen
		 * are more then 3. The line is broken and represented
		 * on the screen on multiple lines.
		 */
		draw_multline_string(explanation_line);
	}
}

/*
 * @draw
 * Method that produce on the screen
 * the interactive content of the object.
 */
void SelectionItem::draw(void)
{
	int16_t display_height = pU8g2->getDisplayHeight();
	int16_t ascent = pU8g2->getMaxCharHeight();
	int16_t lines_num = display_height/ascent;

	/*
	 *  If only one line can be represented on the screen the
	 *  object cannot work properly and therefore the mcu is kept stuck
	 *  on an indefinite while loop.
	 *  Other wise the methods handling the different part of the interactive
	 *  content (explanatory line, value line and OK and BACK selections) are called
	 */
	//
	if(lines_num == 1)
		while(1)
		{
			Serial.println(text);
			Serial.println("Error: Screen too small for this menu representation");
		}
	else
	{
		pU8g2->firstPage();

		while(1)
		{
			draw_description();
			draw_value_line();
			draw_ok_back();
			if(!pU8g2->nextPage())
			{
				update_index_position();
				break;
			}
		}
	}
}

/*
 * @increase
 * Method called in case of an "increase" signal.
 * It calls specific methods that will be defined
 * in children classes.
 */
void SelectionItem::increase(void)
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
			cursor = 0;
			cursor_digit = 0;
		}
	}
	else
	{
		increase_digit_char();
	}
	Serial.println("In SelectionItem::increase");
	Serial.print("cursor: ");Serial.println(cursor);
	Serial.print("cursor_digit: ");Serial.println(cursor_digit);
}

/*
 * @decrease
 * Method called in case of an "decrease" signal.
 * It calls specific methods that will be defined
 * in children classes.
 */
void SelectionItem::decrease(void)
{

	if(!char_activated)
	{
		if(cursor == 0)
		{
			if(cursor_digit == 0)
				cursor = 2;
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
	}
	else
	{
		decrease_digit_char();
	}
}

void SelectionItem::increase_digit_char()
{
	char *cpos = &(value_string[cursor_digit]);
	if(integer)
	{
		if(*cpos == ' ' || *cpos == '9')
		{
			*cpos = '0';
		}
		else
		{
			(*cpos)++;
		}
	}
	else
	{
		if(*cpos == ' ' || *cpos == '.')
		{
			*cpos = '0';
		}
		else if(*cpos == '9')
		{
			*cpos = '.';
		}
		else
		{
			(*cpos)++;
		}
	}
}

void SelectionItem::decrease_digit_char()
{
	char *cpos = &(value_string[cursor_digit]);
	if(integer)
	{
		if(*cpos == ' ' || *cpos == '0')
		{
			*cpos = '9';
		}
		else
		{
			(*cpos)--;
		}
	}
	else
	{
		if(*cpos == ' ' || *cpos == '0')
		{
			*cpos = '.';
		}
		else if(*cpos == '.')
		{
			*cpos = '9';
		}
		else
		{
			(*cpos)--;
		}
	}
}

void SelectionItem::activate_char(void)
{
	if(char_activated==0)
		char_activated = 1;
	else
		char_activated = 0;
}

void SelectionItem::SetOK(Item* pOk_itm, tItemHandler pfOk)
{
	pElements[ok_cursor()] = pOk_itm;
	pFHandlers[ok_cursor()] = pfOk;
}


void SelectionItem::SetBack(Item* pBack_itm, tItemHandler pfBack)
{
	pElements[back_cursor()] = pBack_itm;
	pFHandlers[back_cursor()] = pfBack;
}

double SelectionItem::dValue(void)
{
	return strtod(value_string,NULL);
}

uint16_t SelectionItem::iValue(void)
{
	return atoi(value_string);
}

void SelectionItem::ActivateChar(SelectionItem *obj)
{
	if(obj->char_activation)
		obj->activate_char();
}

void SelectionItem::Reset(SelectionItem *obj)
{
	Serial.println("In SelectionItem::Reset");
	obj->cursor_digit = 0;
	for(uint8_t i = 0; i<obj->value_string_len; i++)
		obj->value_string[i] = ' ';
}

/*
 * @draw_value_line
 * Method for draw the interactive line where the valuestring
 * is changed
 */
void SelectionItem::draw_value_line(void)
{
	int16_t display_width = pU8g2->getDisplayWidth();
	int16_t display_height = pU8g2->getDisplayHeight();
	int16_t ascent = pU8g2->getMaxCharHeight();
	int16_t lines_num = display_height/ascent;
	uint16_t dx = pU8g2->getMaxCharWidth();
	int16_t dx1 = (display_width - pU8g2->getStrWidth(value_string))/2;

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
	}
	else if(lines_num >= 3)
	{
		/* Display can show only three lines or more */

		pU8g2->drawLine(dx1-1, (lines_num-1)*ascent+2, dx1+dx*value_string_len+1, (lines_num-1)*ascent+2);

		for(uint8_t i = 0; i < value_string_len; i++)
		{
			// Second Line drawing
			if(i==cursor_digit)
			{
				if(cursor == 0)
					pU8g2->setDrawColor(0);
			}
			pU8g2->drawGlyph(dx1 + i*dx, (lines_num-1)*ascent, (char) value_string[i]);
			pU8g2->setDrawColor(1);
		}
	}
}

void SelectionItem::draw_ok_back(void)
{
	int16_t display_width = pU8g2->getDisplayWidth();
	int16_t display_height = pU8g2->getDisplayHeight();
	int16_t ascent = pU8g2->getMaxCharHeight();
	int16_t lines_num = display_height/ascent;


	if(lines_num == 2)
	{
		/* Display can show only two lines
		 *
		 * Ok and Back strings are displayed on the second line on the full
		 * right
		 */
		uint8_t OKstrwidth = pU8g2->getStrWidth("OK ");
		uint8_t BACKstrwidth = pU8g2->getStrWidth("BACK");
		if(cursor == ok_cursor())
		{
			pU8g2->setDrawColor(0);
		}
		pU8g2->drawStr(display_width - OKstrwidth - BACKstrwidth, 2*ascent, "OK");
		pU8g2->setDrawColor(1);
		if(cursor == back_cursor())
		{
			pU8g2->setDrawColor(0);
		}
		pU8g2->drawStr(display_width - BACKstrwidth, 2*ascent, "BACK");
		pU8g2->setDrawColor(1);
	}
	else if(lines_num >= 3)
	{
		/* Display can show three lines or more*/
		/*
		 * Ok and Back strings are displayed equally
		 * distantiated on the last available line
		 * on the screen.
		 */
		uint8_t OKstrwidth = pU8g2->getStrWidth(" OK ");
		uint8_t BACKstrwidth = pU8g2->getStrWidth("BACK");
		int16_t dx1 = (display_width - OKstrwidth - BACKstrwidth)/3;
		if(cursor == ok_cursor())
		{
			pU8g2->setDrawColor(0);
		}
		pU8g2->drawStr(dx1, lines_num*ascent, "OK");
		pU8g2->setDrawColor(1);
		if(cursor == back_cursor())
		{
			pU8g2->setDrawColor(0);
		}

		pU8g2->drawStr(2*dx1+OKstrwidth, lines_num*ascent, "BACK");
		pU8g2->setDrawColor(1);
	}
}

/*
 * Content
 * arg[0]: output string where the value selected by the user
 */
void SelectionItem::Content(char *output)
{
	//Serial.print("content: "); Serial.println(value_string);
	strcpy(output,value_string);
}

Interface* SelectionItem::CreateTypicalEncoderInterface(SelectionItem *pItm)
{
	Interface* pIntf = new Interface(); // Goes into dynamic memory otherwise in the stack it would be lost when the function ends
	Event* UpEvent = new Event(CWS);
	UpEvent->SetHandler((void (*)(Item*)) &SelectionItem::Increase);
	pIntf->AddEvent(UpEvent);
	Event* DownEvent = new Event(CCWS);
	DownEvent->SetHandler((void (*)(Item*)) &SelectionItem::Decrease);
	pIntf->AddEvent(DownEvent);
	Event* SelectEvent= new Event(BTN1);
	SelectEvent->SetHandler((void (*)(Item*)) &SelectionItem::Select);
	pIntf->AddEvent(SelectEvent);
	pIntf->SetTarget(pItm);
	return pIntf;
}

