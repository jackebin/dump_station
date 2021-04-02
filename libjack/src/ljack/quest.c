#include <ljack/dbg.h>
#include <ljack/quest.h>

// creating structs and allocs memory to store I/O for current quiz.

Quiz * Quiz_create(const int * VALS) {

    int i = 0;
    
    Quiz * quiz =  malloc(sizeof(Quiz));
    check_mem(quiz);

    quiz->max_quest = VALS[0];
    quiz->max_data  = VALS[1];

    quiz->qm = calloc(quiz->max_quest, sizeof(QuizMember));
    check_mem(quiz->qm);

    quiz->title = calloc(quiz->max_data, 1);
    check_mem(quiz->title);

     for (i = 0; i < quiz->max_quest; i++) {

        QuizMember mem = {.quest = NULL};
        quiz->qm[i] = mem;

        QuizMember * temp = &quiz->qm[i];
        check_mem(temp);

        temp->quest = calloc(quiz->max_data, 1);
        check_mem(temp->quest);

        temp->answr = calloc(quiz->max_data, 1);
        check_mem(temp->answr);

        temp->input = calloc(quiz->max_data, 1);
        check_mem(temp->input);

        temp->reslt = calloc(10, 1);
        check_mem(temp->reslt);
    }

    return quiz;

error:
    Quiz_destroy(quiz);
    return NULL;
}

// copying data for current quiz.

void Quiz_copy(Quiz * quiz, const char * TITL, const char ** ANSW, const char ** QUIZ) {

    int i = 0;

    for (i = 0; i < quiz->max_quest; i++) {

        char * in = NULL;
        
        QuizMember * temp = &quiz->qm[i];
        check_mem(temp);

        in = strncpy(temp->answr, ANSW[i], quiz->max_data);
        check(in != NULL, "Failed to copy ANSW");

        temp->quest[quiz->max_data - 1] = '\0';
        check(temp->quest[quiz->max_data - 1] == '\0',
              "Failed to NULL-terminate quest in iteration %d", i);

        in = strncpy(temp->quest, QUIZ[i], quiz->max_data);
        check(in != NULL, "Failed to copy QUIZ");

        temp->answr[quiz->max_data - 1] = '\0';
        check(temp->quest[quiz->max_data - 1] == '\0',
              "Failed to NULL-terminate answ in iteration %d", i);

    }

    char * in = strncpy(quiz->title, TITL, quiz->max_data);
    check(in != NULL, "Failed to copy TITL in iteration %d", i);

error:
    return;
}

// removes new line and stores the lenght of the string for later use in the compare func.

void rmnl_len(QuizMember * temp, int max_data) {

    int i = 0;
    for (i = 0; i < max_data - 1; i++) {
        if (temp->input[i] == '\n') {
            temp->input[i] = '\0';
        }
    }

    temp->answ_len = i+1;
}

// user inputs in a question loop.

void Quiz_input(Quiz * quiz) {

    int i = 0, now = 0;
    printf("%s\n\n", quiz->title);

    for (i = 0; i < quiz->max_quest; i++) {
    
        char * in;
        
        QuizMember * temp = &quiz->qm[i];
        check_mem(temp);

        printf("Question %d/%d\n\n", now+1, quiz->max_quest);
        printf("%s\n", temp->quest);
        printf("Answer: ");

        in = fgets(temp->input, quiz->max_data - 1, stdin);
        check(in != NULL, "Failed to input answer in iteration %d", i);

        rmnl_len(temp, quiz->max_data);
        temp->input[quiz->max_data - 1] = '\0';
        check(temp->input[quiz->max_data - 1] == '\0',
              "Failed to NULL-terminate input in iteration %d", i);
        now++;
        printf("\n");
    }

error:
    return;
}

// compares user inputs with the correct answer.

void Quiz_compare(Quiz * quiz) {

    int i = 0, j = 0, score = 0;
    char * result[] = {"Correct", "Wrong"};

    for (i = 0; i < quiz->max_quest; i++) {
    
        int control = 0;
        
        QuizMember * temp = &quiz->qm[i];
        check_mem(temp);
        
        for (j = 0; j < temp->answ_len; j++) {
            if (temp->input[j] == temp->answr[j]) {
                control++;
            } else {
                control--;
            }
        }

        if (control == temp->answ_len) {
            score += 1;
            strcpy(temp->reslt, result[0]);

        } else {
            strcpy(temp->reslt, result[1]);
        }
    }

    quiz->score = score;

error:
    return;
}

// prints result.

void Quiz_result(Quiz * quiz){

    int i = 0;

    printf("\n");
    printf("RESULT\t\tINPUT\t\t\tANSWER\t\t\tQUESTION\n\n");

    for (i = 0; i < quiz->max_quest; i++) {
    
        QuizMember * temp = &quiz->qm[i];
        printf("%s\t\t%s\t\t\t%s\t\t\t%s\n", temp->reslt, temp->input, temp->answr, temp->quest);
    }

    printf("\n");

    printf("SCORE: %d/%d\n", quiz->score, quiz->max_quest);
}

// freeing memory.

void Quiz_destroy(Quiz * quiz) {

    int i = 0;

    if (quiz) {
        if (quiz->qm) {
            for (i = 0; i < quiz->max_quest; i++) {
            
                QuizMember * temp = &quiz->qm[i];
                free(temp->quest);
                free(temp->answr);
                free(temp->input);
                free(temp->reslt);
            }
            free(quiz->qm);
        }
        if (quiz->title)
            free(quiz->title);
        free(quiz);
    }
}

// info printed in the executable in case of wrong user input.

void quiz_info(const char ** MSG, const int MSGN	) {

    int i = 0;
    
    for (i = 0; i < MSGN; i++)
        printf("%s\n", MSG[i]);
}

// runs choosen quiz.

int quiz_start(const int * VALS, const char * TITL, const char ** ANSW, const char ** QUIZ) {

    Quiz * quiz = Quiz_create(VALS);
    check(quiz != NULL, "Failed to create list.");

    Quiz_copy(quiz, TITL, ANSW, QUIZ);
    Quiz_input(quiz);
    Quiz_compare(quiz);
    Quiz_result(quiz);
    Quiz_destroy(quiz);

    return 0;

error:
    Quiz_destroy(quiz);
    return 1;
}
