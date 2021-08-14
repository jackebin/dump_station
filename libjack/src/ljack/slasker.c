#include <ljack/tasker.h>
//	"strings and such". Yes, It's an utter mess.
const char PURE_ART[5][25] = {

    {'#','#','#','#','#',' ','#','#','#',' ','#','#','#',' ','#',' ','#',' ','#','#','#',' ','#','#','#'},
    {' ',' ','#',' ',' ',' ','#',' ','#',' ','#',' ',' ',' ','#','#',' ',' ','#',' ',' ',' ','#',' ','#'},
    {' ',' ','#',' ',' ',' ','#','#','#',' ','#','#','#',' ','#',' ',' ',' ','#','#',' ',' ','#','#','#'}, 
    {' ',' ','#',' ',' ',' ','#',' ','#',' ',' ',' ','#',' ','#','#',' ',' ','#',' ',' ',' ','#','#',' '},
    {' ',' ','#',' ',' ',' ','#',' ','#',' ','#','#','#',' ','#',' ','#',' ','#','#','#',' ','#',' ','#'}
};

const char *INFO_PROGRAM	= "\nthis is taskhandler or something. Basically it's all about adding, removing and moving around\n"\
							  "tasks to your heart's content instead of actually complete them.\n\n\033[2;7;1mnote! when writing strings in "\
							  "in \"Tasker\", type \"$\" before hitting enter when done.\033[2;7;0m\n";

const char *MENU_FORM 		= "\n\033[1m%s\033[0m\n%s\n";

const char *MENU_SELECT		= "SELECT\t1) Add Task\t2) Swap Tasks\t\t3) Remove Task\t\t4) Read active Task(s)";
const char *INFO_SELECT		= "\tenter number\t\033[1m5) Complete Task\033[0m\t\033[1m6) Completed Tasks\033[0m\t\033[33m7) Exit Tasker\033[0m";

const char *MENU_MANAGE		= "DISPLAY\t1) All\t\t2) By List\t\t3) By Topic\t\t4) By Title";

const char *INFO_ADD		= "\033[2;7;1mAdd\033[2;7;0m\tcreate a new note/task by enter listname, topic, title and an entry.\n\n";
const char *INFO_SWAP		= "\033[2;7;1mSwap\033[2;7;0m\tsearch or display all tasks before swapping.\t\t\t\033[33m5) Back\033[0m\n";
const char *INFO_REMOVE		= "\033[2;7;1mRemove\033[2;7;0m\tsearch or display all tasks before removing.\t\t\t\033[33m5) Back\033[0m\n";
const char *INFO_READ		= "\033[2;7;1mRead\033[2;7;0m\tread active tasks.\t\t\t\t\t\t\033[33m5) Back\033[0m\n";
const char *INFO_FILING		= "\033[2;7;1mFiling\033[2;7;0m\tread active tasks before mark as completed.\t\t\t\033[33m5) Back\033[0m\n";
const char *INFO_FILED		= "\033[2;7;1mFiled\033[2;7;0m\tsearch or read all completed tasks.\t\t\t\t\033[33m5) Back\033[0m\n";

const char *WARN_FORM		= "\033[33m\t%s\033[0m\n";

const char *WARN_LESS1		= "database empty.";
const char *WARN_LESS2		= "nodeswaps requires at least two tasks in database.";
const char *WARN_FILED		= "no tasks completed yet, you rockstar!";
const char *WARN_SEEKN		= "no matches found";
const char *WARN_INPUT		= "\n\033[33minvalid input, try again.\033[0m";
const char *WARN_INPUTY		= "\n\t\033[33minvalid input, try again.\033[0m\n";
const char *WARN_ID			= "\033[33mid doesn't exist, try again.\033[0m\n";

const char *INFO_FORM		= "\033[33m%s\033[0m\n";

const char *INFO_REDUCE     = "remove task by enter its id.";
const char *INFO_VALIDATE	= "proceed\t1) yes 2) no";
const char *INFO_COMPLETE	= "enter id to declare task as completed.";
const char *INFO_SWAP_NODES = "swap tasks by id:s.";
const char *INFO_SEEK_LNAME = "enter search phrase for listname.";
const char *INFO_SEEK_TOPIC = "enter search phrase for topic.";
const char *INFO_SEEK_TITLE = "enter search phrase for title.";

const char *SHOW_FORM		= "\n\033[1m%s \"%s\"\033[0m\n\n";

const char *INFO_SHOW_ALL	= "Active Tasks in database";
const char *INFO_SHOW_LNAME = "Lists found";
const char *INFO_SHOW_TOPIC = "Topics found";
const char *INFO_SHOW_TITLE = "Titles found";		

const char *READ_FORM 		= "\033[2;7;1mID %d\033[2;7;0m  \033[33mDate Created \033[0m%d-%d-%d "\
							  "\033[33mList \033[0m%s \033[33mTopic \033[0m%s \033[33mTitle \033[0m%s\n\n%s\n\n";
