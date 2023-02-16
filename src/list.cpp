/*
 * Guiml_Menu.cpp
 *
 *  Created on: 17 ott 2021
 *      Author: Utente
 */


#include <parameters.h>
#include <list.h>


// Constructor
guiml::list::list(const char *name, U8G2 *pGraf, uint8_t* fnt, item *bs, item *prnt):
	item(name, pGraf,fnt,bs,prnt)
{
}

/* Constructor that has as
*  arguments:
*  pSel - pointer to the char array where the copy of the selection is stored.
*/

guiml::list::list(const char *name, U8G2 *pGraf , uint8_t* fnt, char** pSel, item* bs, item* prnt):
		item(name, pGraf,fnt, bs,prnt)
{
	pSelected = pSel;
}

/* Constructor that has as
*  arguments:
*  pSel_idx - pointer to the uint8_t variable where the selected index of the list is stored.
*/

guiml::list::list(const char *name, U8G2 *pGraf , uint8_t* fnt, uint8_t* pSel_idx, item* bs, item* prnt):
		item(name, pGraf,fnt, bs,prnt)
{
	pSelected_index = pSel_idx;
}


/* Constructor that has as
*  arguments:
*  pSel_idx - pointer to the char array where the copy of the selection is stored.
*  pList - pointer to the list of zt char arrays from which the user can make his selection
*  length - length of the list
*/
guiml::list::list(const char *name, U8G2 *pGraf, uint8_t* fnt, uint8_t* pSel_idx, const char** pList, uint8_t length,  item* bs, item* prnt):
			item(name, pGraf,fnt,bs,prnt)
{
	pSelected_index = pSel_idx;
	external_pointer_provided = 1;
	items = (char **) pList;
	number_of_items = length;
}

/* Constructor that has as
*  arguments:
*  pSel - pointer to the char array where the copy of the selection is stored.
*  pList - pointer to the list of zt char arrays from which the user can make his selection
*  length - length of the list
*/
guiml::list::list(const char *name, U8G2 *pGraf, uint8_t* fnt, char** pSel, const char **pList, uint8_t length,  item* bs, item* prnt):
			item(name, pGraf,fnt,bs,prnt)
{
	pSelected = pSel;
	external_pointer_provided = 1;
	items = (char **) pList;
	number_of_items = length;
}

/*
 * Destructor
 * free heap memory space dynamically allocated.
 */
guiml::list::~list()
{
	if(items != NULL)
	{
		for(uint8_t i=0; i<number_of_items;i++)
		{
			free(items[i]);
		}
		free(items);
	}
}


/*
 * add_element
 * Add an element to the array member items
 */
void guiml::list::add_element(const char *itm)
{

	if(!external_pointer_provided)
	{
		// Allocate memory space for a new string pointer and then
		// copies the string in the last position.

		uint8_t item_len = strlen(itm)+1;
		if(number_of_items>0)
		{
			items = (char**) realloc(items, sizeof(char*)*(number_of_items+1));
			items[number_of_items] = (char*) malloc(sizeof(char)*item_len);
			strcpy(items[number_of_items],itm);
			number_of_items++;
		}
		else
		{
			items = (char**) malloc(sizeof(char*));
			items[number_of_items] = (char*) malloc(sizeof(char)*item_len);
			strcpy(items[number_of_items],itm);
			number_of_items++;
		}
	}
	else
		while(1)
		{
			Serial.println("ERROR: in guiml::list::add_element pointer to an external string array has been provided.\nIt must be considered as constant.\nIt is not possible to add or remove elements.");
		}
}


/*
 * remove_item
 * frees the memory allocated for the last string.
 * reallocates the array of char pointer reducing the space by one.
 */
void guiml::list::remove_item(void)
{

	if(!external_pointer_provided)
	{
		if(number_of_items>0)
		{
			free(items[number_of_items-1]);
			items[number_of_items-1]=NULL;
			number_of_items--;
			items=(char**) realloc(items,sizeof(char*)*(number_of_items));
		}
	}
	else
		while(1)
		{
			Serial.println("ERROR: in guiml::list::add_element pointer to an external string array has been provided.\nIt must be considered as constant.\nIt is not possible to add or remove elements.");
		}
}

