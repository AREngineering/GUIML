#include "menulib.h"

/*
 * @Item
 * Constructor of the Item Class
 *
 * arg[0]: label - name of the object that will be used in other menus from which this object could be called
 * arg[1]: base_flag - boolean indication if the object is the base or home object of the menu
 *
 */
Item::Item(char* label, uint8_t base_flag)
{
	pInterface = NULL;
	pU8g2 = NULL;
	strcpy(text,label);
	is_home = base_flag;

	/*
	 * pElements is the array where the pointer of other menulib objects reacheable from this object are stored.
	 * The for cycle in the constructor initialize these pointers to NULL
	 */
	for(uint8_t i=0;i<MAX_ITEMS;i++)
	{
		pElements[i]     = NULL;
		pTextElements[i] = NULL;
		pFHandlers[i]    = NULL;
		pParents[i]      = NULL;
	}

	/*
	 * If this is the base object of the Menu the pHome and pSelected_Child are set to
	 * the address of this object. Being the base object of the menu the default to which return
	 * to it must have the pParent pointer set to NULL
	 */
	if(base_flag)
	{
		pHome           = this;
		pSelected_child = this;
		pParent         = NULL;
	}
}

/*
 * Object Destroyer frees the space that has been allocated.
 */
Item::~Item(void)
{
	for(uint8_t i = 0; i<elements_number;i++)
	{
		if(pTextElements[i] != NULL)
			free(pTextElements[i]);
	}
}


/*
 * @SetHome method
 * Sets the pHome pointer of the Object to the input argument pEl and the
 * all the pHome member of all the instantiated object linked in the pElements pointers
 * to pEl.
 *
 * arg[0]: pEl - pointer to the base object
 */
void Item::SetHome(Item *pEl)
{
	if(this != pEl && pHome != pEl)
	{
		pHome = pEl;  // Set Home for this particular Item
		//char tmpstr[MAX_CHARS];
		for(uint8_t i=0; i<elements_number; i++) // and for all the subItems that and referred in pElements;
		{
			//sprintf(tmpstr,"%s - %d/%d %s\n",text,i,elements_number,pElements[i]->text);
			//Serial.print(tmpstr);
			pElements[i]->SetHome(pEl);
		}
	}
}

/*
 * @set_selected_child private method
 * This method tells the base object of the menu what is the active selected object of the menu
 * to be displayed and with which the user is interacting
 *
 * arg[0]: pEl - address of the active object
 */
void Item::set_selected_child(Item *pEl, Item* pPar)
{
	if(pHome!=NULL && pHome->pSelected_child != pEl)
	{
		/*
		 * pHome contains a valid pointer
		 * the pSelected_child member in *pHome is not already
		 * containing the pEl pointer
		 */
		pHome->pSelected_child = pEl;
		if(!pEl->IsHome())
		{
			/*
			 * if pEl is not the pointer to the base object
			 * pEl must be specified through set_parent method the object from which it
			 * has been called.
			 */
			if(pPar == NULL)
			{
				//Serial.println("In set_selected_child if");
				//Serial.print("this address:"); Serial.println((unsigned long) this);
				pEl->set_parent(this);
			}
			else
				pEl->set_parent(pPar);
			pEl->SetHome(pHome);
			pEl->SetU8g2(pU8g2);
		}
		pEl->OnSelection(); // This makes some initialization routine on Selection of the new SelectedChild
	}
	//Serial.println("Exiting set_selected_child");
}

/*
 * @SetU8g2 method
 * Sets the pointer to the U8G2 object responsible for handling the display.
 * All the menulib object that are linked in the pElements array are set with the same pObj
 * in a "recursive" way.
 *
 * arg[0]: pObj - pointer to the U8G2 object
 */
void Item::SetU8g2(U8G2 *pObj)
{
	if(pU8g2 != pObj)
	{
		pU8g2 = pObj;
		for(uint8_t i=0; i<elements_number; i++) // and for all the subItems that and referred in pElements;
			if(pElements[i]!=NULL)
				pElements[i]->SetU8g2(pObj);
	}
}


void Item::SetPInterface(Interface *pIntfc)
{
	pInterface = pIntfc;
	//Serial.print("pInterface: ");Serial.println((unsigned long)pInterface);
}

/*
 * @SendEvent method
 * This method is used to send and Event object to the menulib object
 * linked by the pSelected_child pointer. The corresponding event handler function
 * is then called.
 *
 * arg[0]: ev - event signal (enumerator defined in Event.h)
 *
 */
