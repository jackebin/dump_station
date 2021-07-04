#ifndef ljack_inputstd_h
#define ljack_inputstd_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TEXT 1000
#define MAX_LINE 100

typedef struct Inputstd {
    long std_int;
    double std_float;    
    char *std_string;
    size_t length;
} Inputstd;

Inputstd *Inputstd_create();

typedef int (*scan_cb) (Inputstd *self);

int scan_int(Inputstd *self);
int scan_flt(Inputstd *self);
int scan_str(Inputstd *self);
																/*
	macro	checks (if true)

	stop	done writing
	quit	whileloop exit
	null	nullterminated
	twin	identical char 
	leng	identical length
	line	line length lesser than MAX_LINE
	text	text/string length lesser than MAX_TEXT
																*/
#define stop(A, T) ((T[(A)->length - 2]) == '$')
#define quit(E) ((E) == 0)
#define null(A) ((A)->std_string[(A)->length - 1] == '\0')
#define twin(A, N, I) ((A)->std_string[I] == N[I])
#define leng(A, I) ((A)->length == I)
#define line(L) ((L) < MAX_LINE)
#define text(X) ((X) < MAX_TEXT)
																					
int Inputstd_scan(scan_cb cmp, Inputstd * self);
int Inputstd_copy(const char *temp, Inputstd *self);
int Inputstd_merge(char *temp_line, char *temp_text, Inputstd *self);

#define std_kill(A) if((A)) {\
		if((A)->std_string) free((A)->std_string); free((A)); }

#endif












