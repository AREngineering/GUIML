/*
 * MenuItem.cpp
 *
 *  Created on: 23 mar 2021
 *      Author:
 */

#include "menulib.h"

/*
 * Constructor
 */
MenuItem::MenuItem(char* label,Item* pItm, tItemHandler hdlr, uint8_t base_flag):Item(label,base_flag)
{
	SetBackHandler(hdlr);
	SetBackItem(pItm);
}


/*
 * @MenuItem::AddElement method
 * Uses the Item method but since the last item must always be the "Back" option
 * it puts it always at the end.
 */
void MenuItem::AddElement(char* in_str, Item *pEl, tItemHandler pHdlr)
{
	if(is_home)
	{
		Item::AddElement(in_str, pEl, pHdlr);
	}
	else
	{
		if(elements_number == 0)
		{
			Item::AddElement(in_str, pEl, pHdlr);
		}
		else
		{
			RemoveElement((char*) "Back");
			Item::AddElement(in_str, pEl, pHdlr);
		}
		Item::AddElement((char*) "Back", pParent, pFBack_handler);
	}

}


/*
 * @get_line_text
 * copies in the output string the string in pList indexed by i.
 */
void MenuItem::get_line_text(uint8_t i,char* output, uint8_t selected)
{
	(void) selected;

	if(pTextElements[i]!=NULL)
	{
		strcpy(output,pTextElements[i]);
	}
	else if(pElements[i]!=NULL)
	{
		pElements[i]->Text(output);
	}
	else
	{
		strcpy(output, (char*) "");
	}
}

/*
 * @draw
 * Handle the display to represent the menu.
 */
void MenuItem::draw(void)
{
	int16_t display_height = pU8g2->getDisplayHeight();
	int16_t ascent = pU8g2->getMaxCharHeight();
	int16_t line_number = display_height/ascent;
	int16_t line_number2 = line_number/2;
	int16_t tot_lines = elements_number;


	char tmpstr[MAX_CHARS];
	int16_t init_i = 0;
	int16_t final_i = elements_number-1;

	/*
	 * The menu may have more lines that what can be represented on the display
	 * The following if block handles the representation of the lines near the one
	 * that is now under the cursor.
	 */
	if(tot_lines > line_number)
	{
		if(line_number%2)
		{
			// The display lines are a odd number
			init_i = cursor - line_number2;
			final_i = cursor + line_number2;
		}
		else
		{
			// The display lines are a even number
			init_i = cursor - (line_number2) + 1;
			final_i = cursor + (line_number2);
		}

		init_i = max(0,init_i); //init_i cannot be less than 0
		final_i = init_i + line_number -1;
		final_i = min(elements_number-1,final_i); //final_i cannot be more than back_cursor()
	}


	pU8g2->firstPage();
	while(1)
	{
		uint8_t count = 1;
		for(int16_t i = init_i ; i<= final_i; i++)
		{
			if(i == cursor)
			{
				// Drawing of the line under the cursor (transparent on white)
				get_line_text(i,tmpstr);
				pU8g2->setDrawColor(0);
				// In case the line under the cursor is longer than what can be represented by the display
				// draw_sliding_string makes it slide.
				draw_sliding_string(tmpstr,count*ascent);
				pU8g2->setDrawColor(1);
			}
			else
			{
				// Drawing of a line not under the cursor (white on transparent)
				get_line_text(i,tmpstr);
				pU8g2->drawStr(0, ascent*count, tmpstr);
			}
			count++;
		}

		if(!(pHome->PU8g2()->nextPage()))
		{
			// In case the line under the cursor is longer than what can be represented by the display
			// update_index_position makes it shift.
			update_index_position();
			break;
		}
	}
}