void Item::SendEvent(eventSignal_t ev)
{
	/*
	 * The Event object corresponding to the signal ev is found through the pInterface member
	 * of the pSelected_child
	 */
	Event* selection = pSelected_child->pInterface->SelectEvent(ev);

	//Serial.print("Entered SendEvent - ");Serial.println((unsigned long) selection);

	/*
	 * The handler function is called.
	 */
	if(selection)
		selection->ExecHandler();
	else
	{
		Serial.print("Event ");Serial.print(ev);Serial.println(" not found");
	}
	//(void)selection;
}

/*
 * @AddElement method
 * Adds a new menulib object pointer to the pElements array
 *
 * arg[0] pEl - address of the new object to add.
 */
void Item::AddElement(char* in_str, Item *pEl, tItemHandler pHdlr)
{
	if(in_str != NULL)
	{
		pTextElements[elements_number] =
				(char*) malloc(sizeof(char)*(strlen(in_str)+1));
		strcpy(pTextElements[elements_number],in_str);
	}

	pElements[elements_number] = pEl;
	pFHandlers[elements_number] = pHdlr;

	if(elements_number < MAX_ITEMS)
	{
		elements_number++;
	}
	else
	{
		/*
		 * The max number of elements have been placed into the Item object.
		 * The system goes in stall to avoid strange behaviors
		 */
		while(1); //
	}
}

/*
 * @RemoveElement method
 * Removes an menulib object pointer from the pElements array at the idx index.
 * It reorders the array in such a way that there all the pointers in the array are
 * contiguous.
 *
 * arg[0] idx - index of the pointer to remove
 */
void Item::RemoveElement(uint8_t idx)
{
	/*
	 * The method does something only if the input index is
	 * equal or less than elements_number-1
	 */
	if(idx<elements_number-1)
	{
		/*
		 * Starting from idx+1 element all the following elements of the array are shifted back
		 * by one element. The idx element is therefore deleted keeping the array contiguous, the last
		 * pointer is set to NULL and the number of elements is then decremented,
		 */
		if(pTextElements[idx]!=NULL)
			free(pTextElements[idx]);

		for(uint8_t i=idx+1; i<elements_number; i++) // and for all the subItems that and referred in pElements;
		{
			pElements[i-1] = pElements[i];
			pTextElements[i-1] = pTextElements[i];
			pFHandlers[i-1] = pFHandlers[i];
		}
		pTextElements[elements_number-1] = NULL;
		pElements[elements_number-1] = NULL;
		pFHandlers[elements_number-1] = NULL;
		elements_number--;
	}
	else if(idx==elements_number-1)
	{
		/*
		 * The element to be removed is the last therefore the last pointer of the array is set
		 * to NULL and the number of elements decremented.
		 */
		if(pTextElements[idx]!=NULL)
			free(pTextElements[idx]);
		pTextElements[idx] = NULL;
		delete pElements[idx];
		pElements[idx]     = NULL;
		pFHandlers[idx]    = NULL;
		elements_number--;
	}
}


/*
 * @RemoveElement method
 * Removes from pElements array the pointer of the menulib object with a specific label.
 * It reorders the array in such a way that there all the pointers in the array are
 * contiguous.
 *
 * arg[0] lbl - label of the object to remove.
 */
void Item::RemoveElement(char *lbl)
{
	uint8_t idx = 255;
	uint8_t found = 0;
	/*
	 * Look through pElements to find the pointer to the object with the
	 * specified label. NB strcmp returns 0 if two strings are equal.
	 */
	for(uint8_t i=0; i<elements_number;i++)
	{
		if(pElements[i] != NULL)
		{
			if(!strcmp(lbl,pElements[i]->text))
			{
				idx=i;
				found = 1;
			}
		}
		else if(pTextElements[i] != NULL)
		{
			if(!strcmp(lbl,pTextElements[i]))
			{
				idx=i;
				found = 1;
			}
		}
	}

	if(found)
	{
		RemoveElement(idx);

//		if(idx<elements_number-1)
//		{
//			//Serial.println("Remove char idx 1");
//			for(uint8_t i=idx+1; i<elements_number; i++) // and for all the subItems that and referred in pElements;
//				pElements[i-1] = pElements[i];
//			pElements[elements_number-1] = NULL;
//			elements_number--;
//		}
//		else if(idx==elements_number-1)
//		{
//			//Serial.println("Remove char idx 2");
//			//Serial.println(elements_number);
//			pElements[idx] = NULL;
//			elements_number--;
//			//Serial.println((unsigned int) pElements[idx]);
//			//Serial.println(elements_number);
//		}
	}
}


/*
 * @draw_sliding_string private method
 * private method that allows to display on the menu a input line that slides
 * if its length is greater than what can be shown by the selected
 * graphic display. The x coordinate is zero by default. The y-coordinate
 * is set by the 2nd argument.
 *
 * args[0]: input - string to be displayed
 * args[1]: y - y coordinate at which the line must be displayes
 */
