/*
 * Guiml_auxiliary.cpp
 *
 *  Created on: 26 dic 2021
 *      Author: Utente
 */

#include <auxiliary.h>


/*
 * aux_draw_sliding_string
 * function that given a U8G2 object pointer draws a string in the box from xmin to xmax and the height of one character.
 * with its characters sliding from left to right (*ascending set) or right to left (*ascending reset).
 * Every time the function is called:
 * - The number of characters that will be displayed in the screen given the box area is computed
 * - The characters are displayed going from the *start_index value to the last character than can be displayed
 * - If the *update_start_index is set the *start_index is increased or decreased with respect to the value of *ascending
 * - If the first displayed character  is the first of the in string *ascending is set
 * - If the last displayed character is the last of the in string *ascending is reset
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the sliding string will be displayed
 * - ymin - high y coordinate of the box where the sliding string will be displayed
 * - xmax - right x coordinate of the box where the sliding string will be displayed
 * - ascending - pointer to the flag that indicates if the slide must be from left to right or right to left
 * - start_index - pointer to the variable containing the index of the first character to be drawn
 * - update_start_index - pointer to the flag indicating if the start_index must be increased or decreased.
 * - in - input string to be drawn.
 */
void aux_draw_sliding_string(U8G2 *pU8G2lib, int16_t xmin, int16_t ymin, int16_t xmax, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in)
{
	U8G2 *pU8 = pU8G2lib;
	int16_t width = xmax - xmin;
	uint16_t graphstring_len = width/pU8->getMaxCharWidth();
	uint16_t instring_len = strlen(in);

//	Serial.print("start_index: ");Serial.println(*start_index);
//	Serial.print("ascending: ");Serial.println(*ascending);
//	Serial.print("instring_len: ");Serial.println(instring_len);
//	Serial.print("graphstring_len: ");Serial.println(graphstring_len);

	/*
	 * If the in string is longer than the string that can be represented it will
	 * be drawn as sliding otherwise it will be fixed.
	 */
	if(instring_len > graphstring_len)
	{
		char *graphstring = (char*) malloc(sizeof(char)*(graphstring_len+1));

////		Serial.print("update_start_index: "); Serial.println(*update_start_index);
//		if(*update_start_index)
//		{
////			Serial.println("Entered in update_start_index if");
//			if(*ascending)
//			{
//				if(*start_index + graphstring_len < instring_len)
//					(*start_index)++;
//			}
//			else if(!(*ascending) && (*start_index > 0))
//			{
//				(*start_index)--;
//			}
//			(*update_start_index) = 0;
//		}

		if(*update_start_index)
		{
			if(*start_index > 0 && !*ascending)
				(*start_index)--;
			else if(*start_index < instring_len - graphstring_len && *ascending)
				(*start_index)++;
			*update_start_index = 0;
		}

		uint16_t j=0;
		uint16_t i=*start_index;
		while(1)
		{
			if(j<graphstring_len)
			{
				graphstring[j]=in[i];
			}
			else
			{
				graphstring[j]='\0';
				break;
			}

			if(in[i+1]=='\0')
			{
				if(*ascending)
					*ascending = 0;
				break;
			}

			if(i==0 && !(*ascending))
			{
				*ascending = 1;
			}

			i++;
			j++;
		}



		pU8->drawStr(xmin, ymin+pU8->getAscent(), graphstring);
		free(graphstring);
	}
	else
	{
		pU8->drawStr(xmin, ymin+pU8->getAscent(), in);
	}
}

/*
 * aux_draw_sliding_string
 * function that given a U8G2 object pointer draws a string in the box from xmin to the end of the display and the height of one character at the y coordinate ymin.
 * with its characters sliding from left to right (*ascending set) or right to left (*ascending reset).
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the sliding string will be displayed
 * - ymin - high y coordinate of the box where the sliding string will be displayed
 * - ascending - pointer to the flag that indicates if the slide must be from left to right or right to left
 * - start_index - pointer to the variable containing the index of the first character to be drawn
 * - update_start_index - pointer to the flag indicating if the start_index must be increased or decreased.
 * - in - input string to be drawn.
 */
void aux_draw_sliding_string(U8G2 *pU8G2lib, int16_t xmin, int16_t ymin, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	aux_draw_sliding_string(pU8G2lib, xmin, ymin, xmax, ascending, start_index, update_start_index, in);
}


