/*
 * Guiml_ValueUnit.cpp
 *
 *  Created on: 13 Dec 2021
 *      Author: revel
 */


#include <value_unit_selection.h>

/*
 * value_selection
 * Constructor
 * Arguments:
 * - digits_num : number of ciphers used for the variable encoding
 * - pIVal : pointer to the integ er variable to be set by this GUI page.
 */
guiml::value_unit_selection::value_unit_selection(const char *name, U8G2 *pGraf, uint8_t* fnt, const char *dspt, uint8_t digits_num, double *pVal, item* bs, item* prnt):
item(name,pGraf,fnt,bs,prnt)
{
	number_digits = digits_num;
	digit_string = (char*) malloc(sizeof(char)*(digits_num+1));
	for(uint8_t i=0;i<=digits_num;i++)
		digit_string[i] = ' ';
	digit_string[digits_num] = '\0';


	description = (char*) malloc(sizeof(char)*(strlen(dspt)+1));
	strcpy(description,dspt);

	pValue = pVal;
}

/*
 * Destructor
 * Dynamically allocated arrays must be freed to avoid
 * memory leakage.
 */
guiml::value_unit_selection::~value_unit_selection()
{
	free(digit_string);
	free(description);
}

/*
 * get_content
 * method that return the pointer to a zt char array
 * that will represent as text the value and the unit
 */
char* guiml::value_unit_selection::get_content(void)
{
	char* str_add;

	// Check if the digits_string field is empty or not.
	uint8_t counter = 0;
	for(uint8_t i=0;i<number_digits;i++)
	{
		if(digit_string[i]!=' ')
		{
			counter++;
		}
	}

	// gets the content of the list GUI object containing
	// list of units.
	char* unit = pUnit_pointer->get_content();

	uint8_t unit_len = strlen(unit)+1;

	counter += unit_len;

	// dynamic allocation of the output string.
	str_add = (char*) malloc(sizeof(char) * counter);

	uint8_t j=0;
	// only the non blank characters of the
	// digit_string field are copied.
	for(uint8_t i=0;i<number_digits;i++)
		if(digit_string[i]!=' ')
		{
			str_add[j] = digit_string[i];
			j++;
		}

	str_add[j] = '\0';

	// Then the string containing the selected unit is
	// concatenated.
	strcat(str_add,unit);

	free(unit); //otherwise memory will leak;

	return str_add;
}

/*
 * increase
 * method that increase the cursor is charge_cursor flag member is set
 * otherwise increase the character via the digit_up method.
 */
void guiml::value_unit_selection::increase(void)
{
	if(change_cursor)
		if(cursor == number_digits+2)
			cursor = 0;
		else
			cursor++;
	else
		digit_up();
}

/*
 * decrease
 * opposite of increase.
 */
void guiml::value_unit_selection::decrease(void)
{
	if(change_cursor)
		if(cursor == 0)
			cursor = number_digits+2;
		else
			cursor--;
	else
		digit_down();
}

/*
 * digit_up
 * Method that increases the value of the cipher under the cursor when its change has been activated
 * by the user.
 */
void guiml::value_unit_selection::digit_up(void)
{
	Serial.println("In Guiml_Value::digit_up");
	if(cursor < number_digits)
	{
		if(digit_string[cursor] == ' ')
			digit_string[cursor] = '0';
		else if( '0' <= digit_string[cursor]  && digit_string[cursor] < '9' )
			digit_string[cursor]++;
		else if( digit_string[cursor] == '9')
			digit_string[cursor] = '.';
		else if( digit_string[cursor] == '.')
			digit_string[cursor] = '-';
		else if( digit_string[cursor] == '-')
			digit_string[cursor] = ' ';
	}
	else if(cursor == number_digits)
	{
		if(pUnit_pointer != NULL)
			pUnit_pointer->receive_forward();
	}
	Serial.println(digit_string);
}

/*
 * digit_down
 * Method that decreases the value of the cipher under the cursor when its change has been activated
 * by the user.
 */
