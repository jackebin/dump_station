#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glob.h>
#include <ctype.h>
#include "dbg.h"

#define MAX_DATA 512
#define MAX_ROWS 100

struct Entry {
    int    id;
    int    set;
    double my_time;
    double my_distance;
    char   my_date[MAX_DATA];
    char   my_run[MAX_DATA];
};

struct Journal {
    int    run;
    int    id_arr[MAX_ROWS];
    double time_arr[MAX_ROWS];
    double dist_arr[MAX_ROWS];
    double net_arr[MAX_ROWS];
    double rat_arr[MAX_ROWS];
    struct Entry entry[MAX_ROWS];
};

struct Connect {
    char * filename;
    FILE * file;
    struct Journal * journal;
};

struct tm * loctime;
struct Connect * journal_open(char * mode);

void info_menu(void);
void info_create(void);
void info_files(void);
void info_add(struct Connect * conn);
void info_list(struct Connect * conn);
void info_date(struct Connect * conn);
void info_name(struct Connect * conn);
void info_distance(struct Connect * conn);
void info_performance(struct Connect * conn);

int  listing_search_name(struct Connect * conn);
int  listing_search_date(struct Connect * conn);
int  listing_search_performance(struct Connect * conn);
int  listing_search_distance(struct Connect * conn);
int  listing_search_all(struct Connect * conn);

int  read_string(char ** out_string);
int  read_time(char ** out_string);
int  remove_nl(char * out_string);
int  read_char(char * out_char);
int  bubble_it(struct Journal * journal, char mode);

int  journal_list(struct Connect * conn);
int  journal_enter(struct Connect * conn);
int  journal_create(struct Connect * conn);
int  journal_write(struct Connect * conn);
int  journal_load(struct Connect * conn);
int  journal_add(struct Connect * conn);
int  journal_calc(struct Journal * journal);
int  journal_print(struct Entry * ent, struct Journal * journal, int i);

void journal_destroy(struct Connect * conn);

//  switch statement for evaluating desired task.

int
main(int argc, char * argv[])
{
    system("clear");
    info_menu();

    char mode = ' ';
    int rc = 0;

    struct Connect * conn = journal_open(&mode);
    check(conn, "Failed to open Journal.");

    switch(mode) {

        case 'a':
            rc = journal_add(conn);
            check(rc == 0, "Failed to add entry.");

            rc = journal_write(conn);
            check(rc == 0, "Failed to write Journal.")
            break;

        case 'l':
            rc = journal_list(conn);
            check(rc == 0, "Failed to print Journal.");
            break;

        case 'c':
            rc = journal_create(conn);
            check(rc == 0, "Failed to create Journal.");

            rc = journal_write(conn);
            check(rc == 0, "Failed to write Journal.");
            break;

        case 'e':
            printf("Exiting.\n");
            break;

        default:
            sentinel("Wrong input.");
            break;
    }

    journal_destroy(conn);
    return 0;

error:
    journal_destroy(conn);
    return -1;
}

//  start function returning a conn-pointer used for upcoming task(s).

struct
Connect * journal_open(char * mode)
{
    int rc = 0;
    char * res = NULL;

    struct Connect * conn = malloc(sizeof(struct Connect));
    check_mem(conn);

    conn->journal = malloc(sizeof(struct Journal));
    check_mem(conn->journal);

    rc = read_char(mode);
    check(rc == 0, "Input error mode.");

    if (*mode == 'e')
        return conn;

    if (*mode == 'c') {
        info_create();

        rc = read_string(&conn->filename);
        check(rc == 0, "Failed to read filename.");

        res = strcat(conn->filename, ".dat");
        check(res != NULL, "Failed to cat .dat");

        conn->file = fopen(conn->filename, "w");
        check(conn->file, "Failed to open file.");

    } else {
        conn->filename = calloc(MAX_DATA, 1);
        rc = journal_enter(conn);
        check(rc == 0, "Failed globbing.");

        conn->file = fopen(conn->filename, "r+");
        check(conn->file, "Failed to open file.");

        journal_load(conn);
    }

    return conn;

error:
    journal_destroy(conn);
    return NULL;
}

//  switch statement for evaluating desired data output.

