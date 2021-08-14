#include <ljack/dbg.h>
#include <ljack/inputstd.h>
																					/*
	updated version of this "standard input module" I made while playing
	around with macros. most of the functions have a quite naive approach
	but they've actually worked quite flawless with other projects.
	
	main difference from earlier is the "wrappers" taking care of all
	"create>what>ok>kill"-logic instead of having to code this procedure
	for every little input in my projects.
																					*/
Inputstd *Inputstd_create() {
	return calloc(1, sizeof(Inputstd));
}

char Inputstd_scan(scan_cb cmp, Inputstd * self) {

	int rc = 0;
	rc = cmp(self);
	check(rc == 0, "error to read scan");
		
	return 0;
error:
	return 1;
}

char scan_int(Inputstd *self) {
	
	long num = 0, i = 0;
	char *res = NULL;
	char temp_int[MAX_LINE] = { '\0' };
	
	res = fgets(temp_int, MAX_LINE, stdin);
	check(res != NULL, "error temp_int, scan_int (fgets)");
		
	for (i = 0; temp_int[i] != '\n' && i < MAX_LINE; i++) {
		if (!isdigit(temp_int[i]) || isalpha(temp_int[i])) {
			printf("not an integer, try again\n");
			return scan_int(self);
		}
	}
	num = atoi(temp_int);
	self->std_int = num;
	check(self->std_int == num, "assign error scan_int, atoi");
	
	return 0;
error:
	return 1;
}

char scan_flt(Inputstd *self) {
	
	int i = 0;
	double flt = 0.0;
	char *res = NULL;
	char temp_flt[MAX_LINE] = { '\0' };
	
	res = fgets(temp_flt, MAX_LINE, stdin);
	check(res != NULL, "error temp_flt, scan_flt (fgets)");
		 
	for (i = 0; temp_flt[i] != '\n' && i < MAX_LINE; i++) {
		if (isalpha(temp_flt[i])) {
			printf("not a float, try again\n");
			return scan_flt(self);
		}
	}
	flt = atof(temp_flt);
	self->std_float = flt;
	check(self->std_float == flt, "assign error scan_flt, atof");
	
	return 0;
error:
	return 1;
}

char scan_str(Inputstd * self) {
																					/*
	concatenates each new line (temp_line) into the textbuffer (temp_text) using
	'Inputstd_merge until the user tells the program to stop (or an error occurs).
	if successfull, the program will calloc memory of size self->length to
	self->std_string before copying textbuffer[0] to [lenght-1].
																					*/
	short exit = 0, rc = 0;
	char *res = NULL;
	char temp_text[MAX_TEXT] = { '\0' };

	while (!exit) {
		char temp_line[MAX_LINE];
		check(std_quit(exit), "incorrect exit-trigger, scan_str");
		
		res = fgets(temp_line, MAX_LINE, stdin);
		check(res != NULL, "error temp_line, scan_str (fgets)");
		
		rc = Inputstd_merge(temp_line, temp_text, self);
		check(rc == 0, "error merge, scan_str");
		
		if (std_stop(self, temp_text))
			exit++;
	}
//	reducing length by one ignoring '\n' in upcoming copy
	self->length--;
//	replacing exitflag '$' with null
	temp_text[self->length - 1] = '\0';
	
	self->std_string = calloc(1, self->length);
	check(self->std_string != NULL, "error calloc, scan_str");

	rc = Inputstd_copy(temp_text, self);
	check(rc == 0, "error copy, scan_str");

	return 0;
error:
	return 1;
}

char Inputstd_merge(char *temp_line, char *temp_text, Inputstd *self) {
																					/*
	assigns 'start' to latest self->lenght while 'track' keeping, well - track,
	of current length up to next '\n'.
	
	merging linebuffer 'temp_line' into the textbuffer 'temp_text' unless
	MAXLINE has been reached. This repeats until the user triggers an exit
	('$' + enter) or MAXTEXT has been reached.
																					*/				
    size_t track = 0, add = 0, start = 0;
    
    start = self->length;
    for (track = 0; temp_line[track] != '\n'; track++) {
    	self->length++;
		check(std_line(track), "line overflow, Inputstd_merge");
	}
	self->length++;
    for (add = start, track = 0; add < self->length; add++, track++) {
        temp_text[add] = temp_line[track];
        check(std_text(self->length), "text overflow, Inputstd_merge");
    }
    
    return 0;
error:
	return 1;
}

