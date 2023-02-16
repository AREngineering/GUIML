/*
 * Guiml_Bool.cpp
 *
 *  Created on: 9 dic 2021
 *      Author: Utente
 */

#include <bool_selection.h>

/*
 * Constructor
 */
guiml::bool_selection::bool_selection(const char *name, U8G2 *pGraf, uint8_t *font, uint8_t *pVal, item* bs, item* prnt):
	item(name,pGraf,font,bs,prnt)
{
	pValue=pVal;
}

/*
 * Toggle Member
 */
void guiml::bool_selection::toggle(void)
{
	if(*pValue)
		*pValue=0;
	else
		*pValue=1;
}

/*
 * set variable true
 */
void guiml::bool_selection::set_true(void)
{
	*pValue = 1;
}

/*
 * Set variabile false
 */
void guiml::bool_selection::set_false(void)
{
	*pValue = 0;
}

/*
 * receive_forward activates toggle.
 */
void guiml::bool_selection::receive_forward(void)
{
	toggle();
	//base->Activate();
}

/*
 * Increase the cursor.
 */
void guiml::bool_selection::increase()
{
	cursor = (cursor+1)%2;
	//base->Activate();
}

/*
 * Decrease the cursor.
 */
void guiml::bool_selection::decrease()
{
	cursor = (cursor-1)%2;
	//base->Activate();
}

/*
 * Selection writes the information saved in the cursor
 * in the parameter variable.
 */
void guiml::bool_selection::select()
{
	if(!cursor)
		*pValue = 1;
	else
		*pValue = 0;

	on_selection();
//	pBase->set_selected(pParent);
	//base->Activate();
}

/*
 * void get_content()
 * return dynamically allocated string containing "yes" if the parameter is true or "no" if it is false.
 */
char* guiml::bool_selection::get_content(void)
{
	char *str_add;
	if(*pValue)
	{
		str_add = (char*) malloc(sizeof(char)*4);
		strcpy(str_add,"yes");
	}
	else
	{
		str_add = (char*) malloc(sizeof(char)*3);
		strcpy(str_add,"no");
	}
	return str_add;
}

/*
 * void draw()
 * method that draw the page on the display.
 */
void guiml::bool_selection::draw(void)
{
	int8_t char_height = pU8G2lib->getMaxCharHeight();
	int16_t display_width = pU8G2lib->getDisplayWidth();
	int16_t display_height = pU8G2lib->getDisplayHeight();
	int16_t char_ascent = pU8G2lib->getAscent();
	int16_t num_rows = display_height/char_height;
	//int16_t num_rows2 = num_rows/2;

	// Different approaches to the drawing of the page
	// with respect to the number of lines that can be
	// displayed on the screen.
	if(num_rows == 1)
		while(1);
	else if(num_rows == 2)
	{
		pU8G2lib->clearBuffer();
		pU8G2lib->firstPage();
		do
		{
			// Two lines can be drawn on the screen
			// Label on the first, yes and no selection on the second.

			// Label is displayed as description on the first line as
			// sliding string.
			draw_sliding_string(label);

			int16_t yes_width = pU8G2lib->getStrWidth("yes");
			int16_t no_width = pU8G2lib->getStrWidth("no");

			int16_t yes_x = display_width/4-yes_width/2;

			// If cursor is 0 it is over the "yes" position
			// the yes position is highlighted.
			if(!cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(yes_x,char_height, yes_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(yes_x, char_height+char_ascent, "yes");
			int16_t no_x = display_width/4*3-no_width/2;

			// If cursor is 1 it is over the "no" position and that
			// position is highlighted.
			if(cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(no_x,char_height, no_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(no_x, char_height+char_ascent, "no");
		}while(pU8G2lib->nextPage());
	}
	else if(num_rows == 3)
	{
		// Three lines can be drawn on the screen
		// Label is displayed on the first
		// Yes and no option displayed on the third
		pU8G2lib->clearBuffer();
		pU8G2lib->firstPage();
		do
		{
			draw_split_string(0, 0, display_width, 2*char_height, label);

			int16_t yes_width = pU8G2lib->getStrWidth("yes");
			int16_t no_width = pU8G2lib->getStrWidth("no");

			int16_t yes_y = 2*char_height;
			int16_t yes_x = display_width/4-yes_width/2;

			// Cursor on the "yes" position, "yes" position highlighted
			if(!cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(yes_x,yes_y, yes_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(yes_x, yes_y+char_ascent, "yes");
			int16_t no_x = display_width/4*3-no_width/2;

			// Cursor on the "no" position, "no" position highlighted
			if(cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(no_x,yes_y, no_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(no_x, yes_y+char_ascent, "no");
		}while(pU8G2lib->nextPage());
	}
	else
	{
		// Display that can show more than 3 lines.
		pU8G2lib->clearBuffer();
		pU8G2lib->firstPage();
		do
		{
			// Label displayed on the first line
			draw_split_string(0, 0, display_width, display_height/2, label);

			int16_t yes_width = pU8G2lib->getStrWidth("yes");
			int16_t no_width = pU8G2lib->getStrWidth("no");

			// "yes" and "no" option placed vertically at 3/4 of the display height.
			int16_t yes_y = 3*display_height/4 - char_height/2;
			int16_t yes_x = display_width/4-yes_width/2;

			// Cursor on the "yes" position, "yes" position highlighted
			if(!cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(yes_x,yes_y, yes_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(yes_x, yes_y+char_ascent, "yes");
			int16_t no_x = display_width/4*3-no_width/2;

			// Cursor on the "no" position, "no" position highlighted
			if(cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(no_x,yes_y, no_width, char_height);
				pU8G2lib->setDrawColor(2);
			}
			pU8G2lib->drawStr(no_x, yes_y+char_ascent, "no");
		}while(pU8G2lib->nextPage());
	}
}


/*
 * void on_selection()
 * reset the cursor to 0 when an object in the array is selected.
 * User can specify a pointer to an external function to operate changes on global variables on selection.
 */
void guiml::bool_selection::on_selection(void)
{
	if(pFon_selection != NULL)
	{
		pFon_selection(this);
	}

	cursor = 0;

	if(pItem_on_selection != NULL)
		pBase->set_selected(pItem_on_selection);
	else
		pBase->set_selected(pParent);
}

/*
 * void on_leave()
 * reset the cursor to 0 when the user returns to the parent object.
 * User can specify a pointer to an external function to operate changes on global variables on leave.
 */
void guiml::bool_selection::on_leave(void)
{
	if(pFon_leave != NULL)
	{
		pFon_leave(this);
	}
	cursor = 0;
	if(pItem_on_leave != NULL)
	{
		pBase->set_selected(pItem_on_leave);
	}
}

