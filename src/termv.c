#include <stdio.h>
#include <stdlib.h>

#include "termv.h"
#include "errcodes.h"


termv *new_termv() {
	termv *this = malloc(sizeof(termv));
	if (this) {
		this->termv = calloc(MIN_ARR_SIZE, sizeof(term_color));
		if (this->termv) {
			this->termc = 0;
			this->allocc = MIN_ARR_SIZE;
			return this;
		}
		else free(this);
	}
	return NULL;
}


term_color termv_get(termv *this, unsigned int index) {
	term_color ret_col = NULL_TERMC;
	if (index < this->termc) ret_col = this->termv[index];
	return ret_col;
}


int termv_add(termv *this, term_color new_term_color) {
	if (this->termc == this->allocc) {
		term_color *tmpp =
			realloc(this->termv,
			(this->termc + MIN_ARR_SIZE) * sizeof(term_color));
		if (tmpp) {
			this->termv = tmpp;
			this->allocc += MIN_ARR_SIZE;
			return termv_add(this, new_term_color);
		}
		else {
			perror("realloc");
			return E_REALLOC;
		}
	}
	else {
		this->termv[this->termc] = new_term_color;
		this->termc++;
		return E_SUCCESS;
	}
}


void free_termv(termv *this) {
	free(this->termv);
	free(this);
}