char Inputstd_copy(const char *temp, Inputstd * self) {
//	metacopy
	size_t i = 0;

	for (i = 0; i < self->length; i++) {
		self->std_string[i] = temp[i];
		check(std_twin(self, temp, i), "unmatching character idx: %ld", i);
	}
	check(std_leng(self, i), "incorrect lengths, Inputstd_copy");
	check(std_null(self), "incorrect nulltermination, Inputstd_copy");
	
	return 0;
error:																																			
	return 1;
}
char Inputstd_strcpy(char *to, const char *from, size_t len) {
//	allround copy
	size_t i = 0;
	
	for (i = 0; i < len; i++) to[i] = from[i];
		to[len - 1] = '\0';
	check(to[len - 1] == '\0', "failed to nullterminate string");

	return 0;
error:
	return 1;
}

long std_numwrap() {
//	returns long int
	char rc = 0;
	long ret = 0;

	Inputstd * input = Inputstd_create();
	check(input != NULL, "failed to create input");
	
	rc = Inputstd_scan(scan_int, input);
	check(rc == 0, "failed to scan numwrap");
	
	ret = input->std_int;
	
	std_kill(input);
	return ret;
error:
	std_kill(input);
	return 1; 
}

long std_numnwrap(const long LOW, const long HIGH, const char *wrong_mess) {
//	returns long int with condition set between LOW and HIGH.
	char rc = 0;
	long ret = 0;

	Inputstd * input = Inputstd_create();
	check(input != NULL, "failed to create input");
	
	rc = Inputstd_scan(scan_int, input);
	check(rc == 0, "failed to scan numnwrap");
	
	ret = input->std_int;

	if (std_stdn(ret, LOW, HIGH)) {
		std_kill(input);
		return ret;
	} else {
		printf("%s", wrong_mess);
		std_kill(input);
		return std_numnwrap(LOW, HIGH, wrong_mess);
	}
	
error:
	std_kill(input);
	return 1; 
}

double std_fltwrap() {
//	returns double
	char rc = 0;
	double ret = 0;

	Inputstd * input = Inputstd_create();
	check(input != NULL, "failed to create input");
	
	rc = Inputstd_scan(scan_flt, input);
	check(rc == 0, "failed to scan fltwrap");
	
	ret = input->std_float;
	
	std_kill(input);
	return ret;
error:
	std_kill(input);
	return 1; 
}

double std_fltnwrap(const double LOW, const double HIGH, const char *wrong_mess) {
//	returns double with condition set between LOW and HIGH.
	char rc = 0;
	double ret = 0;

	Inputstd * input = Inputstd_create();
	check(input != NULL, "failed to create input");
		
	rc = Inputstd_scan(scan_flt, input);
	check(rc == 0, "failed to scan fltnwrap");
	
	ret = input->std_int;
		
	if (ret < LOW || ret > HIGH) {
		printf("%s", wrong_mess);
		std_kill(input);
		return std_numnwrap(LOW, HIGH, wrong_mess);
	}
		
	std_kill(input);
	return ret;
error:
	std_kill(input);
	return 1; 
}


char std_strwrap(char * strwrap) {	//
//	returns a string.
	char rc = 0;
	
	Inputstd *input = Inputstd_create();
	check(input != NULL, "failed to create input");

	rc = Inputstd_scan(scan_str, input);
	check(rc == 0, "failed to scan strwrap");
	
	rc = Inputstd_strcpy(strwrap, input->std_string, input->length);
	check(rc == 0, "failed to copy");

	std_kill(input);
	return 0;
error:
	std_kill(input);
	return 1;
}	

char std_strnwrap(char * strnwrap, size_t * len_strnw) {
//	returns a string and its size (through len_strnw ptr).
	short rc = 0;

	Inputstd *input = Inputstd_create();
	check(input != NULL, "failed to create input");

	rc = Inputstd_scan(scan_str, input);
	check(rc == 0, "failed to scan strnwrap");
	
	rc = Inputstd_strcpy(strnwrap, input->std_string, input->length);
	check(rc == 0, "failed to copy");

	*len_strnw = input->length;

	std_kill(input);
	return 0;
error:
	std_kill(input);
	return 1;
}	

char std_validate(const char *wrong_mess) {
//	returns 'TRUE' or 'FALSE'.
	long hedge = 0;
	char rc = 0, ret = 0;
	
	Inputstd * input = Inputstd_create();
	check(input != NULL, "failed to create input");
	
	rc = Inputstd_scan(scan_int, input);
	check(rc == 0, "failed to return integer");
	
	hedge = input->std_int - 1;
	
	if(!std_bins(hedge)) {
		printf("%s", wrong_mess);
		std_kill(input);
		return std_validate(wrong_mess);
	}	
	ret = (char)hedge;
	std_kill(input);
	return ret;		

error:
	std_kill(input);
	return -1; 																			
}

void std_kill(Inputstd * self) {
	if (self) {
		if (self->std_string) {
			free(self->std_string);
			self->std_string = NULL;
		}
		free(self);
		self = NULL;
	}	
}