void guiml::value_unit_selection::digit_down(void)
{
	if(cursor < number_digits)
	{
		if(digit_string[cursor] == '0')
			digit_string[cursor] = ' ';
		else if( '0' < digit_string[cursor] && digit_string[cursor] <= '9' )
			digit_string[cursor]--;
		else if( digit_string[cursor] == '.')
			digit_string[cursor] = '9';
		else if( digit_string[cursor] == '-')
			digit_string[cursor] = '.';
		else if( digit_string[cursor] == ' ')
			digit_string[cursor] = '-';
	}
	else if(cursor == number_digits)
	{
		if(pUnit_pointer != NULL)
			pUnit_pointer->receive_backward();
	}
}

/*
 * select
 * Method that is activated to indicate a selection
 * If the cursor is on a cipher and the change_cursor flag is set then it will change the operation so that now it is the cipher will be changed
 * If the cursor is on a cipher and the change_cursor flag is reset then it will change the operation so that now it is the cursor member will be changed
 * If the cursor is on the ok cursor position then data coded in the digit_string is decoded and saved in the global variable pointed by pIVal or pDVal members
 * If the cursor is on the back cursor position then the page is leaved and nothing is done to the pointed variable.
 */
void guiml::value_unit_selection::select(void)
{
	if(cursor < number_digits)
	{
		if(!change_cursor)
		{
			if(digit_string[cursor] != ' ')
			{
				cursor++;
				if(cursor < number_digits)
				{
					if(digit_string[cursor]==' ')
						digit_string[cursor]='0';
				}
				else
				{
					change_cursor = !change_cursor;
				}
			}
			else
				change_cursor = !change_cursor;
		}
		else
		{
			change_cursor = !change_cursor;
		}

	}
	else if(cursor == unit_cursor()) //unit position
	{
		if(pUnit_pointer!=NULL)
			pBase->set_selected(pUnit_pointer);
	}
	else if(cursor == ok_cursor()) //OK position
	{
		on_selection();
	}
	else if(cursor == back_cursor()) //back position
	{
		on_leave();
	}
}

/*
 * on_selection
 * When the selection is on the ok cursor position on_selection is called.
 * It decodes the information in the digit_string and puts it in the pointed variable.
 */
void guiml::value_unit_selection::on_selection()
{
	uint8_t empty=1;
	for(uint8_t i=0; i<number_digits; i++)
	{
		if(digit_string[i]!=' ')
		{
			empty=0;
			break;
		}
	}

	if(empty)
		return;

	*pValue = atof(digit_string);

	if(pFon_selection != NULL)
		pFon_selection(this);

	if(pItem_on_selection)
		pBase->set_selected(pItem_on_selection);
	else
		pBase->set_selected(pParent);
}


/*
 * on_leave
 * When the selection is on the back cursor position on_selection is called.
 * It does nothing but it calls the handler provided by the coder user when one is provided.
 */
void guiml::value_unit_selection::on_leave()
{
	if(pFon_leave != NULL)
		pFon_leave(this);

	if(pItem_on_leave)
		pBase->set_selected(pItem_on_leave);
	else
		pBase->set_selected(pParent);
}


/*
 * draw
 * Method that draws this GUI page on the display.
 */