/*
 * remove_item
 * Arguments:
 * - i : index of the element to be removed
 * frees the memory allocated for the i-th string.
 * shifts down all the elements of the char pointer array after i-th and reallocates the space for the shifted string.
 * reallocate the memory for the array of characters reducing it by one element.
 */
void guiml::list::remove_item(uint8_t i)
{
	if(!external_pointer_provided)
	{
		if(i == number_of_items-1)
		{
			remove_item();
		}
		else if(i >= number_of_items)
		{
			while(1);
		}
		else
		{
			free(items[i]); //Destroys the content of the i-th element
			items[i] = NULL;

			// Shift down all the element from index i+1 to number_of_item
			for(uint8_t j=i+1;j<number_of_items;j++)
			{
				uint8_t item_len = strlen(items[j])+1;
				if(items[j-1]!=NULL)
				{
					items[j-1] = (char *) realloc(items[j-1],sizeof(char)*item_len); // j-1 index of the array has memory allocated that must be reallocated
				}
				else
				{
					items[j-1] = (char *) malloc(sizeof(char)*item_len); //j-1 has not memory allocated it must be allocated.
				}
				strcpy(items[j-1],items[j]); // copy the string from j to j-1.
			}
			number_of_items--;
			items = (char**) realloc(items,sizeof(char*)*number_of_items); //reallocation of the memory for the array of char pointers
		}
	}
	else
		while(1)
		{
			Serial.println("ERROR: in guiml::list::add_element pointer to an external string array has been provided.\nIt must be considered as constant.\nIt is not possible to add or remove elements.");
		}
}

/*
 * select
 * Method called when a select signal has arrived to the object.
 */
void guiml::list::select(void)
{
	if(cursor < number_of_items)
	{
		// Cursor is on one item
		selected_idx = cursor;
		on_selection();

		//base->Activate();
	}
	else
	{
		// Cursor on the back option
		back();
	}
}

/*
 * increase
 * increases cyclically the cursor
 */
void guiml::list::increase(void)
{
	if(cursor == number_of_items)
	{
		cursor = 0;
	}
	else
	{
		cursor++;
	}
	//base->Activate();
}

/*
 * decrease
 * decreases cyclically the cursor
 */
void guiml::list::decrease(void)
{
	if(cursor == 0)
	{
		cursor = number_of_items;
	}
	else
	{
		cursor--;
	}
	//base->Activate();
}

/*
 * back
 * Method called on back option selection
 */
void guiml::list::back(void)
{

	on_leave();
	//base->Activate();
}

///*
// * set_pSelected
// * Method used to set the pointer to the external char array
// * where the selected string of the list must be copied
// */
//void guiml::list::set_pSelected(char **pSel)
//{
//	pSelected = pSel;
//}

/*
 * on_selection
 * copies the selected string to the external string referred by pSelected pointer
 */
void guiml::list::on_selection(void)
{
	if(pSelected_index != NULL)
	{
		*pSelected_index = cursor;
	}
	else if(pSelected != NULL)
	{
		if(*pSelected == NULL) // The pointed address obtained by resolving pSelect is NULL so no memory has been allocated for this string; Allocation required
		{
			uint8_t item_len = strlen(items[cursor])+1;
			*pSelected = (char*) malloc(sizeof(char)*item_len);
		}
		else // reallocation is necessary instead
		{
			uint8_t item_len = strlen(items[cursor])+1;
			*pSelected = (char*) realloc(*pSelected,sizeof(char)*item_len);
		}
		strcpy(*pSelected,items[cursor]);
	}

	if(pFon_selection!=NULL)
		pFon_selection(this);

	if(pItem_on_selection != NULL)
		pBase->set_selected(pItem_on_selection); //Goes back to the object provided by the code user
	else
		pBase->set_selected(pParent); //Goes back to the parent object
	//Serial.print("after pSelected: ");Serial.println(*pSelected);
}

/*
 * on_leave
 * on back selection cursor is reset
 */
void guiml::list::on_leave(void)
{
	if(pFon_leave != NULL)
		pFon_leave(this);

	cursor = 0;

	if(pItem_on_leave!=NULL)
		pBase->set_selected(pItem_on_leave);
	else
		pBase->set_selected(pParent);
}


