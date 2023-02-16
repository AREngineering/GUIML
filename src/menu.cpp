/*
 * Guiml_Menu.cpp
 *
 *  Created on: 17 ott 2021
 *      Author: Utente
 */


#include <menu.h>

/*
 * Menu Constructor
 * Uses Item constructor
 */
guiml::menu::menu(const char *name, U8G2 *pGraf, uint8_t* fnt, item *bs, item* prnt):
	item(name, pGraf, fnt, bs,prnt)
{
	items = (item**) malloc (sizeof(item*)*1);
	// Dynamic allocation of the array of pointers to the GUI page object.
	items[0] = NULL;
}

/*
 * Menu Destructor.
 * Dynamically allocated space are freed.
 */
guiml::menu::~menu()
{
	for(uint8_t i=0; i<number_of_items; i++)
		delete(items[i]);

	free(items);
}

/*
 * void add_element(pInobj)
 * Method that adds a GUI page link to the menu at the end of the array.
 * Arguments:
 * - pInobj : pointer to the GUI page object to be inserted at the array
 */
void guiml::menu::add_element(item *pInobj)
{
	number_of_items++;
	items = (item**) realloc(items,sizeof(item*)*number_of_items);
	items[number_of_items-1] = pInobj;
}

/*
 * void add_element(pInobj)
 * Method that adds a GUI page link to the menu at a specified index of the array
 * Arguments:
 * - i : index at which the pointer must be inserted in the array
 * - pInobj : pointer to the GUI page object to be inserted in the array
 */
void guiml::menu::add_element(uint8_t i, item *pInobj)
{
	number_of_items++;
	items = (item**) realloc(items,sizeof(item*)*number_of_items);

	for(uint8_t j=i+1; j<number_of_items;i++)
	{
		items[j]=items[j-1];
	} // Shifting the elements array by one from the i-th index

	items[i] = pInobj;
}

/*
 * void remove_element()
 * Method that remove the GUI page at the end on the menu
 */
void guiml::menu::remove_element(void)
{
	delete(items[number_of_items-1]); // The object is deleted
	items[number_of_items-1]=NULL;
	number_of_items--;
	items = (item**) realloc(items,sizeof(item*)*number_of_items);
}

/*
 * void remove_element(i)
 * Method that remove the GUI page at i-th index
 * Arguments:
 * - i : index of the object to be removed from the array
 */
void guiml::menu::remove_element(uint8_t i)
{
	if(i == number_of_items-1)
	{
		remove_element();
	}
	else if(i > number_of_items-1)
	{
		while(1);
	}
	else
	{
		delete(items[i]); // delete the object
		for(uint8_t j=i+1;j<number_of_items;j++)
			items[j-1] = items[j]; // shift the object from i+1 index down.
		items[number_of_items-1] = NULL;
		number_of_items--;
		items = (item**) realloc(items,sizeof(item*)*number_of_items);
	}
}


/*
 * void remove_element(i)
 * Method that remove the GUI page with a certain label
 * Arguments:
 * - lbl : label for the search
 */
void guiml::menu::remove_element(const char *lbl)
{
	uint8_t index=255;
	for(uint8_t i=0; i < number_of_items; i++)
	{
		if(items[i]!=NULL )
		{
			// Search of the correct index
			char* item_label = items[i]->get_label();
			if(!strcmp(item_label,lbl))
			{
				index = i;
				break;
			}
			free(item_label);
		}
	}

	if(index != 255)
	{
		// Object is removed.
		remove_element(index);
	}
}

/*
 * void select()
 * Method that is used to select an object from the array of the GUI pages
 */
void guiml::menu::select(void)
{
	//Serial.println("In guiml::menu::select");
	//delay(1000);

	if(cursor < number_of_items)
	{
		//Serial.print("In guiml::menu::select()\nSelected Address: "); Serial.println((unsigned int) items[cursor]);

		// set_select method is used on the root object of the GUI and the
		// pointer to the object selected by the cursor member is passed.


		// on_selection is used to reset the status of the cursor.
		on_selection();


		//base->Activate();
	}
	else
	{
		back();
	}
}

/*
 * void increase(void)
 * Method used to increase the cursor.
 */
void guiml::menu::increase(void)
{

	if(cursor == number_of_items)
	{
		cursor = 0;
	}
	else
	{
		cursor++;
	}
//	Serial.println("In guiml::menu::increase");
//	Serial.print("base address:"); Serial.println((unsigned int) base);
//	delay(1000);
	//base->Activate();
}


/*
 * void decrease()
 * Method used to decrease the cursor.
 */
void guiml::menu::decrease(void)
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
 * void draw()
 * Method that draws the menu page.
 */