void guiml::value_unit_selection::draw(void)
{
	int8_t  char_height = pU8G2lib->getMaxCharHeight();
	int8_t  char_width = pU8G2lib->getMaxCharWidth();
	int16_t display_width = pU8G2lib->getDisplayWidth();
	int16_t display_height = pU8G2lib->getDisplayHeight();
	int16_t char_ascent = pU8G2lib->getAscent();
	int16_t num_rows = display_height/char_height;


	//Serial.println("ENTRATO IN guiml::value_unit_selection::draw");

	char *unit=NULL;
	if(pUnit_pointer != NULL)
	{
		unit = pUnit_pointer->get_content();
	}
	else
	{
		unit = (char*) malloc(sizeof(char)*4);
		strcpy(unit,"   ");
	}

	int16_t digits_width = number_digits*char_width;
	int16_t unit_width = pU8G2lib->getStrWidth(unit);
	int16_t ok_width = pU8G2lib->getStrWidth(" ok ");
	int16_t back_width = pU8G2lib->getStrWidth("back");

	pU8G2lib->clearBuffer();
	pU8G2lib->firstPage();

	do
	{
		if(num_rows == 1)
			while(1);
		else if(num_rows == 2)
		{
			//Serial.println("ENTRATO IN guiml::value_unit_selection::draw num_rows==2");
			//Serial.print("cursor: ");Serial.println(cursor);
			//Serial.print("change_cursor: ");Serial.println(change_cursor);
			int16_t unit_x = digits_width+1;
			int16_t unit_y = char_height+char_ascent;
			int16_t ok_x = display_width - ok_width - back_width -2 ;
			int16_t back_x = display_width - back_width - 1 ;

			//draw_sliding_string(0, char_ascent, display_width, description);
			draw_sliding_string(description);

			for(uint8_t i=0;i<=number_digits+2;i++)
			{
				if(i < number_digits)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(i*char_width, unit_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(i*char_width, unit_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawGlyph(i*char_width, unit_y, digit_string[i]);
					}
				}
				else if(i == unit_cursor() )
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(unit_x, unit_y-char_ascent, unit_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(unit_x, unit_y, unit);
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(unit_x, unit_y, unit);
					}
				}
				else if(i == ok_cursor())
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(ok_x, unit_y-char_ascent, ok_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(ok_x, unit_y, " ok ");
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(ok_x, unit_y, " ok ");
					}
				}
				else if(i == number_digits + 2)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(back_x, unit_y-char_ascent, back_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(back_x, unit_y, "back");
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(back_x, unit_y, "back");
					}
				}
			}
		}
		else if(num_rows == 3)
		{
			int16_t digits_x = (display_width - (digits_width + unit_width + 2))/2;
			int16_t digits_width = pU8G2lib->getStrWidth(digit_string);
			int16_t unit_x   =  digits_x + digits_width + 1;
			int16_t unit_y   = char_height+char_ascent;
			int16_t ok_x     = (display_width - (ok_width + back_width))/3;
			int16_t ok_y     = 2*char_height + char_ascent;
			int16_t back_x   = 2*ok_x + ok_width;

			draw_sliding_string(0, 0, display_width, description);

			for(uint8_t i=0;i<=number_digits+2;i++)
			{
				if( i < number_digits)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(digits_x+i*char_width, unit_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(digits_x+i*char_width, unit_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawGlyph(digits_x+i*char_width, unit_y, digit_string[i]);
					}
				}
				else if(i == unit_cursor() )
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(unit_x, unit_y-char_ascent, unit_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(unit_x, unit_y, unit);
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawStr(unit_x, unit_y, unit);
					}
				}
				else if(i == ok_cursor())
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(ok_x, ok_y-char_ascent, ok_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(ok_x, ok_y, " ok ");
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawStr(ok_x, ok_y, " ok ");
					}
				}
				else if(i == number_digits + 2)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(back_x, ok_y-char_ascent, back_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(back_x, ok_y, "back");
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawStr(back_x, ok_y, "back");
					}
				}
			}
		}
		else if(num_rows>3)
		{
			int16_t last_row_y = (num_rows-2)*char_height;
			int16_t digits_x = (display_width - (digits_width + unit_width + 2))/2;
			int16_t digits_width = pU8G2lib->getStrWidth(digit_string);
			int16_t unit_x   =  digits_x + digits_width + 1;
			int16_t unit_y   =  last_row_y+char_ascent;
			int16_t ok_x     = (display_width - (ok_width + back_width))/3;
			int16_t ok_y     =  last_row_y + char_height + char_ascent;
			int16_t back_x   =  2*ok_x + ok_width;

			draw_split_string(0, 0, display_width, last_row_y, description);

			for(uint8_t i=0;i<=number_digits+2;i++)
			{
				if( i< number_digits)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(digits_x+i*char_width, unit_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(digits_x+i*char_width, unit_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawGlyph(digits_x+i*char_width, unit_y, digit_string[i]);
					}
				}
				else if(i == unit_cursor() )
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(unit_x, unit_y-char_ascent, unit_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(unit_x, unit_y, unit);
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(unit_x, unit_y, unit);
					}
				}
				else if(i == ok_cursor())
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(ok_x, ok_y-char_ascent, ok_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(ok_x, ok_y, " ok ");
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawStr(ok_x, ok_y, " ok ");
					}
				}
				else if(i == back_cursor())
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(back_x, ok_y-char_ascent, back_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(back_x, ok_y, "back");
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawStr(back_x, ok_y, "back");
					}
				}
			}
		}
	}while(pU8G2lib->nextPage());
	free(unit); //avoid memory leak;
}
