#ifndef ljack_Tasker_h
#define ljack_Tasker_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ljack/inputstd.h>
#include <ljack/dbg.h>

#define AIMS 3
#define SLEN 25
#define TELL 2000

const char	*INFO_PROGRAM;
const char	PURE_ART[5][25];

const char	*MENU_FORM, *READ_FORM;
const char	*MENU_SELECT, *MENU_MANAGE, *INFO_SELECT, *INFO_SWAP, *INFO_REMOVE, *INFO_READ, *INFO_FILING, *INFO_FILED, *INFO_READ;
const char	*WARN_FORM, *WARN_LESS1, *WARN_LESS2, *WARN_FILED, *WARN_INPUT, *WARN_INPUTY, *WARN_SEEKN;
const char	*INFO_FORM, *INFO_ADD, *INFO_REDUCE, *INFO_VALIDATE, *INFO_COMPLETE, *INFO_SWAP_NODES, *INFO_SEEK_LNAME, *INFO_SEEK_TOPIC, *INFO_SEEK_TITLE, *INFO_USAGE;	
const char	*SHOW_FORM, *INFO_SHOW_ALL, *INFO_SHOW_LNAME, *INFO_SHOW_TOPIC, *INFO_SHOW_TITLE;

typedef enum IOSelect {
	ADD_TASK, MOV_TASK, RMV_TASK, GET_TASK, CMP_TASK, GET_CMP, QUIT, IEND
} IOSelect;

typedef enum LISelect {
	READ_ALL, READ_LIST, READ_TOPIC, READ_TITLE, LBACK, LEND
} LISelect;

typedef struct NodeData {
	char filed;
	unsigned short id;
	unsigned short date_created[AIMS];
	char lname[SLEN];
	char topic[SLEN];
	char title[SLEN];
	char entry[TELL];
} NodeData;

typedef struct DataBase {
	NodeData *ndata;
	unsigned short end;
	unsigned short cmp;
} DataBase;

typedef struct Connection {
	FILE *file;
	DataBase *ldata;
	char archive;
	unsigned short hits;
} Connection;

void title();

#define run_warn(M, N) (M < N)
#define run_info(F, ...) fprintf(stdout, F, ##__VA_ARGS__)

Connection *Connection_create();

char tasker_start();
char tasker_run(IOSelect *select);

char DataBase_set();
char DataBase_write(Connection *conn);
char DataBase_read(Connection *conn);

char db_data_set(Connection *conn);
char db_node_set(Connection *conn);
char db_data_get(Connection *conn);				
char db_node_get(Connection *conn);

void NodeData_copy(NodeData *to, NodeData * from, int i);
						
char node_array_create(Connection *conn);
char node_array_add(Connection *conn);
char node_array_expand(Connection *conn);
char node_array_reduce(Connection *conn);
char node_array_swap(Connection *conn);
char node_array_set_complete(Connection * conn);

char DataBase_print(Connection *conn);

void seek_all(Connection *conn);
void seek_lname(Connection *conn);
void seek_topic(Connection *conn);
void seek_title(Connection *conn);

static inline void REFRESH() {
	char rc = 0;
	rc = system("clear");
	if (rc) printf("just for getting rid of warnings");
	title();
	printf("%s", INFO_PROGRAM);
}
static inline void node_print(NodeData * cur) {
	printf(READ_FORM, cur->id, cur->date_created[0], cur->date_created[1], cur->date_created[2],
		cur->lname, cur->topic, cur->title, cur->entry);
}

static inline void prev_kill(Connection * conn) {
	if (conn->ldata->ndata) { free(conn->ldata->ndata); conn->ldata->ndata = NULL; }
}
static inline void file_kill(Connection * conn) {
	if (conn->file) { fclose(conn->file); conn->file = NULL; }
}
static inline void conn_kill(Connection * conn) {
	if (conn) {
		if (conn->ldata) { if (conn->ldata->ndata) { free(conn->ldata->ndata); conn->ldata->ndata = NULL; }
			free(conn->ldata); conn->ldata = NULL;
		}
		if (conn->file) { fclose(conn->file); conn->file = NULL; }		
		free(conn); conn = NULL;
	}
}
#endif