void Item::draw_sliding_string(char* input, int16_t y)
{
	int16_t display_width   = pU8g2->getDisplayWidth();
	int16_t char_width      = pU8g2->getMaxCharWidth();
	int16_t rep_string_len = display_width/char_width;
	uint8_t input_len   = strlen(input);

	//Serial.print("rep_string_len:");Serial.println(rep_string_len);
	//Serial.print("input_len: ");Serial.println(input_len);
	//Serial.print("inc_dec:"); Serial.println(input_len);



	if(input_len > rep_string_len)
	{
		/*
		 * It makes sense to shift the string only if its length is greater than
		 * the space in the display.
		 */
		//Serial.println("Input len long");
		//Serial.print("index_position: ");Serial.println(index_position);
		char* tmpstr = (char*) malloc(sizeof(char)*(rep_string_len+1));
		if(!draw_ascending && (index_position + rep_string_len == input_len) )
		{
			// the line has moved toward left and all its final part is shown on the display
			// draw_ascending = 1 makes the string move to the other direction.
			draw_ascending = 1;
		}
		else if(draw_ascending && index_position == 0)
		{
			// the line has moved toward right and all its initial part is shown on the display
			// draw_ascending = 0 makes the string move to the other direction.
			draw_ascending = 0;
		}

		for(uint8_t i=0; i<=rep_string_len; i++)
		{
			// input string is copied in the temp string
			tmpstr[i]=input[index_position+i];
		}
		tmpstr[rep_string_len] = '\0';


		pU8g2->drawStr(0, y, tmpstr);
		// The allocated space must be freed.
		free(tmpstr);
	}
	else
	{
		pU8g2->drawStr(0, y, input);
	}
}


/*
 * @update_index_position private method
 * Tells the menulib object that it must update the
 * first character to be shown in the sliding string
 */
void Item::update_index_position(void)
{
	if(draw_ascending)
		index_position--;
	else
		index_position++;
}

/*
 * @draw_multline_string private method
 * private method that allows to display on the menu a input line that will
 * be broken in multiple line with automatic new line but without hyphenation.
 * The initial coordinates by default are (0,0)
 *
 * args[0]: input - string to be displayed
 */
void Item::draw_multline_string(char *input)
{
	int16_t display_width   = pU8g2->getDisplayWidth();
	int16_t display_height  = pU8g2->getDisplayHeight();
	int16_t ascent          = pU8g2->getMaxCharHeight();
	int16_t char_width      = pU8g2->getMaxCharWidth();
	uint8_t input_len   = strlen(input);
	int16_t lines_num       = display_height/ascent;
	int16_t lines_len       = display_width/char_width;

	char *line = (char*) malloc(sizeof(char)*(lines_len+1));
	line[0]='\0';
	if(input_len > lines_num*(lines_len-2))
	{
		while(1)
		{
			Serial.print("ERROR: explanation line in "); Serial.print(text);Serial.println(" too long for the screen.");
		}
	}
	else
	{

		line[0] = '\0';
		uint8_t x_idx = 0;
		uint8_t y_idx = 0;
		uint8_t on_word = 0;
		uint8_t word_begin_x_idx =0;
		uint8_t word_begin_i = 0;
		uint8_t i = 0;



		while(1)
		{
			uint8_t isf = isspace(input[i]);
			uint8_t enl = (x_idx==lines_len);
			//Serial.print("i: ");Serial.println(i);
			//Serial.print("x_idx:");Serial.println(x_idx);
			//Serial.print("line: ");Serial.println(line);
			//Serial.print("line address:");Serial.println((int) line);
			//Serial.print("text: "); Serial.println(text);
			//Serial.print("enl: "); Serial.println(enl);


			//delay(1000);


			pU8g2->setDrawColor(1);
			if(input[i] == '\0')
			{
				// If the line is finished
				// then you write the end of string character;
				line[x_idx] = '\0';
				x_idx=0;
				y_idx++;
				pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[0] = '\0';
				break;
			}
			else if(input[i] == '\n')
			{
				// IF return character is found
				line[x_idx] = '\0';
				x_idx=0;
				y_idx++;
				pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[0] = '\0';
			}
			else if(!isf && !on_word && !enl)
			{
				// Non space character found when not on a word before end of display width
				// New word beginning
				on_word = 1;
				word_begin_i = i;
				word_begin_x_idx = x_idx;
				line[x_idx] = input[i];
				x_idx++;
				line[x_idx] = '\0';
			}
			else if(!isf && !on_word && enl)
			{
				// New word at the end of line
				on_word = 1;
				line[x_idx] = '\0';
				x_idx=0;
				y_idx++;
				word_begin_i = i;
				word_begin_x_idx = x_idx;
				//pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[x_idx] = input[i]; //First character of the new line set with the explanation_line[i] char
				x_idx++;
				line[x_idx] = '\0';
			}
			else if(!isf && on_word && !enl)
			{
				// New non space character in the current word. Display width limit has not been reached.
				line[x_idx] = input[i];
				x_idx++;
				line[x_idx] = '\0';
			}
			else if(!isf && on_word && enl)
			{
				// The current word goes outside the limit of the display width
				// The current word must go to the beginning of a new line.
				x_idx = 0;
				y_idx++;
				line[word_begin_x_idx] = '\0';
				i = word_begin_i-1; // -1 because at the end of the while i is increased by one
				pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[0] = '\0';
			}
			else if(isf && !on_word && !enl)
			{
				// Multiple space character before the limit of the display width - Nothing to do
			}
			else if(isf && !on_word && enl)
			{
				// Multiple space character at the end of the display width
				// Draw line
				line[x_idx] = '\0';
				x_idx = 0;
				y_idx++;
				pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[0] = '\0';
			}
			else if(isf && on_word && !enl)
			{
				// end of a word found before the limit of the display width
				on_word = 0;
				line[x_idx] = input[i]; //save in line the first space character
				x_idx++;
				line[x_idx] = '\0';
			}
			else if(isf && on_word && enl)
			{
				// end of a word found at the limit of the display width
				// draw line
				on_word = 0;
				x_idx = 0;
				y_idx++;
				pU8g2->drawStr(x_idx, y_idx*ascent, line);
				line[0] = '\0';
			}

			if(y_idx >  (lines_num-2))
			{
				while(1)
				{
					Serial.print("ERROR: explanation line in "); Serial.print(text);Serial.println(" too long for the screen.");
				}
			}

			if(i<input_len)
				i++;
			else
				break;
		}
	}
	free(line);
}

