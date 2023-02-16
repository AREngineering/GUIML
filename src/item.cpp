/*
 * item.cpp
 *
 *  Created on: 13 Oct 2021
 *      Author: revel
 */

#include <item.h>

/*
 * Item Class Constructor
 * Arguments
 * - name  : name associated to the object
 * - pGraf : pointer to the U8G2 library object
 * - fnt   : default font to be used
 * - bs    : pointer to the base object (default NULL)
 * - prnt  : pointer to the parent object (default NULL)
 */
guiml::item::item(const char *name, U8G2 *pGraf, uint8_t *fnt, item *bs, item *prnt)
{
	uint8_t name_len = strlen(name)+1; //+1 to allocate the \0 character at the end of the char array
	/* Dinamic allocation of the space for the strings
	 * reduces memory consumption*/
	label = (char *) malloc(sizeof(char)*name_len);
	content = (char *) malloc(sizeof(char)*MAX_CONTENT_LENGTH);
	strcpy(label, name);
	pInterface = new interface(); // The Interface object is handled through the Item Object and its derivates.
							      // All the interaction happen at the Item object level. It is not created outside of the
	                              // object scope and passed by reference.

	pU8G2lib = pGraf;
	font = fnt;
	pSelected = NULL;

	/* With bs==NULL the created object is considered the base one
	 * it has no base (it is the base) and is has no parent object
	 * (it is the parent object.)*/
	if(bs == NULL)
	{

		is_base = 1;
		pBase = this;
		pSelected = this;
	}
	else
	{
		is_base = 0;
		pBase = bs;
	}

	if(prnt != NULL)
	{
		pParent = prnt;
	}
}

/*
 * Item Class Destructor
 * Since there is dynamic allocation of space in the constructor
 * there must be a destructor that frees that allocated memory to
 * avoid memory leak
 */
guiml::item::~item()
{
	free(label);
	free(content);
	delete(pInterface); // Being created in the heap memory inside the Item object scope it must be destructed in the object scope.
}


/*
 * char* get_label(void)
 * Returns a copy of the label string allocated in the heap.
 * The user must delete it when no more useful
 */
char* guiml::item::get_label(void)
{
	char *str_add;
	uint8_t label_len = strlen(label)+1;
	str_add = (char*) malloc(sizeof(char)*label_len);
	strcpy(str_add,label);
	return str_add;
}

/*
 * void add_event(sig_name, pHandler)
 * Function that to the interface of the object the handler function pointer for the signal named with the string sig_name
 * Arguments:
 * - sig_name: name of the signal
 * - pHandler : function pointer to the handler of the signal named by sig_name
 */
void guiml::item::add_event(const char* sig_name, handler_t pHandler)
{
	//Serial.print("this: ");Serial.println((unsigned int)this);
	pInterface->add_event(new event(sig_name,pHandler,this));
}

/*
 * void activate()
 * Method that activates the drawing of the selected page on the screen.
 * Must be called only on the base object of the GUI.
 */
void guiml::item::activate(void)
{
	if(is_base)
	{
		pSelected->draw();
		pSelected->update_start_index = 1;
	}
	else
		while(1);
}

/*
 * void set_parent(pObj)
 * Method that indicates to the object its parent object pointer.
 * Must be used ONLY on objects that are not the base one of the GUI.
 * Argument:
 * - pObj: pointer to the parent object.
 */
void guiml::item::set_parent(item *pObj)
{
	if(!is_base)
		pParent=pObj;
	else
		while(1);
}


/*
 * void send_signal(signal)
 * Method that must be used only on the base object.
 * It sends to the selected object the signal indicated
 * by the signal. This will be passed to the selected object
 * via the receive_signal method.
 * Argument:
 * - signal: name of the signal
 */
void guiml::item::send_signal(const char *signal)
{
	//if(is_base)
	pSelected->receive_signal(signal);
	//else
		//while(1);
}

/*
 * void receive_signal(signal)
 * Method that is used on the selected object.
 * Argument:
 * - signal : name of the signal.
 */
void guiml::item::receive_signal(const char *signal)
{
	pInterface->receive_event(signal);
}

