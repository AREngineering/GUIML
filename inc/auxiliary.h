/*
 * Guiml_auxiliary.h
 *
 *  Created on: 26 dic 2021
 *      Author: Utente
 */

#ifndef LIBRARY_INC_GUIML_AUXILIARY_H_
#define LIBRARY_INC_GUIML_AUXILIARY_H_

#include "U8g2lib.h"
#include "Arduino.h"


/* Functions to use the U8G2 library and draw a string sliding from left to right or right to left */
void aux_draw_sliding_string(U8G2 *pU8G2lib, int16_t xmin, int16_t ymin, int16_t xmax, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in);
void aux_draw_sliding_string(U8G2 *pU8G2lib, int16_t xmin, int16_t ymin, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in);
void aux_draw_sliding_string(U8G2 *pU8G2lib, uint8_t *ascending, uint16_t *start_index, uint8_t *update_start_index, const char* in);

/* Functions to use the U8G2 library and draw a string split such that words are not truncated. */
void aux_draw_split_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, const char *in);
void aux_draw_split_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, const char *in);
void aux_draw_split_string(U8G2 *pU8G2lib,const char *in);

/* Functions to use the U8G2 library and draw a cut string*/
void aux_draw_cut_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, int16_t xmax, const char* in);
void aux_draw_cut_string(U8G2 *pU8G2lib,int16_t xmin, int16_t ymin, const char* in);
void aux_draw_cut_string(U8G2 *pU8G2lib,const char* in);


#endif /* LIBRARY_INC_GUIML_AUXILIARY_H_ */