int
journal_list(struct Connect * conn)
{
    system("clear");
    info_list(conn);

    int rc = 0;
    char list = ' ';

    rc = read_char(&list);
    check(rc == 0, "Failed to read char.");
    printf("\n");

    switch(list) {
        case 'n':
            rc = listing_search_name(conn);
            check(rc == 0, "Failed to print names.");
            break;

        case 'd':
            rc = listing_search_date(conn);
            check(rc == 0, "Failed to print dates.");
            break;

        case 'p':
            rc = listing_search_performance(conn);
            check(rc == 0, "Failed to print dates.");
            break;

        case 'i':
            rc = listing_search_distance(conn);
            check(rc == 0, "Failed to print dates.");
            break;

        case 'a':
            rc = listing_search_all(conn);
            check(rc == 0, "Failed to print all.");
            break;

       default:
           sentinel("Failed to load any search.");
           break;

    }
    return 0;

error:
    journal_destroy(conn);
    return -1;
}

/*
    my first ever glob encounter. Users choose among
    existing journals (if there are any) by index numbers.
*/

int
journal_enter(struct Connect * conn)
{
    system("clear");
    info_files();

    int idx = 0, val = 0, rc = 0;
    char * temp_idx = NULL;
    char * res = NULL;

    glob_t paths;
    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    rc = glob("*.dat", GLOB_NOCHECK | GLOB_NOSORT, NULL, &paths);
    check(rc == 0, "Failed to glob.");

    for(idx = 0; idx < paths.gl_pathc; idx++)
        printf("[%d]: %s\n", idx+1, paths.gl_pathv[idx]);
    printf("\nIndex number: ");

    rc = read_string(&temp_idx);
    check(rc == 0, "Failed input.");
    val = atoi(temp_idx);
    check(val >= 1 || val <= idx+1, "Only digits allowed mapped to existing journals.");

    res = strcpy(conn->filename, paths.gl_pathv[val-1]);
    check(res != NULL, "Failed to copy pathway.");

    free(temp_idx);
    globfree(&paths);
    return 0;

error:
    journal_destroy(conn);
    globfree(&paths);
    free(temp_idx);
    return -1;
}

/*
    creating a new journal with max amount of rows (entries).
    this nglobal going to be arbitrary - a choice made by the user - 
    when I've played around some more with this program.
*/

int
journal_create(struct Connect * conn)
{
    int i = 0;
    for (i = 0; i < MAX_ROWS; i++) {
        struct Entry ent = {.id = i,.set = 0 };
        conn->journal->entry[i] = ent;
    }

    return 0;
}

//  write a new journal or overwrite an old one with new data.

int
journal_write(struct Connect * conn)
{
    int rc = 0;
    rewind(conn->file);

    rc = fwrite(conn->journal, sizeof(struct Journal), 1, conn->file);
    check (rc == 1, "Failed to write Journal.");

    rc = fflush(conn->file);
    check(rc >= 0, "Failed to flush Journal.");

    return 0;

error:
    journal_destroy(conn);
    return -1;
}

// loading old journals.

int
journal_load(struct Connect * conn)
{
    int rc = 0;
    rc = fread(conn->journal, sizeof(struct Journal), 1, conn->file);
    check(rc == 1, "Failed to read file");

    return 0;

error:
    journal_destroy(conn);
    return -1;
}

//  Rather goofy func dealing with user inputs to be stored in the Journal. 