/*
 * void set_selected(pObj)
 * Method that tells the base object of the menu
 * which is the guiml object that is selected to
 * be displayed and interacted with.
 * Arguments:
 * - pObj : pointer to the selected object.
 */
void guiml::item::set_selected(item *pObj)
{
	if(is_base)
		pSelected = pObj;
	else if(pBase != NULL)
		pBase->set_selected(pObj);
	else
		while(1)
			Serial.println("Runtime Error in guiml::item::set_selected");
}

/*
 * void draw_sliding_string(xmin,ymin,xmax,in)
 * Method that prints a sliding string.
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - xmax : x coordinate of the ending point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_sliding_string(int16_t xmin, int16_t ymin, int16_t xmax, const char* in)
{
	aux_draw_sliding_string(pU8G2lib, xmin, ymin, xmax, &ascending, &start_index, &update_start_index, in);
}

/*
 * void draw_sliding_string(xmin,ymin,in)
 * Method that prints a sliding string from a starting point to the end of the screen.
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_sliding_string(int16_t xmin, int16_t ymin, const char* in)
{
	aux_draw_sliding_string(pU8G2lib, xmin, ymin, &ascending, &start_index, &update_start_index, in);
}

/*
 * void draw_sliding_string(in)
 * Method that prints a sliding string from (0,0) coordinates to the end of the display.
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - in   : string to be displayed
 */
void guiml::item::draw_sliding_string(const char* in)
{
	aux_draw_sliding_string(pU8G2lib, &ascending, &start_index, &update_start_index, in);
}

/*
 * void draw_split_string(xmin,ymin,xmax,ymax,in)
 * Method that prints a split string in the rectangle area with corners (xmin,ymin) and (xmax,ymax).
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - xmax : x coordinate of the ending point where the string is shown on the display
 * - ymax : x coordinate of the ending point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_split_string(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, const char *in)
{
	aux_draw_split_string(pU8G2lib, xmin, ymin, xmax, ymax, in);
}

/*
 * void draw_split_string(xmin,ymin,xmax,ymax,in)
 * Method that prints a split string in the rectangle area with corners (xmin,ymin) and the maximum coordinates of the display.
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_split_string(int16_t xmin, int16_t ymin, const char *in)
{
	aux_draw_split_string(pU8G2lib, xmin, ymin, in);
}

/*
 * void draw_split_string(xmin,ymin,xmax,ymax,in)
 * Method that prints a split string in the rectangle area with corners (0,0) and the maximum coordinates of the display.
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - in   : string to be displayed
 */
void guiml::item::draw_split_string(const char *in)
{
	aux_draw_split_string(pU8G2lib, in);
}

/*
 * void draw_cut_string(xmin,ymin,xmax,in)
 * Method that prints the part of the in string that is covered by the rectangle
 * with corners (xmin,ymin) and (xmax,ymin+font_height)
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - xmax : x coordinate of the ending point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_cut_string(int16_t xmin, int16_t ymin, int16_t xmax, const char* in)
{
	aux_draw_cut_string(pU8G2lib, xmin, ymin, xmax, in);
}

/*
 * void draw_cut_string(xmin,ymin,xmax,in)
 * Method that prints the part of the in string that is covered by the rectangle
 * with corners (xmin,ymin) and (display_xmax,ymin+font_height)
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - xmin : x coordinate of the starting point where the string is shown on the display
 * - ymin : y coordinate of the starting point where the string is shown on the display
 * - in   : string to be displayed
 */
void guiml::item::draw_cut_string(int16_t xmin, int16_t ymin, const char* in)
{
	aux_draw_cut_string(pU8G2lib, xmin, ymin, in);
}

/*
 * void draw_cut_string(xmin,ymin,xmax,ymax,in)
 * Method that prints a split string in the rectangle area with corners (0,0) and (display_xmax,font_height)
 * The actual drawing is carried out by the auxiliary function.
 * Arguments:
 * - in   : string to be displayed
 */
void guiml::item::draw_cut_string(const char* in)
{
	aux_draw_cut_string(pU8G2lib, in);
}

