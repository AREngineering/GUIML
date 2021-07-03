/*
 * MenuListValue.cpp
 *
 *  Created on: 27 mar 2021
 *      Author: Utente
 */

#include "menulib.h"

/*
 * get_line_text
 * arg[0]: i - index of the element for which to produce the line to be printed
 * arg[1]: output - output string (must be already allocated in stack memory or heap)
 * arg[2]: selected - the cursor is on index i
 */
void MainMenuValue::get_line_text(uint8_t i, char* output, uint8_t selected)
{
	//Serial.println("In MainMenuValue::get_line_text");
	uint16_t display_width = pU8g2->getDisplayWidth();
	uint16_t char_width = pU8g2->getMaxCharWidth();
	uint16_t char_number = display_width/char_width;

	output[char_number] = '\0';
	for(uint8_t j=0; j<char_number; j++)
	{
		output[j] = ' ';
	}
	char content[MAX_CHARS];

	// Intermediate string where the output
	// line is created
	char outtext[MAX_CHARS];

	// If there is a pTextElement at the i index
	// then that is copied to the output.
	if(pTextElements[i]!=NULL)
	{
		strcpy(outtext,pTextElements[i]);
	}
	else if(pElements[i]!=NULL)
	{
		// Otherwise the text member of the
		// pElements[i] is copied
		pElements[i]->Text(outtext);
	}
	else
	{
		// Otherwise an empty string is copied
		strcpy(outtext, (char*) "");
	}

	if(pElements[i]!=NULL)
	{
		// If pElements[i] exists read the content string
		// and copy it in the content string.
		pElements[i]->Content(content);
	}
	else
		strcpy(content, (char*) "");


	/*
	 * Section where the actual output line is produced
	 * from outtext and content strings.
	 */
	uint8_t content_length = strlen(content);
	uint8_t init_j = (char_number-content_length);
	for(uint8_t j=0;j<content_length;j++)
	{
		output[init_j+j]=content[j];
	}
	uint8_t final_j = init_j-1;
	uint8_t outtext_avail_chars = init_j-1;
	uint8_t outtext_len = strlen(outtext);

	if(selected)
	{
		// If selected is true
		// Text must be sliding
		if(outtext_len>outtext_avail_chars)
		{
			// The text must slide in the first outtext_avail_chars characters of the string
			if(!draw_ascending && ((index_position + outtext_avail_chars) ==  outtext_len) )
			{
				draw_ascending = 1;
			}
			else if(draw_ascending && index_position == 0)
			{
				draw_ascending = 0;
			}

			for(uint8_t j=0; j<=outtext_avail_chars; j++)
			{
				output[j]=outtext[index_position+j];
			}
		}
		else
		{
			// Text is static
			final_j = min(strlen(outtext),final_j);

			for(uint8_t j=0; j<final_j;j++)
			{
				output[j] = outtext[j];
			}
		}
	}
	else
	{
		// Text is static
		final_j = min(strlen(outtext),final_j);

		for(uint8_t j=0; j<final_j;j++)
		{
			output[j] = outtext[j];
		}
	}
}