int
journal_add(struct Connect * conn)
{
    system("clear");
    info_add(conn);

    int rc = 0, count = 0;
    char * input_date = NULL;
    char * input_run = NULL;
    char * input_time = NULL;
    char * input_dist = NULL;
    char * res = NULL;

    // finds the next row not taken.

    while (count < MAX_ROWS) {
        struct Entry * ent = &conn->journal->entry[count];
        if (ent->set == 0)
            break;
        count++;
    }

    struct Entry * ent = &conn->journal->entry[count];
    ent->set = 1;

    rc = read_time(&input_date);
    check(rc == 0, "Failed input");

    printf("Name your run: ");
    rc = read_string(&input_run);
    check(rc == 0, "Failed input.");

    printf("Time: ");
    rc = read_string(&input_time);
    check(rc == 0 && isdigit(*input_time), "Failed input, using letters?");

    printf("Distance: ");
    rc = read_string(&input_dist);
    check(rc == 0 && isdigit(*input_dist), "Failed input, using letters?");

    res = strncpy(ent->my_date, input_date, MAX_DATA);
    check(res != NULL, "Failed to copy time.")

    res = strncpy(ent->my_run, input_run, MAX_DATA);
    check(res != NULL, "Failed to copy course.");

    ent->my_time = atof(input_time);
    ent->my_distance = atof(input_dist);

    free(input_date);
    free(input_run);
    free(input_time);
    free(input_dist);

    return 0;

error:
    if(input_date) free(input_date);
    if(input_run)  free(input_run);
    if(input_time) free(input_time);
    if(input_dist) free(input_dist);

    journal_destroy(conn);
    return -1;
}

/*
    buffering historical records used for calculations 
    before printing desired data out.
*/

int
journal_calc(struct Journal * journal)
{
    int i = 0, run = 0;
    for (i = 0; i < MAX_ROWS; i++) {

        struct Entry *ent = &journal->entry[i];

        if (ent->set) {
            journal->id_arr[i] = ent->id;
            journal->time_arr[i] = ent->my_time;
            journal->dist_arr[i] = ent->my_distance;
            run++;
        }
    }

    journal->run = run;
    journal->net_arr[0] = 0;

    for (i = 1; i < journal->run; i++)
        journal->net_arr[i] = (journal->time_arr[i-1] - journal->time_arr[i]);
    for (i = 0; i < journal->run; i++)
        journal->rat_arr[i] = (journal->time_arr[i] / journal->dist_arr[i]);

    return 0;
}

/*
    a simple bubble at work before printing 
    runs in descending order by distance or pace.
*/

int
bubble_it(struct Journal * journal, char mode)
{
    int i = 0, j = 0, nswap = 0;
    float fswap = 0.0;

    if (mode == 'r') {
        for (i = 0; i < journal->run; i++) {
            for (j = i + 1; j < journal->run; j++) {
                if (journal->rat_arr[i] < journal->rat_arr[j]) {

                    fswap = journal->rat_arr[i];
                    journal->rat_arr[i] = journal->rat_arr[j];
                    journal->rat_arr[j] = fswap;

                    nswap = journal->id_arr[i];
                    journal->id_arr[i] = journal->id_arr[j];
                    journal->id_arr[j] = nswap;

                }
            }
        }
    }
    if (mode == 'd') {
        for (i = 0; i < journal->run; i++) {
            for (j = i + 1; j < journal->run; j++) {
                if (journal->dist_arr[i] > journal->dist_arr[j]) {

                    fswap = journal->dist_arr[i];
                    journal->dist_arr[i] = journal->dist_arr[j];
                    journal->dist_arr[j] = fswap;

                    nswap = journal->id_arr[i];
                    journal->id_arr[i] = journal->id_arr[j];
                    journal->id_arr[j] = nswap;

                }
            }
        }

    }
    return 0;
}

//  quite unstable and naive nl remover atm after fgets usage.

int
remove_nl(char * out_string)
{
    int i = 0;
    for (i = 0; i < MAX_DATA; i++) {
        if (out_string[i] == '\n') {
            out_string[i] = '\0';
            break;
        }
    }
    return 0;
}

/*
    basic inputs. Less prone to break compared 
    with my earlier experiments but yes, it's still a
    naive approach.
*/

int
read_string(char ** out_string)
{
    int rc = 0;

    *out_string = calloc(MAX_DATA, 1);
    char * res = fgets(*out_string, MAX_DATA, stdin);
    check(res != NULL, "Failed input.");

    rc = remove_nl(*out_string);
    check(rc == 0, "Failed remove nl.");

    return 0;

error:
    if(*out_string) free(*out_string);
    return -1;
}

/*
    func for single chars; letters or numbers. To recieve
    the latter the function follows up by atoi or atof atm, something
    I want to be dealt with inside the function itself in time using
    stdarg.
*/

