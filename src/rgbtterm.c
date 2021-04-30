/* === === === === === === === === === === === ===
 * rgbtterm: a program that returns the terminal color closest to the given RGB color.
 * Creator: UnikMask <visorunik@gmail.com>
 * Last updated: April 30 2021
 * License: MIT
 * === === === === === === === === === === === ===
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "rgbtterm.h"
#include "errcodes.h"

const char *COL_PATH = "../termcolors.conf";

rgb new_rgb(unsigned int gui_int) {
	rgb ret_rgb = {
		(gui_int >> 16 ) & 0xFF,
		(gui_int >> 8) & 0xFF,
		gui_int & 0xFF
	};
	return ret_rgb;
}

int new_term_arr (const char fp[255], termv *term_out) {
	FILE *colstream;
	int errcode = E_SUCCESS;

	if (! (colstream = fopen(fp, "r"))) {
		perror("file");
		errcode = E_FILE;
		goto func_fail;
	}

	// Initialise terminal colors array.
	if (!term_out) {
		perror("malloc");
		errcode = E_MALLOC;
		goto func_fail;
	}

	// Parse each line in the program.
	char line[255] = "";
	for (int i = 0; !feof(colstream); i++) {
		term_color ctermc = NULL_TERMC;

		// Parse current line
		fgets(line, 255, colstream);
		if (parse_line(line, i + 1, &ctermc) != E_SUCCESS) goto func_fail;
		if (termv_add(term_out, ctermc) != E_SUCCESS) goto func_fail;
	}

	// Safely end function for success
	if (errcode == E_SUCCESS) {
		fclose(colstream);
		return E_SUCCESS;
	}
	else goto func_fail;



func_fail: // In case of function failure - safely exit function.
	if (errcode != E_MALLOC) free_termv(term_out);
	fclose(colstream);
	return errcode;

}


int parse_line (char *line, int lineno, term_color *ret_color) {
	// Initialize variables
	int errcode = E_SUCCESS;
	errno = E_SUCCESS;
	int i = 0;
	if (!ret_color) {
		perror("null pointer");
		return E_EMPTY;
	}

	// Perform first element to integer conversion.
	#define dec_digit(X) X - '0' // Define decimal digit casting
	unsigned short decs = 0;
	while (line[i] != '\n' && line[i] != ' ') {
		decs = decs * 10 + dec_digit(line[i]);
		i++;
	}
	// Check that the returned int is a byte.
	if (decs != (decs & 0xFF)) {
		printf("Why? %x \n", ret_color->bytecolor);
		goto parse_fail;
	}
	else {
		ret_color->bytecolor = ((unsigned char *) &decs)[0];
	}


	// Perform second element to integer conversion.
	#define hex_digit(X) (long) strtol((char[]){X, 0}, NULL, 16)
	while (line[i] != '\n') {
		ret_color->guicolor = ret_color->guicolor * 16 + hex_digit(line[i]);
		if (errno) goto parse_fail;
		else i++;
	}

	return errcode; // Return error code at end of function.

parse_fail: // If a parsing error happens.
		fprintf(stderr, "parsing error at line %d: %s", lineno, line);
		return E_PARSE;
}

/*
 * Compares two rgb values and returns the distance between both values.
 */
double rgbcmp(rgb a, rgb b) {
	// Get squares of all color differences
	int rs = pow(a.r - b.r, 2);
	int gs = pow(a.g - b.g, 2);
	int bs = pow(a.b - b.b, 2);

	// Return the distance
	return sqrt((double) rs + gs + bs);
}

/*
 * Main function of the program, reads the argument (has to be a color),
 * and returns the closest terminal color to it.
 */
int main(int argc, char* argv[]) {
	//Check arguments
	if (argc != 2) { // Incorrect number of arguments.
		printf("ERROR: Too many arguments! Number of arguments given: %d\n", argc - 1);
	}
	else if (argc == 2)// Number of arguments is correct
	{
		// Convert given hexadecimal to an rgb color
		if (*argv[0] == '#') *argv[0] = 0;
		int arg_int = (int) strtol(argv[1], NULL, 16);
		rgb user_rgb = new_rgb(arg_int);

		// Load terminal colors
		termv* m_termv = new_termv();
		if (new_term_arr(COL_PATH, m_termv) != E_SUCCESS) {
			free(m_termv);
			exit(EXIT_FAILURE);
		}

		// Initialize variables for comparisons.
		term_color ret_color;
		double min_dist = 255;  // Treshold for getting appropriate color.
		
		// Loop compare given color to all colors
		for (unsigned int i = 0; i < m_termv->termc; i++) {
			// Get the distance between both colors.
			rgb term_rgb = new_rgb(termv_get(m_termv, i).guicolor);
			int dist_val = (int) rgbcmp(user_rgb, term_rgb);
			if (dist_val < min_dist) { // If distance lowest, get closest color.
				min_dist = dist_val;
				ret_color = termv_get(m_termv, i);
			}
		}

		// Print final terminal color
		int fchar = (int) ret_color.bytecolor;
		free (m_termv);
		printf("Closest terminal color: %d\n", fchar);
		return 0;
	}
}
