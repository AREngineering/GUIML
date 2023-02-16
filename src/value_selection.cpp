/*
 * Guiml_ValueUnit.cpp
 *
 *  Created on: 13 Dec 2021
 *      Author: revel
 */


#include <value_selection.h>

/*
 * value_selection
 * Constructor
 * Arguments:
 * - digits_num : number of ciphers used for the variable encoding
 * - pIVal : pointer to the integ er variable to be set by this GUI page.
 */
guiml::value_selection::value_selection(const char *name, U8G2 *pGraf, uint8_t* fnt, const char *dspt, uint8_t digits_num, int32_t *pIVal, item* bs, item* prnt):
item(name,pGraf,fnt,bs,prnt)
{
	number_digits = digits_num;
	digit_string = (char*) malloc(sizeof(char)*(digits_num+1));
	for(uint8_t i=0;i<=digits_num;i++)
		digit_string[i] = ' ';
	digit_string[digits_num] = '\0';


	description = (char*) malloc(sizeof(char)*(strlen(dspt)+1));
	strcpy(description,dspt);

	int_flag=1;
	pIValue = pIVal;
}

/*
 * value_selection
 * Constructor
 * Arguments:
 * - digits_num : number of ciphers used for the variable encoding
 * - pDVal : pointer to the double variable to be set by this GUI page.
 */
guiml::value_selection::value_selection(const char *name, U8G2 *pGraf, uint8_t* fnt, const char *dspt, uint8_t digits_num, double *pDVal, item* bs, item* prnt):
item(name,pGraf,fnt,bs,prnt)
{
	number_digits = digits_num;
	digit_string = (char*) malloc(sizeof(char)*(digits_num+1));
	for(uint8_t i=0;i<=digits_num;i++)
		digit_string[i] = ' ';
	digit_string[digits_num] = '\0';


	description = (char*) malloc(sizeof(char)*(strlen(dspt)+1));
	strcpy(description,dspt);

	int_flag = 0;
	pDValue = pDVal;
}


/*
 * destructor
 * digit_string and description char arrays are dynamically allocated
 * and must be freed to avoid memory leak
 */
guiml::value_selection::~value_selection()
{
	free(digit_string);
	free(description);
}


/*
 * get_content
 * Method that produces a string containing the value coded in the digit_string
 * used eventually by the parent object to show the content of this object.
 * Output char array dynamically allocated, must be freed by calling code when no more useful.
 */
char* guiml::value_selection::get_content(void)
{
	if(pIValue != NULL)
		read_pIValue();
	else if(pDValue != NULL)
		read_pDValue();

	char* str_add;

	// The method looks for the number of non-blank ciphers
	uint8_t counter = 0;
	for(uint8_t i=0;i<number_digits;i++)
		if(digit_string[i]!=' ')
			counter++;

	// char array is dynamically allocated
	str_add = (char*) malloc(sizeof(char) * (counter+1));

	// string is copied only with the non-blank ciphers
	uint8_t j=0;
	for(uint8_t i=0;i<number_digits;i++)
		if(digit_string[i]!=' ')
		{
			str_add[j] = digit_string[i];
			j++;
		}

	str_add[j] = '\0';

	return str_add;
}

/*
 * increase
 * move the cursor one step up or increase the value of the cipher under cursor
 */
void guiml::value_selection::increase(void)
{
	// if change_cursor is 1 the cursor is changed
	// otherwise the cipher char is increased by one

	Serial.println("Entering Increase");
	if(change_cursor)
	{
		if(cursor == back_cursor())
			cursor = 0;
		else
			cursor++;
	}
	else
		digit_up();
}


/*
 * decrease
 * move the cursor one step down or decrease the value of the cipher under cursor
 */
void guiml::value_selection::decrease(void)
{
	if(change_cursor)
	{
		if(cursor == 0)
			cursor = back_cursor();
		else
			cursor--;
	}
	else
		digit_down();
}

/*
 * digit_up
 * Method that increases the value of the cipher under the cursor when its change has been activated
 * by the user.
 */
