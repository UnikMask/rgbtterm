#ifndef _RGBTTERM_H_
#define _RGBTTERM_H_

#include "termv.h"

#define NULL_RGB {'\0', '\0', '\0'}

const unsigned int N_COLORS = 240;

typedef struct {
	unsigned char r, g, b;
} rgb;

int parse_line (char *line, int lineno, term_color *ret_color);


rgb new_rgb(unsigned int gui_int);


int new_term_arr (const char fp[255], termv *term_arr);


#endif // _RGBTTERM_H_