int
read_char(char * out_char)
{
    int rc = 0;
    char * temp_char = NULL;
    rc = read_string(&temp_char);
    check(rc == 0, "Failed make char.");
    *out_char = temp_char[0];
    free(temp_char);
    return 0;

error:
    return -1;
}

//  Fetching current date and time.

int
read_time(char ** out_string)
{
    *out_string = calloc(MAX_DATA, 1);
    check(*out_string != NULL, "Memory Error reading time.");

    struct tm *xyz;
    time_t l;
    l = time(NULL);
    xyz = localtime(&l);

    strncpy(*out_string, asctime(xyz), MAX_DATA);

    return 0;

error:
    return -1;
}

// lists and prints runs by distance.

int
listing_search_distance(struct Connect * conn)
{
    system("clear");
    info_distance(conn);

    int rc = 0, i = 0, found = 0, map = 0;
    struct Journal * journal = conn->journal;

    rc = journal_calc(journal);
    check(rc == 0, "Failed to calculate runs.");

    rc = bubble_it(journal, 'd');
    check(rc == 0, "Failed to bubble it.");

    printf("\nRUN \t\t RESULTS\n\n");
    for (i = journal->run-1; i >= 0; i--) {
        map = journal->id_arr[i];
        struct Entry * ent = &journal->entry[map];

        found++;
        rc = journal_print(ent, journal, map);
    }

    check(rc == 0, "Listing error.");
    if (found == 0)
        printf("Sorry, No match.\n");

    return 0;

error:
    journal_destroy(conn);
    return -1;
}

// lists and prints runs by pace.

int
listing_search_performance(struct Connect * conn)
{
    system("clear");
    info_performance(conn);

    int rc = 0, i = 0, found = 0, map = 0;
    struct Journal * journal = conn->journal;

    rc = journal_calc(journal);
    check(rc == 0, "Failed to calculate runs.");

    rc = bubble_it(journal, 'r');
    check(rc == 0, "Failed to bubble it.");

    printf("\nRUN \t\t RESULTS\n\n");
    for (i = journal->run-1; i >= 0; i--) {
        map = journal->id_arr[i];
        struct Entry * ent = &journal->entry[map];

        found++;
        rc = journal_print(ent, journal, map);
    }

    check(rc == 0, "Listing error.");
    if (found == 0)
        printf("Sorry, No match.\n");

    return 0;

error:
    journal_destroy(conn);
    return -1;
}

// search and prints runs by name.

int
listing_search_name(struct Connect * conn)
{
    system("clear");
    info_name(conn);

    int rc = 0, i = 0, found = 0;
    char * search = NULL;
    char * res = NULL;

    struct Journal * journal = conn->journal;

    rc = journal_calc(journal);
    check(rc == 0, "Failed to calculate runs.");

    rc = read_string(&search);
    check(rc == 0, "Failed search date.");

    printf("\nRUN \t\t RESULTS\n\n");
    for (i = journal->run-1; i >= 0; i--) {
        struct Entry * ent = &journal->entry[i];
        if ((res = strstr(ent->my_run, search)) != NULL) {
            found++;
            rc = journal_print(ent, journal, i);
        }
    }

    check(rc == 0, "Listing error.");
    if (found == 0)
        printf("Sorry, No match.\n");

    free(search);
    return 0;

error:
    if(search) free(search);
    journal_destroy(conn);
    return -1;
}

// search and prints runs by date.

int
listing_search_date(struct Connect * conn)
{
    system("clear");
    info_date(conn);

    int rc = 0, i = 0, found = 0;
    char * search = NULL;
    char * res = NULL;

    struct Journal * journal = conn->journal;
    rc = journal_calc(journal);
    check(rc == 0, "Failed to calculate runs.");

    rc = read_string(&search);
    check(rc == 0, "Failed search date.");

    printf("\nRUN \t\t INFO\n\n");
    for (i = journal->run-1; i >= 0; i--) {
        struct Entry * ent = &journal->entry[i];
        if ((res =strstr(ent->my_date, search)) != NULL) {
            found++;
            rc = journal_print(ent, journal, i);
        }
    }

    check(rc == 0, "Listing error.");
    if (found == 0)
        printf("Sorry, No match.\n");

    free(search);
    return 0;

error:
    if(search) free(search);
    journal_destroy(conn);
    return -1;
}

