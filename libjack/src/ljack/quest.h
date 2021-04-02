#ifndef ljack_Quest_h
#define ljack_Quest_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct QuizMember {
    int answ_len;
    char * answr;
    char * quest;
    char * input;
    char * reslt;

} QuizMember;

typedef struct Quiz {
    int score;
    int max_quest;
    int max_data;
    char * title;
    QuizMember * qm;

} Quiz;

Quiz * Quiz_create(const int * VALS);
void rmnl_len(QuizMember * temp, int max_data);
void quiz_info(const char ** MSG, const int MSGN);
int  quiz_start(const int * VALS, const char * TITL, const char ** ANSW, const char ** QUIZ);


void Quiz_copy(Quiz * quiz, const char * TITL, const char ** ANSW, const char ** QUIZ);
void Quiz_input(Quiz * quiz);
void Quiz_compare(Quiz * quiz);
void Quiz_result(Quiz * quiz);
void Quiz_destroy(Quiz * quiz);

#endif
