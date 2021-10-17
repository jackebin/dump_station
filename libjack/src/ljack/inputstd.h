#ifndef libjack_inputstd_h
#define libjack_inputstd_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TEXT 2000
#define MAX_LINE 100

typedef struct Inputstd {
    long std_int;
    double std_float;    
    char *std_string;
    size_t length;
} Inputstd;

Inputstd *Inputstd_create();

typedef char (*scan_cb) (Inputstd *self);

char scan_int(Inputstd *self);
char scan_flt(Inputstd *self);
char scan_str(Inputstd *self);
																					/*
	macro		checks (if true)

	std_stop	done writing
	std_quit	whileloop exit
	std_null	nullterminated
	std_twin	identical char 
	std_leng	identical length
	std_line	line length lesser than MAX_LINE
	std_text	text/string length lesser than MAX_TEXT
	std_bins	within range of "y and n"
	std_stdn	within range of input LOW and MAX 
																					*/
#define std_stop(A, T) ((T[(A)->length - 2]) == '$')
#define std_quit(E) ((E) == 0)
#define std_null(A) ((A)->std_string[(A)->length - 1] == '\0')
#define std_twin(A, N, I) ((A)->std_string[I] == N[I])
#define std_leng(A, I) ((A)->length == I)
#define std_line(L) ((L) < MAX_LINE)
#define std_text(X) ((X) < MAX_TEXT)

#define std_bins(N) ((N) == 0 || (N) == 1)
#define std_stdn(I, L, H) ((I) >= (L) && (I) <= (H))															

char Inputstd_scan(scan_cb cmp, Inputstd * self);
char Inputstd_merge(char *temp_line, char *temp_text, Inputstd *self);
char Inputstd_copy(const char *temp, Inputstd *self);
char Inputstd_strcpy(char *to, const char *from, size_t len);

long std_numwrap();
long std_numnwrap(const long LOW, const long HIGH, const char *wrong_mess); 
double std_fltwrap();
double std_fltnwrap(const double LOW, const double HIGH, const char *wrong_mess);
char std_strwrap(char *strnwrap);
char std_strnwrap(char *strnwrap, size_t *len_strnw);
char std_validate(const char * wrong_mess);

void std_kill(Inputstd * self);

#endif