// prints all records.

int
listing_search_all(struct Connect * conn)
{
    system("clear");
    printf("%s | LIST | ALL\n", conn->filename);

    int rc = 0, i = 0, found = 0;

    struct Journal * journal = conn->journal;
    rc = journal_calc(journal);
    check(rc == 0, "Failed to calculate runs.");

    printf("\nRUN \t\t RESULTS\n\n");
    for (i = journal->run-1; i >= 0; i--) {
        struct Entry * ent = &journal->entry[i];
        found++;
        rc = journal_print(ent, journal, i);
    }

    check(rc == 0, "Listing error.");
    if (found == 0)
        printf("Database empty.\n");

    return 0;

error:
    journal_destroy(conn);
    return -1;
}

void
journal_destroy(struct Connect * conn)
{
    if (conn) {
        if (conn->file)
            fclose(conn->file);
        if (conn->journal)
            free(conn->journal);
        if (conn->filename)
            free(conn->filename);
        free(conn);
    }
}

//  "GUI" from here.

int
journal_print(struct Entry * ent, struct Journal * journal, int i)
{
    printf(
        "Name: \t\t %s\n"
        "Date: \t\t %s"
        "Distance: \t %2.2lf km\n"
        "Time: \t\t %2.2lf min\n"
        "Average Pace: \t %2.2lf min/km\n\n",
        ent->my_run, ent->my_date,
        ent->my_distance, ent->my_time,
        journal->rat_arr[i]
    );

    return 0;
}

void
info_menu(void)
{
    printf(
        "JOURNAL | MENU\n\n"
        "Type lowercase letter and press enter\n"
        "to proceed throughout the program.\n\n"
        "ADD RUN...............(a)\n"
        "LIST RUN(s)...........(l)\n"
        "CREATE NEW JOURNAL....(c)\n"
        "EXIT JOURNAL..........(e)\n\n"
        "Choose: "
    	);
}

void
info_create(void)
{
    printf(
        "JOURNAL | CREATE JOURNAL\n\n"
        "Name your Journal.\n\n"
        "Name: "
    );
}

void
info_files(void)
{
    printf(
        "JOURNAL | OPEN\n\n"
        "List of created Journals below.\n"
        "Type number of matching index to open.\n\n"
    );
}

void
info_add(struct Connect * conn)
{
    printf("%s | ADD RUN\n\n", conn->filename);
    printf(
        "All runs are given a new or an old name.\n"
        "Distance in kilometers, time in minutes.\n"
        "Mark decimals with period.\n\n"
    );
}

void
info_list(struct Connect * conn)
{
    printf("%s | LIST RUN(S)\n\n", conn->filename);
    printf(
        "LIST BY NAME..........(n)\n"
        "LIST BY DATE..........(d)\n\n"
        "LIST BY DISTANCE......(i)\n"
        "LIST BY PERFORMANCE...(p)\n"
        "LIST ALL..............(a)\n\n"
        "Choose: "
    );
}

void
info_date(struct Connect * conn)
{
    printf("%s | LIST | SEARCH BY DATE\n\n", conn->filename);
    printf(
        "Search by either Weekday, Month,\n"
        "Day and Year in any combination.\n"
        "Format Example:\n"
        "Weekday:\"Mon\"\n"
        "Month:  \"Apr\"\n"
        "Day:    \"1\"\n"
        "Year:   \"Mon\"\n\n"
        "Date: "
    );
}

void
info_name(struct Connect * conn)
{
    printf("%s | LIST | SEARCH BY NAME\n\n", conn->filename);
    printf(
        "Enter a search phrase. The program\n"
        "will list all matching records.\n\n"
        "Phrase: "
    );
}

void
info_performance(struct Connect * conn)
{
    printf("%s | LIST | PERFORMANCES\n\n", conn->filename);
    printf(
        "Best runs by pace in descending order.\n"
    );
}

void
info_distance(struct Connect * conn)
{
    printf("%s | LIST | DISTANCES\n\n", conn->filename);
    printf(
        "Longest distances in descending order.\n"
    );
}