void guiml::menu::draw(void)
{
	//Variables containing geometric information of the display and on the font.
	uint8_t char_height = pU8G2lib->getMaxCharHeight();
	uint16_t display_width = pU8G2lib->getDisplayWidth();
	int16_t num_rows = pU8G2lib->getDisplayHeight()/char_height;
	int16_t num_rows2 = num_rows/2;
	//int16_t num_cols = display_width/pU8G2lib->getMaxCharWidth();
	int16_t first_idx = 0;
	int16_t last_idx  = number_of_items;

//	Serial.println(number_of_items);
//	Serial.println(num_rows);
//	Serial.println(first_idx);
//	Serial.println(num_rows2);
//	Serial.print("number of items: "); Serial.println(number_of_items);
//	Serial.print("number of rows: "); Serial.println(num_rows);
//	Serial.print("first_idx before: "); Serial.println(first_idx);
//	Serial.print("last_idx before: "); Serial.println(last_idx);

	// the number of items is increased by one because
	// after the last item label a "back" item is placed so the
	// user can return to the parent object
	if(number_of_items +1 > num_rows)
	{
		// More items in the list (with the "back" option)
		// than what can be displayed on the screen.
		if(num_rows%2) //odd number of rows
		{
			// first_idx index of the first object that will be displayed
			first_idx = cursor - num_rows2;

			// last_idx index of the last object that will be displayed
			last_idx = cursor + num_rows2;

			// first and last_idx must be placed around the value set in the cursor member

			// first_idx must be equal or higher than zero.
			// last_idx must be lower or equal to number_of_item
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
//			Serial.print("first_idx: "); Serial.println(first_idx);
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

//	Serial.print("number of items: "); Serial.println(number_of_items);
//	Serial.print("number of rows: "); Serial.println(num_rows);
//	Serial.print("first_idx after: "); Serial.println(first_idx);
//	Serial.print("last_idx after: "); Serial.println(last_idx);

	//pU8G2lib->firstPage();
	//do
	//{
//	Serial.print("u8g2 address:");Serial.println((unsigned int) pU8G2lib);


	pU8G2lib->clearBuffer(); // the display buffer is cleared
	pU8G2lib->firstPage();

	do
	{
		uint8_t j=0;

		for(int16_t i=first_idx; i <= last_idx; i++)
		{
			char* lbl =NULL;
			char* cnt =NULL;

			if(i<number_of_items)
			{
				// Both content and label of the items to be displayed are copied.
				cnt = items[i]->get_content();
				if(cnt == NULL)
				{
					cnt = (char*) malloc(sizeof(char));
					cnt[0] = '\0';
				}
				lbl = items[i]->get_label();
			}
			else
			{
				// For the back option label is set to "back";
				// content to a blank string
				lbl = (char*) malloc(sizeof(char)*5);
				strcpy(lbl,"back");
				cnt = (char*) malloc(sizeof(char));
				cnt[0]='\0';
			}

			// display the item under the cursor
			if(i == cursor)
			{
				// Font mode and draw color are set to have an highlighting of the object
				// under the cursor.
				pU8G2lib->setFontMode(1);
				pU8G2lib->setDrawColor(1);
				pU8G2lib->drawBox(0,char_height*j, display_width, char_height);
				pU8G2lib->setDrawColor(2);

				// The label of the object under cursor is drawn as a sliding string.
				// 3/4 of the width of the display is reserved to the labels
				draw_sliding_string(0, char_height*j, 0.73*display_width, lbl);

				// The content as a cut string
				// 1/4 of the display width is reserved to the content of the object in the array.
				draw_cut_string(0.77*display_width, char_height*j, display_width-1, cnt);
				pU8G2lib->setDrawColor(1);
			}
			else
			{
				// Object not under the cursor are drawn without highlighting and label and content are cut strings.
				draw_cut_string( 0, char_height*j, 0.73*display_width, lbl);
				draw_cut_string( 0.77*display_width, char_height*j, display_width-1, cnt);
			}
			j++;
			if(cnt != NULL)
				free(cnt);

			if(lbl !=NULL)
				free(lbl);
		}
	}
	while(pU8G2lib->nextPage());
}


/*
 * void forward()
 * Method to send to the object under the cursor a forward signal.
 */
void guiml::menu::forward(void)
{
	if(cursor < number_of_items)
	{
		if(items[cursor]!=NULL)
		{
			// The receive_forward method is used on the object under the cursor.
			items[cursor]->receive_forward();
		}
	}
	//base->Activate();
}

/*
 * void backward()
 * Method to send to the object under the cursor a backward signal.
 */
void guiml::menu::backward(void)
{
	if(cursor < number_of_items)
	{
		if(items[cursor]!=NULL)
		{
			items[cursor]->receive_backward();
		}
	}
	//base->Activate();
}

/*
 * void back()
 * Method to return to the parent if this object.
 */
void guiml::menu::back(void)
{
	if(!is_base) // nothing to do is this object is the root one of the GUI.
	{
		on_leave();
	}
	//base->Activate();
}

/*
 * void on_selection()
 * reset the cursor to 0 when an object in the array is selected.
 * User can specify a pointer to an external function to operate changes on global variables on selection.
 */
void guiml::menu::on_selection(void)
{
	if(pFon_selection != NULL)
	{
		pFon_selection(this);
	}

	item *itm = items[cursor];
	cursor = 0;

	pBase->set_selected(itm);
}

/*
 * void on_leave()
 * reset the cursor to 0 when the user returns to the parent object.
 * User can specify a pointer to an external function to operate changes on global variables on leave.
 */
void guiml::menu::on_leave(void)
{
	if(pFon_leave != NULL)
	{
		pFon_leave(this);
	}
	cursor = 0;

	if(pItem_on_leave != NULL)
		pBase->set_selected(pItem_on_leave);
	else
		pBase->set_selected(pParent);
}
