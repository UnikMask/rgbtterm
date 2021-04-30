#ifndef TERMV_H_
#define TERMV_H_

#define NULL_TERMC {0,'\0'}
#define MIN_ARR_SIZE 100


typedef struct {
	unsigned int guicolor;
	unsigned char bytecolor;
} term_color;

typedef struct {
	term_color *termv;
	unsigned int termc;
	unsigned int allocc;
} termv;


termv *new_termv ();


term_color termv_get(termv *curr, unsigned int index);


int termv_add(termv *curr, term_color new_term_col);


void free_termv(termv *curr);

#endif // TERMV_H_
