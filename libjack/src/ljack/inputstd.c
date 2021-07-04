#include <ljack/dbg.h>
#include <ljack/inputstd.h>
																					/*
	I've been playing around with (my own) macros for the first time and
	made some sort of "standard input module" in the process.
																					*/
Inputstd *Inputstd_create() {
	return calloc(1, sizeof(Inputstd));
}

int Inputstd_scan(scan_cb cmp, Inputstd * self) {

	int rc = 0;
	rc = cmp(self);
	check(rc == 0,
		"error to read scan");
		
	return 0;
error:
	return 1;
}

int scan_int(Inputstd *self) {

	printf(">");
	
	long num = 0, i = 0;
	char *res = NULL;
	char temp_int[MAX_LINE] = { '\0' };
	
	res = fgets(temp_int, MAX_LINE, stdin);
	check(res != NULL,
		"error temp_int, scan_int (fgets)");
		
	for (i = 0; temp_int[i] != '\n' && i < MAX_LINE; i++) {
		if (!isdigit(temp_int[i]) || isalpha(temp_int[i])) {
			printf("\nnot an integer\ntry again\n");
			return scan_int(self);
		}
	}
	num = atoi(temp_int);
	self->std_int = num;
	check(self->std_int == num,
		"assign error scan_int, atoi");
	
	return 0;
error:
	return 1;
}

int scan_flt(Inputstd *self) {

	printf(">");
	
	int i = 0;
	double flt = 0.0;
	char *res = NULL;
	char temp_flt[MAX_LINE] = { '\0' };
	
	res = fgets(temp_flt, MAX_LINE, stdin);
	check(res != NULL,
		"error temp_flt, scan_flt (fgets)");
		 
	for (i = 0; temp_flt[i] != '\n' && i < MAX_LINE; i++) {
		if (isalpha(temp_flt[i])) {
			printf("\nnot a float\ntry again\n");
			return scan_flt(self);
		}
	}
	flt = atof(temp_flt);
	self->std_float = flt;
	check(self->std_float == flt,
		"assign error scan_flt, atof");
	
	return 0;
error:
	return 1;
}

int scan_str(Inputstd * self) {
																					/*
	concatenates each new line (temp_line) into the textbuffer (temp_text) using
	'Inputstd_merge until the user tells the program to stop (or an error occurs).
	if successfull, the program will calloc memory of size self->length to
	self->std_string before copying textbuffer[0] to [lenght-1].
																					*/
	printf("press '$' when done writing\n\n");
	
	short exit = 0, rc = 0;
	char *res = NULL;
	char temp_text[MAX_TEXT] = { '\0' };

	while (!exit) {
		char temp_line[MAX_LINE];
		check(quit(exit),
			"incorrect exit-trigger, scan_str");
		res = fgets(temp_line, MAX_LINE, stdin);
		check(res != NULL,
			"error temp_line, scan_str (fgets)");
		rc = Inputstd_merge(temp_line, temp_text, self);
		check(rc == 0,
			"error merge, scan_str");
		if (stop(self, temp_text))
			exit++;
	}
//	reducing length by one ignoring '\n' in upcoming copy
	self->length--;
//	replacing exitflag '$' with null
	temp_text[self->length - 1] = '\0';
	
	self->std_string = calloc(1, self->length);
	check(self->std_string != NULL,
		"error calloc, scan_str");

	rc = Inputstd_copy(temp_text, self);
	check(rc == 0,
		"error copy, scan_str");

	return 0;
error:
	return 1;
}

int Inputstd_merge(char *temp_line, char *temp_text, Inputstd *self) {
																					/*	
	merging linebuffer 'temp_line' into the textbuffer 'temp_text' unless
	MAX_LINE has been reached. this repeats until the user triggers an exit
	('$') or MAX_TEXT has been reached.
																					*/				
    size_t track = 0, add = 0, start = 0;
    
    start = self->length;
    for (track = 0; temp_line[track] != '\n'; track++) {
    	self->length++;
		check(line(track),
			"line overflow, Inputstd_merge");
	}
	self->length++;
    for (add = start, track = 0; add < self->length; add++, track++) {
        temp_text[add] = temp_line[track];
        check(text(self->length),
        	"text overflow, Inputstd_merge");
    }
    
    return 0;
error:
	return 1;
}

int Inputstd_copy(const char *temp, Inputstd * self) {

	size_t i = 0;

	for (i = 0; i < self->length; i++) {
		self->std_string[i] = temp[i];
		check(twin(self, temp, i),
			"unmatching character idx: %ld", i);
	}
	check(leng(self, i),
		"incorrect lengths, Inputstd_copy");
	check(null(self),
		"incorrect nulltermination, Inputstd_copy");
	
	return 0;
error:																																			
	return 1;
}
