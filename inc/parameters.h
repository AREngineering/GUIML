/*
 * Guiml_parameters.h
 *
 *  Created on: 24 nov 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_PARAMETERS_H_
#define LIBRARY_INC_GUIML_PARAMETERS_H_

#define REFRESH_DELAY 300 //[ms]
#define MAX_CONTENT_LENGTH 32
#define MAX_STR_LENGTH 32
#define MAX_EVENT_LENGTH 16
#define MAX_NUM_ITEMS 16

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <U8g2lib.h>

namespace guiml
{
	class item;
	typedef void (*handler_t)(item*);
}
#endif /* LIBRARY_INC_GUIML_PARAMETERS_H_ */