/*
 * receive_forward
 * Method called by the parent object on this one when it receives a forward signal.
 * Increases cyclically cursor (avoiding the back option) and activates the selection.
 */
void guiml::list::receive_forward(void)
{
	if(cursor == number_of_items-1)
	{
		cursor = 0;
	}
	else
	{
		cursor++;
	}
	select();
}

/*
 * receive_backward
 * Method called by the parent object on this one when it receives a backward signal.
 * Decreases cyclically cursor (avoiding the back option) and activates the selection.
 */
void guiml::list::receive_backward(void)
{
	if(cursor == 0)
	{
		cursor = number_of_items-1;
	}
	else
	{
		cursor--;
	}
	select();
}

/*
 * get_content
 * Method to get a hardcopy of the selected item on a new allocated string
 * whose pointer is returned by the method.
 * The allocated memory for the returned string must be freed by the calling function
 * when no more useful.
 */
char* guiml::list::get_content(void)
{

	char* str_address = NULL;
	if(selected_idx!=255)
	{
		uint8_t item_len = strlen(items[selected_idx])+1;
		str_address = (char*) malloc (sizeof(char)*item_len);
		strcpy(str_address,items[selected_idx]);
	}
	else
	{
		str_address = (char*) malloc (sizeof(char)*4);
		strcpy(str_address,"   ");
	}
	return str_address;
}


/*
 * draw
 * Method that draws the page on the display.
 */
void guiml::list::draw(void)
{
	uint8_t char_height = pU8G2lib->getMaxCharHeight();
	uint16_t display_width = pU8G2lib->getDisplayWidth();
	int16_t num_rows = pU8G2lib->getDisplayHeight()/char_height;
	int16_t num_rows2 = num_rows/2;
	//int16_t num_cols = display_width/pU8G2lib->getMaxCharWidth();
	int16_t first_idx = 0;
	int16_t last_idx  = number_of_items;

	// Given the cursor position it decides what is the first and the last
	// item to be shown on the display. Different approaches if a even or odd
	// number of lines can be displayed.
	if(number_of_items + 1> num_rows)
	{
		if(num_rows%2) //odd number of rows
		{
			first_idx = cursor - num_rows2;
			last_idx = cursor + num_rows2;
			if(first_idx < 0)
			{
				first_idx = 0;
				last_idx = first_idx + num_rows -1;
			}
			else if(last_idx > number_of_items)
			{
				last_idx = number_of_items;
				first_idx = last_idx - num_rows+1;
			}
		}
		else //even number of rows
		{
			first_idx = cursor - num_rows2 + 1;
			last_idx = cursor + num_rows2;

			if(first_idx < 0)
			{
				first_idx = 0;
				last_idx = first_idx + num_rows -1;
			}
			else if(last_idx > number_of_items)
			{
				last_idx = number_of_items;
				first_idx = last_idx - num_rows+1;
			}
		}
	}

	//Serial.print("number of items: "); Serial.println(number_of_items);
	//Serial.print("number of rows: "); Serial.println(num_rows);
	//Serial.print("first_idx:"); Serial.println(first_idx);
	//Serial.print("last_idx:"); Serial.println(last_idx);

	pU8G2lib->clearBuffer();
	pU8G2lib->firstPage();

	do
	{
		int16_t j=0;
		//Serial.print("cursor: ");Serial.println(cursor);

		// drawing of the selected lines around the cursor.
		// The line under the cursor is highlighted.
		for(int16_t i=first_idx; i <= last_idx; i++)
		{

			if(i == cursor)
			{
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(0,char_height*j, display_width, char_height);
				pU8G2lib->setDrawColor(2);

				if(i<number_of_items)
					draw_sliding_string(0, char_height*j, display_width, items[i]);
				else
					draw_sliding_string(0,char_height*j, display_width,"back");
				pU8G2lib->setDrawColor(1);
			}
			else
			{
				if(i<number_of_items)
					draw_cut_string(0, char_height*j, display_width, items[i]);
				else
					draw_cut_string(0,char_height*j,display_width,"back");
			}
			j++;
		}
	}while(pU8G2lib->nextPage());
}