/*
 * aux_draw_sliding_string
 * function that given a U8G2 object pointer draws a string in the box starting from the left,height corner of the display and using all the available display width,
 * with its characters sliding from left to right (*ascending set) or right to left (*ascending reset).
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - ascending - pointer to the flag that indicates if the slide must be from left to right or right to left
 * - start_index - pointer to the variable containing the index of the first character to be drawn
 * - update_start_index - pointer to the flag indicating if the start_index must be increased or decreased.
 * - in - input string to be drawn.
 */
void aux_draw_sliding_string(U8G2 *pU8G2lib, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	aux_draw_sliding_string(pU8G2lib, 0, 0, xmax, ascending, start_index, update_start_index, in);
}


/*
 * aux_draw_split_string
 * function that given a U8G2 object pointer draws a string in a box with corners of coordinates (xmin,ymin)-(xmax,ymax).
 * The string is split on multiple lines without hyphenation. The words are neither truncated at the end of the line.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the string will be displayed
 * - ymin - high y coordinate of the box where the string will be displayed
 * - xmax - right x coordinate of the box where the string will be displayed
 * - ymax - low y coordinate of the box where the string will be displayed
 * - in - input string to be drawn.
 */
void aux_draw_split_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, const char *in)
{
	U8G2 *pU8 = pU8G2lib;
	uint16_t input_len = strlen(in)+1;
	char *buffer = (char*) malloc(sizeof(char)*input_len);
	uint8_t char_height = pU8->getMaxCharHeight();
	uint8_t char_ascent = pU8->getAscent();
	uint8_t char_width = pU8->getMaxCharWidth();
	int16_t width = xmax - xmin;
	int16_t xpos = xmin;
	int16_t ypos = ymin+char_ascent;
	char *graphstring = (char*) malloc(sizeof(char)*(width/char_width + 5));

	strcpy(buffer,in);

	// Eliminate the trailing white spaces
	uint16_t j=0;
	for(uint16_t i=0; i<input_len-1; i++)
	{
		if(buffer[i]==' ' || buffer[i]=='\n')
			j=i;
		else
			break;
	}

	if(j!=0)
	{
		for(uint16_t i=j; i<input_len-1;i++)
		{
			buffer[i-j] = buffer[i];
		}
		buffer[input_len-j] = '\0';
	}



	uint16_t buffer_len = strlen(buffer);

	j = 0;
	for(uint16_t i=0; i<=buffer_len;i++)
	{
		if(buffer[i]!=' ' && buffer[i]!='\n' && buffer[i] !='\0')
		{
			graphstring[j] = buffer[i];
			j++;
		}
		else if(buffer[i] == ' ' || buffer[i] == '\0' )// character is a blanck space or it has arrived to the end of the string
		{
			if(j!=0) //Repeated white spaces are ignored.
			{
				if(buffer[i] != '\0')
				{
					graphstring[j]=' ';
					graphstring[j+1]='\0';
				}
				else
				{
					graphstring[j]='\0';
				}
//				Serial.print("xpos: ");Serial.print(xpos);Serial.print(" ypos: ");Serial.print(ypos);
//				Serial.print(" graphstring: ");Serial.println(graphstring);
				j = 0;
				if(xpos+pU8->getStrWidth(graphstring)<xmax)
				{
					pU8->drawStr(xpos, ypos, graphstring);
					xpos = xpos+pU8->getStrWidth(graphstring);
				}
				else // The word could not be drawn in the allocated space on screen given its initial point.
				{
					if(xmin == xpos) // The word could not be drawn in all the allocated screen
					{
						// The word is nevertheless drawn on screen.
						pU8->drawStr(xpos, ypos, graphstring);
						xpos = xmin;
						ypos = ypos+pU8->getMaxCharHeight();
						if(ypos >= ymax)
							break; // All the allocated space in the screen has been used.
					}
					else
					{
						// The word is draw in the following line
						xpos = xmin;
						ypos = ypos+ pU8->getMaxCharHeight();
						if(ypos < ymax) // If a new line is available in the allocated screen space
						{
							pU8->drawStr(xpos, ypos, graphstring);
							xpos = xpos+pU8->getStrWidth(graphstring);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		else if(buffer[i] == '\n') // Carriage Return
		{
			// Consecutive carriage returns are recognized.
			graphstring[j]='\0';
//			Serial.print("xpos: ");Serial.print(xpos);Serial.print(" ypos: ");Serial.print(ypos);
//			Serial.print(" graphstring: ");Serial.println(graphstring);
			j=0;
			if(xpos+pU8->getStrWidth(graphstring)<xmax)
			{
//				Serial.println("<xmaz");
				pU8->drawStr(xpos, ypos, graphstring);
				xpos = xpos+pU8->getStrWidth(graphstring);
				ypos = ypos+char_height;
			}
			else // The word could not be drawn in the allocated space on screen given its initial point.
			{
				if(xmin == xpos) // The word could not be drawn in all the allocated screen
				{
					// The word is nevertheless drawn on screen.
//					Serial.println("== xpos");
					pU8->drawStr(xpos, ypos, graphstring);
					xpos = xmin;
					ypos = ypos+char_height;
					if(ypos >= ymax)
						break; // All the allocated space in the screen has been used.
				}
				else
				{
//					Serial.println("else");
					// The word is draw in the following line
					xpos = xmin;
					ypos = ypos+ char_height;
					if(ypos < ymax) // If a new line is available in the allocated screen space
					{
						pU8->drawStr(xpos, ypos, graphstring);
						xpos = xmin;
						ypos = ypos+ char_height;
					}
					else
					{
						break;
					}
				}
			}
		}
	}

//	Serial.println("Freeing strings");
	free(graphstring);
	free(buffer);

}

/*
 * aux_draw_split_string
 * function that given a U8G2 object pointer draws a string in a box going from the left,high corner of coordinates (xmin,ymin) to the right low corner of the display
 * The string is split on multiple lines without hyphenation. The words are neither truncated at the end of the line.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the string will be displayed
 * - ymin - high y coordinate of the box where the string will be displayed
 * - in - input string to be drawn.
 */
void aux_draw_split_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, const char *in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	int16_t ymax = pU8G2lib->getDisplayHeight();
	aux_draw_split_string(pU8G2lib,xmin, ymin, xmax, ymax, in);
}

/*
 * aux_draw_split_string
 * function that given a U8G2 object pointer draws a string in a box going from the left high corner of the display to the right low corner of the display
 * The string is split on multiple lines without hyphenation. The words are neither truncated at the end of the line.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - in - input string to be drawn.
 */
void aux_draw_split_string(U8G2 *pU8G2lib,const char *in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	int16_t ymax = pU8G2lib->getDisplayHeight();
	aux_draw_split_string(pU8G2lib,0, 0, xmax, ymax, in);
}


/*
 * aux_draw_cut_string
 * function that given a U8G2 object pointer draws a string in a box going from the left high corner with width equal to xmax-xmin and the height of one character
 * Only the characters that fit in the box are drawn.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the string will be displayed
 * - ymin - high y coordinate of the box where the string will be displayed
 * - xmax - right x coordinate of the box where the string will be displayed
 * - in - input string to be drawn.
 */
void aux_draw_cut_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, int16_t xmax, const char* in)
{
	U8G2 *pU8 = pU8G2lib;
	int16_t width = xmax - xmin;
	uint16_t graphstring_len = width/pU8->getMaxCharWidth();


	uint16_t instring_len = strlen(in);

	if(graphstring_len > instring_len)
	{
		pU8->drawStr(xmin, ymin+pU8->getAscent(), in);
	}
	else
	{
		char *graphstring = (char*) malloc(sizeof(char)*(graphstring_len+5));
		for(uint16_t i=0; i<=graphstring_len ; i++)
		{
			graphstring[i] = in[i];
		}
		graphstring[graphstring_len+1] = '\0';
		pU8->drawStr(xmin, ymin+pU8->getAscent(), graphstring);
		free(graphstring);
	}
}

/*
 * aux_draw_cut_string
 * function that given a U8G2 object pointer draws a string in a box going from the left high corner to the right edge of the display and with a height of one character
 * Only the characters that fit in the box are drawn.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - xmin - left x coordinate of the box where the string will be displayed
 * - ymin - high y coordinate of the box where the string will be displayed
 * - xmax - right x coordinate of the box where the string will be displayed
 * - in - input string to be drawn.
 */
void aux_draw_cut_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, const char* in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	aux_draw_cut_string(pU8G2lib,xmin, ymin, xmax, in);
}

/*
 * aux_draw_cut_string
 * function that given a U8G2 object pointer draws a string in a box going from the left high corner of the display to the left edge of the display and with the height of one character
 * Only the characters that fit in the box are drawn.
 *
 * Arguments:
 * - pU8G2lib - pointer to the U8G2 library object
 * - in - input string to be drawn.
 */
void aux_draw_cut_string(U8G2 *pU8G2lib,const char* in)
{
	int16_t xmax = pU8G2lib->getDisplayWidth();
	aux_draw_cut_string(pU8G2lib,0, 0, xmax, in);
}

