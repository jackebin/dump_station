#include <ljack/dbg.h>
#include <ljack/quest.h>
#include <ljack/quests.h>

int main (int argc, char * argv[]) {

    int quiz_num = 0;
    int rc = 0;

    check(argc == 2, "\nUSAGE: quiz <quiz_number>");

    rc = system("clear");
    check(rc == 0, "Failed to clear terminal.");

    quiz_num = atoi(argv[1]);
    check(quiz_num < 3 || quiz_num > 0, "\nUSAGE: quiz <quiz number>.\n");

    switch(quiz_num)  {
        case 1:
            rc = quiz_start(DATA_TYPE_VALS, DATA_TYPE_TITL, DATA_TYPE_ANSW, DATA_TYPE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;

        case 2:
            rc = quiz_start(TYPE_SIZE_VALS, TYPE_SIZE_TITL, TYPE_SIZE_ANSW, TYPE_SIZE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;
            
        case 3:
            rc = quiz_start(TYPE_MODE_VALS, TYPE_MODE_TITL, TYPE_MODE_ANSW, TYPE_MODE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;

        default:
            sentinel("\nUSAGE: quiz <quiz number>\n");
            break;
    }

    return 0;

error:
    quiz_info(MSG, MSGN);
    return 1;
}
