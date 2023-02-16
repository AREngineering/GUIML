/*
 * Guiml_Event.cpp
 *
 *  Created on: 24 nov 2021
 *      Author: Utente
 */


#include <event.h>

/*
 * Event constructor
 * Arguments:
 * - nm       : string containing the signal name
 * - callback : pointer to the function handling the signal
 * - tgt      : pointer to the GUI object on which the handler will operate.
 */
guiml::event::event(const char* nm, handler_t callback, item *tgt)
{
	uint8_t nm_len = strlen(nm)+1;

	// dynamic allocation of the memory for the name string.
	name = (char*) malloc(sizeof(char)*nm_len);
	strcpy(name,nm);
	pFHandler = callback;
	//Serial.print("before target:");Serial.println((unsigned int)target);
	target = tgt;
	//Serial.print("after target:");Serial.println((unsigned int)target);
}

/*
 * Event destructor
 * Frees the dynamically allocated memory
 */
guiml::event::~event()
{
	free(name);
}


/*
 * uint8_t activate(signal)
 * if the signal string is equal to the name member one the function linked by pFHandler will be called on the
 * target item object.
 */
uint8_t guiml::event::activate(const char *signal)
{
	// Sanity check if pFHandler is NULL
	// Error system hangs
	//if(pFHandler == NULL)
	//	while(1);


	uint8_t res = 0;

	if(!strcmp(name,signal))
	{
		//Serial.print("name: "); Serial.println(name);
		//Serial.print("target address:");Serial.println((unsigned int) target);
		//Serial.print("pFHandler address:"); Serial.println((unsigned int) pFHandler);
		pFHandler(target);
		res = 1;
//	{
//
	}
	return res;
}

