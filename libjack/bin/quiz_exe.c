#include <ljack/dbg.h>
#include <ljack/quest.h>
#include <ljack/quests.h>
#include <dlfcn.h>

typedef int (*lib_function) (const int * VALS, const char * TITL, const char ** ANSW, const char ** QUIZ);

int main (int argc, char * argv[]) {

    int quiz_num = 0;
    int rc = 0;

    rc = system("clear");
    check(rc == 0, "Failed to clear terminal.");
    check(argc == 2, "\nUSAGE: quiz <quiz_number>");

    quiz_num = atoi(argv[1]);
    check(quiz_num < 3 || quiz_num > 0, "\nUSAGE: quiz <quiz number>.\n");

    char * lib_file = "../build/libjack.so";
    char * func_to_run = "quiz_start";

    void * lib = dlopen(lib_file, RTLD_NOW);
    check(lib != NULL, "Failed to open library %s: %s", lib_file, dlerror());

    lib_function func = dlsym(lib, func_to_run);
    check(func != NULL, "Didn't find function %s in library %s: %s", func_to_run, lib_file, dlerror());

    switch(quiz_num)  {
        case 1:
            rc = func(DATA_TYPE_VALS, DATA_TYPE_TITL, DATA_TYPE_ANSW, DATA_TYPE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;

        case 2:
            rc = func(TYPE_SIZE_VALS, TYPE_SIZE_TITL, TYPE_SIZE_ANSW, TYPE_SIZE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;

       case 3:
            rc = func(TYPE_MODE_VALS, TYPE_MODE_TITL, TYPE_MODE_ANSW, TYPE_MODE_QUIZ);
            check(rc == 0, "Error while loading quiz data.");
            break;

       default:
            sentinel("\nUSAGE: quiz <quiz number>\n");
            break;
    }

    rc = dlclose(lib);
    check(rc == 0, "Failed to close lib %s", lib_file);

    return 0;

error:
    quiz_info(MSG, MSGN);
    return 1;
}
