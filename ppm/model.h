#ifndef MODEL_H
#define MODEL_H

#pragma once

/* INTERFACE TO THE MODEL */


/* THE SET OF SYMBOLS THAT MAY BE ENCODED */

/* MACROS */
#define No_of_chars	256			            /* Number of character symbols */
#define EOF_symbol	(No_of_chars + 2)	    /* Index of EOF symbol	*/
#define ESC_symbol	(1)						/*ESC index*/

#define No_of_symbols 	(No_of_chars + 2)	/* Total number of symbols */
#define Max_frequency	16383			/* Maximum allowed frequency count 	*/
						                /* 2^14 -1 	            			*/


/* TRANSLATION TABLES BETWEEN CHARACTERS AND SYMBOL INDEXES */
extern int char_to_index[No_of_chars];			       /* To index from charater */
extern unsigned char index_to_char[No_of_symbols+1];  /* To character from index */


/* CUMULATIVE FREQUENCY TABLE */

//extern int *cum_freq;//[No_of_symbols+1];			/* Cummulative symbol frequencies 	*/

#endif
