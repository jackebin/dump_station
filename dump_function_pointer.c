#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
simple program for simple arithmetic operations. it's actually
a very stupiud build for the task but I really wanted to play
around with a function pointer so here we are.
*/

typedef void (*math_cb)(float a, float b); // <- the pointer.

const char * ACTION[] = {
    "Addition", "Subtraction",
    "Multiplication", "Division"
};
void kill(const char * message) {

    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s", message);
    }

    exit(1);
}

void runit(float a, float b, math_cb cbf) {
    cbf(a, b);
}

void addit(float a, float b) {
    printf("%2.2f + %2.2f = %2.2f\n", a, b, a+b);
}
void subit(float a, float b) {
    printf("%2.2f - %2.2f = %2.2f\n", a, b, a-b);
}
void mulit(float a, float b) {
    printf("%2.2f x %2.2f = %2.2f\n", a, b, a*b);
}
void divit(float a, float b) {
    float res = a / b;
    printf("%2.2f / %2.2f = %2.2f\n", a, b, res);
}

int main(int argc, char * argv[]) {
    system("clear");

    float a, b;
    int i, rc;

    printf("Choose operation: \n");
    for (i = 0; i < 4; i++) {
        printf("%d) %s\n", i + 1, ACTION[i]);
    }
    printf(">  ");

    int action = -1;
    rc = fscanf(stdin, "%d", &action);
    if (rc < 1)
        kill("Input error.\n\n");

    system("clear");
    printf("Choose operation: \n");
    for (i = 1; i < 5; i++) {
        if (i == action) {
            printf("%d) %s %s\n", i, ACTION[action-1], "<-");
        } else {
            printf("%d) %s\n", i, " ");
        }
    }

    printf("\n");

    printf("First number: ");
    rc = fscanf(stdin, "%f", &a);
    if (rc < 1)
        kill("Input error.\n\n");

    printf("Second number: ");
    rc = fscanf(stdin, "%f", &b);
    if (rc < 1)
        kill("Input error.\n\n");

    printf("\n");
/*
    addit(a, b) etc. had worked all fine but here we're
    witnessing - in a very unnecessary way - a
    function pointer doing its magic!
*/
     switch(action) {
        case 1:
            runit(a, b, addit);
            break;
        case 2:
            runit(a, b, subit);
            break;
        case 3:
            runit(a, b, mulit);
            break;
        case 4:
            if (b == 0)
                kill("Division by Zero\n");
            runit(a, b, divit);
            break;

        default:
            kill("Tell the programmer he sucks.\n");
    }

    return 0;
}