/*
 *
 */
void Item::increase(void)
{
	//Serial.println("In Item::increase");
	//Serial.print("cursor: "); Serial.println(cursor);
	//Serial.print("elements_number: "); Serial.println(elements_number);
	//delay(1000);
	if(cursor == elements_number-1)
	{
		cursor = 0;
	}
	else
	{
		cursor++;
	}
}

/*
 *
 */
void Item::decrease(void)
{
	Serial.println("In Item::decrease");
	Serial.print("cursor: "); Serial.println(cursor);
	Serial.print("elements_number: "); Serial.println(elements_number);
	delay(1000);
	if(cursor == 0)
	{
		cursor = elements_number-1;
	}
	else
	{
		cursor--;
	}
}

/*
 * @select method
 * Move the menu to the item selected by the cursor member
 * via the pElements array member.
 */
void Item::select(void)
{
	if(pFHandlers[cursor]!=NULL)
	{
		Serial.println("Launching pFHandler");
		pFHandlers[cursor](this);
	}

//	for(uint8_t i=0; i<elements_number;i++)
//	{
//		Serial.print("pElements ");Serial.print(i);Serial.print(": ");Serial.println((unsigned int) pElements[cursor]);
//	}
//
//	Serial.print("cursor: ");Serial.println(cursor);
//	Serial.print("pElements: ");Serial.println((unsigned int) pElements[cursor]);
	if(pElements[cursor]!=NULL)
	{

//		Serial.println("before set_selected_child");
		set_selected_child(pElements[cursor]);

	}
	else
	{
		// If pElements[cursor] is NULL the menu return to the
		// base menulib object
		//Serial.println("In Item::GoToNextItem 2");
		set_selected_child(pHome);
	}
}

/*
 * @CreateTypicalEncoderInterface
 * Helper function that creates the typical Interface for an
 * encoder with the typical three signals:
 * the encoder rotating one step Clockwise (CWS)
 * the encoder rotating one step CounterClockwise (CCWS)
 * the encoder button being pressed
 */
Interface* Item::CreateTypicalEncoderInterface(Item *pItm)
{
	Interface* pIntf = new Interface(); // Goes into dynamic memory otherwise in the stack it would be lost when the function ends
	Event* UpEvent = new Event(CWS);
	UpEvent->SetHandler((void (*)(Item*)) &Item::Increase);
	pIntf->AddEvent(UpEvent);
	Event* DownEvent = new Event(CCWS);
	DownEvent->SetHandler((void (*)(Item*)) &Item::Decrease);
	pIntf->AddEvent(DownEvent);
	Event* SelectEvent= new Event(BTN1);
	SelectEvent->SetHandler((void (*)(Item*)) &Item::Select);
	pIntf->AddEvent(SelectEvent);
	pIntf->SetTarget(pItm);
	return pIntf;
}