void guiml::value_selection::digit_up(void)
{
	//Serial.println("In Guiml_Value::digit_up");
	// Operates only when the cursor is over one of the ciphers
	Serial.println("Entering digit_up");
	if(cursor < number_digits)
	{
		// If the variable is double cyclically change the cipher increasing the number
		// from 0 to 9, then '.','-' and ' ';
		if(!int_flag)
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
		else
		{
			//if it is integer cycles the same but without the '.' char.
			Serial.print("cursor: ");Serial.println(cursor);
			Serial.print("digit_string[cursor]: ");Serial.println((unsigned int) digit_string[cursor]);
			Serial.println("Space integer: "); Serial.println((unsigned int) ' ');
			if(digit_string[cursor] == ' ')
				digit_string[cursor] = '0';
			else if( '0' <= digit_string[cursor]  && digit_string[cursor] < '9' )
				digit_string[cursor]++;
			else if( digit_string[cursor] == '9')
				digit_string[cursor] = '-';
			else if( digit_string[cursor] == '-')
				digit_string[cursor] = ' ';
		}
	}
	//Serial.println(digit_string);
}

/*
 * digit_down
 * Method that decreases the value of the cipher under the cursor when its change has been activated
 * by the user.
 */
void guiml::value_selection::digit_down(void)
{
	if(cursor < number_digits)
	{
		if(!int_flag)
		{
			// If the variable is double cyclically change the cipher increasing the number
			// from 0 to 9, then '.','-' and ' ';
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
		else
		{
			//if it is integer cycles the same but without the '.' char.
			if(digit_string[cursor] == '0')
				digit_string[cursor] = ' ';
			else if( '0' < digit_string[cursor] && digit_string[cursor] <= '9' )
				digit_string[cursor]--;
			else if( digit_string[cursor] == '-')
				digit_string[cursor] = '9';
			else if( digit_string[cursor] == ' ')
				digit_string[cursor] = '-';
		}
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
void guiml::value_selection::select(void)
{
	if(cursor < number_digits)
	{
//		if(!change_cursor)
//		{
//			if(digit_string[cursor] != ' ')
//			{
//				cursor++;
//				if(cursor < number_digits)
//				{
//					if(digit_string[cursor]==' ')
//						digit_string[cursor]='0';
//				}
//				else
//				{
//					change_cursor = !change_cursor;
//				}
//			}
//			else
//				change_cursor = !change_cursor;
//		}
//		else
//		{
//			change_cursor = !change_cursor;
//		}

		Serial.println("Inverting change_cursor");
		change_cursor = !change_cursor;


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

void guiml::value_selection::on_selection()
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

	if(!int_flag)
		*pDValue = atof(digit_string);
	else
		*pIValue = atoi(digit_string);

	if(pFon_selection != NULL)
		pFon_selection(this);

	if(pItem_on_selection != NULL)
		pBase->set_selected(pItem_on_selection);
	else
		pBase->set_selected(pParent);
}

/*
 * on_leave
 * When the selection is on the back cursor position on_selection is called.
 * It does nothing but it calls the handler provided by the coder user when one is provided.
 */

void guiml::value_selection::on_leave()
{
	if(pFon_leave != NULL)
		pFon_leave(this);

	if(pItem_on_leave != NULL)
		pBase->set_selected(pItem_on_leave);
	else
		pBase->set_selected(pParent);
}

/*
 * draw
 * Method that draws this GUI page on the display.
 */
void guiml::value_selection::draw(void)
{
	// Geometric data regarding the display and the character
	int8_t  char_height = pU8G2lib->getMaxCharHeight();
	int8_t  char_width = pU8G2lib->getMaxCharWidth();
	int16_t display_width = pU8G2lib->getDisplayWidth();
	int16_t display_height = pU8G2lib->getDisplayHeight();
	int16_t char_ascent = pU8G2lib->getAscent();
	int16_t num_rows = display_height/char_height;


	//Serial.println("ENTRATO IN guiml::value_selection::draw");
	int16_t digits_width = number_digits*char_width;
	int16_t ok_width = pU8G2lib->getStrWidth(" ok ");
	int16_t back_width = pU8G2lib->getStrWidth("back");

	pU8G2lib->clearBuffer();
	pU8G2lib->firstPage();
	do
	{
		// Things are shown consistently with the number of rows of character
		// that can be shown on the display.
		if(num_rows == 1)
			while(1);
		else if(num_rows == 2)
		{
			//Serial.println("ENTRATO IN guiml::value_selection::draw num_rows==2");
			//Serial.print("cursor: ");Serial.println(cursor);
			//Serial.print("change_cursor: ");Serial.println(change_cursor);


			// digits_y y coordinate where the digits ciphers will be placed
			int16_t digits_y = char_height+char_ascent;
			// ok_x and back_x - x coordinates  where ok and back will be placed. y same as digits.
			int16_t ok_x     = display_width - ok_width - back_width - 2 ;
			int16_t back_x   = display_width - back_width - 1 ;

			//draw_sliding_string(0, char_ascent, display_width, description);
			// The description is drawn as a sliding string on the beginning of the first row
			draw_sliding_string(description);



			for(uint8_t i=0;i<=back_cursor();i++)
			{
				// Drawing of the ciphers
				if(i < number_digits)
				{
					// If the cursor is on the i-th position
					// the i-th cipher will be highlighted
					// Otherwise the cipher is simply drawn.
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(i*char_width, digits_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(i*char_width, digits_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawGlyph(i*char_width, digits_y, digit_string[i]);
					}
				}
				else if(i == ok_cursor())
				{
					// If the cursor is on the ok position
					// the ok string is highlighted
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(ok_x, digits_y-char_ascent, ok_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(ok_x, digits_y, " ok ");
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(ok_x, digits_y, " ok ");
					}
				}
				else if(i == back_cursor())
				{
					// If the cursor is on the back position
					// the back string is highlighted
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(back_x, digits_y-char_ascent, back_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawStr(back_x, digits_y, "back");
						pU8G2lib->setDrawColor(1);
					}
					else
					{
						pU8G2lib->drawStr(back_x, digits_y, "back");
					}
				}
			}
		}
		else if(num_rows == 3)
		{
			// Three rows of character space are available on the display

			// x coordinate of the ciphers string centered with respect to the display width
			// y coordinate placed on the 2nd row
			int16_t digits_x = (display_width - digits_width)/2;
			int16_t digit_y   = char_height+char_ascent;

			// x coordinate of the ok and back buttons they are placed with equal space
			// in the width of the line.
			// y coordinate 3rd character row of the display.
			int16_t ok_x     = (display_width - (ok_width + back_width))/3;
			int16_t ok_y     = 2*char_height + char_ascent;
			int16_t back_x   = 2*ok_x + ok_width;

			draw_sliding_string(0, 0, display_width, description);

			// Same comments as in the case num_rows==2
			for(uint8_t i=0;i<=number_digits+2;i++)
			{
				if( i < number_digits)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(digits_x+i*char_width, digit_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(digits_x+i*char_width, digit_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawGlyph(digits_x+i*char_width, digit_y, digit_string[i]);
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
		else if(num_rows>3)
		{
			// Display has more than 3 rows
			int16_t last_row_y = (num_rows-2)*char_height;
			int16_t digits_x = (display_width - (digits_width + 2))/2;
			int16_t digits_y   =  last_row_y+char_ascent;
			int16_t ok_x     = (display_width - (ok_width + back_width))/3;
			int16_t ok_y     =  last_row_y + char_height + char_ascent;
			int16_t back_x   =  2*ok_x + ok_width;


			// All the space between the first row and the third last is devoted to
			// the drawing of the description.
			// The second last line is used for the ciphers
			// The last one is used for the ok and back buttons.
			draw_split_string(0, 0, display_width, last_row_y, description);


			// Same comments as for the other cases.
			for(uint8_t i=0;i<=number_digits+2;i++)
			{
				if( i< number_digits)
				{
					if(i == cursor)
					{
						pU8G2lib->setFontMode(1);
						pU8G2lib->setDrawColor(1);
						pU8G2lib->drawBox(digits_x+i*char_width, digits_y-char_ascent, char_width, char_height);
						pU8G2lib->setDrawColor(2);
						pU8G2lib->drawGlyph(digits_x+i*char_width, digits_y, digit_string[i]);
						pU8G2lib->setDrawColor(1);

					}
					else
					{
						pU8G2lib->drawGlyph(digits_x+i*char_width, digits_y, digit_string[i]);
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
}

/*
 * receive_forward
 * method invoked when the parent object receive a forward signal and its cursor points to
 * this object.
 * If the object changes a integer value the method increases the digit_string so that it codifies
 * a number higher by one unit.
 * If the highest values possible is selected the value cycles to its minimum.
 */
void guiml::value_selection::receive_forward(void)
{
	if(int_flag)
	{
		uint8_t found = 0;
		// It checks if the digit_string field is not empty
		for(uint8_t i= 0; i<number_digits; i++)
		{
			if('0' <= digit_string[i] && digit_string[i] <= '9')
			{
				found = 1;
				break;
			}
		}

		// If something is found in the digit_string field it
		// decodifies its value using atoi.
		if(found)
		{
			int32_t temp = atoi(digit_string);

			int32_t p_max_value = 1;

			// Computes the maximum number codifiable in the
			// digit_string field
			for(uint8_t i=0;i<number_digits;i++)
				p_max_value = p_max_value*10;

			p_max_value--;

			// And the minimum one.
			int32_t n_max_value = 1;
			for(uint8_t i=0;i<number_digits-1;i++)
				n_max_value = n_max_value*10;

			n_max_value = n_max_value+1;

			// if the maximum value is the one codified set the field to the minumum
			// otherwise it simply increases it by one.
			if(temp == p_max_value)
				temp = n_max_value;
			else
				temp++;

			// the value is stored in the digit_string as string.
			sprintf(digit_string,"%ld",temp);

			// the cursor is set to point to the ok
			cursor = ok_cursor();

			// select method is invoked to influence the variable pointed by pIVal
			select();
		}
		else
		{
			// If nothing is found the value is set to 1 both for the string encoding the value and
			// for the variable pointed by pIVal
			int32_t temp = 1;
			sprintf(digit_string,"%ld",temp);
			cursor = ok_cursor();
			select();
		}
	}
}


/*
 * receive_backward
 * Same as the receive_forward method only it goes in the
 * opposite direction.
 */
void guiml::value_selection::receive_backward(void)
{
	if(int_flag)
	{
		uint8_t found = 0;
		for(uint8_t i= 0; i<number_digits; i++)
		{
			if('0' <= digit_string[i] && digit_string[i] <= '9')
			{
				found = 1;
				break;
			}
		}

		if(found)
		{
			int32_t temp = atoi(digit_string);

			int32_t p_max_value = 1;

			for(uint8_t i=0;i<number_digits;i++)
				p_max_value = p_max_value*10;

			p_max_value--;

			int32_t n_max_value = 1;
			for(uint8_t i=0;i<number_digits-1;i++)
				n_max_value = n_max_value*10;

			n_max_value = -(n_max_value-1);

			if(temp == n_max_value)
				temp = p_max_value;
			else
				temp--;

			sprintf(digit_string,"%ld",temp);
			cursor = ok_cursor();
			select();
		}
		else
		{
			int32_t temp = -1;
			sprintf(digit_string,"%ld",temp);
			cursor = ok_cursor();
			select();
		}
	}
}

/*
 * read_pIValue()
 * Import in the digit_string the value contained in *pIValue
 */
void guiml::value_selection::read_pIValue()
{
	if(pIValue != NULL)
		sprintf(digit_string,"%ld",*pIValue);


	uint8_t i = 0;

	while(i<number_digits)
	{
		if(digit_string[i] == '\0')
		{
			digit_string[i] = ' ';
			while(i<number_digits)
			{
				digit_string[i] = ' ';
				i++;
			}
		}
		i++;
	}

	digit_string[number_digits] = '\0';
}

/*
 * read_pDValue()
 * Import in the digit_string the value contained in *pDValue
 */
void guiml::value_selection::read_pDValue()
{
	if(pDValue != NULL)
		dtostrf(*pDValue, number_digits, 2, digit_string);
}

