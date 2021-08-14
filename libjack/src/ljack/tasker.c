#include <ljack/tasker.h>
																										/*
	back to school!
	
	I made a "taskmanager" of some sort for managing all tasks and appointments during upcoming studies.
	it started out as a doubly linked list but ended up as a "dynamic array" because I didn't managed to
	wove the linked list properly with fwrite/fread :/ However - it was a fun, instructive little project!

	the core logic throughout the program is to copy the original data to a buffer, kill original, then
	rebuild it with the buffer as blueprint (with or without changes) before rewriting files. this repeats
	over and again until the user quits or everything crashes. the program is actually kinda stable though.
	
	Coded most on the fly why I'm from now on definitely going to PLAN my projects better before I begin but
	even though I kill and rebuild connections all the time in a weird manner I kinda like it, the feeling of
	everything being get, set, written and killed before "starting over". At least Valgrind doesn't complain.
	
	most noobish solution among others might be storing data in two files. long story short - amateurism,
	and these files are still hard coded. Mark Tasks as completed works but barely userfriendly and quite
	prone to bugs because of how id:s being updated at the moment but at least it's a start.
																										*/
char tasker_start() {
//	..or "main" - relic-func used in unit testing.
	char rc = 0;
	REFRESH();
	
	rc = DataBase_set();
	check(rc == 0, "failed set database");

	while (1) {
		
		run_info(MENU_FORM, MENU_SELECT, INFO_SELECT);
		IOSelect select = (int)std_numnwrap(ADD_TASK, IEND, WARN_INPUTY) - 1;
		
		if (select == QUIT) { break; }
		
		rc = tasker_run(&select);
		check(rc == 0, "failed while running session");
	}
	return 0;
error:
	return 1;
}

Connection *Connection_create() {

	Connection * conn = calloc(1, sizeof(Connection));
	check(conn != NULL, "failed to calloc conn");
	
	conn->ldata = calloc(1, sizeof(DataBase));
	check(conn->ldata != NULL, "failed to calloc ldata");
	
	return conn;
error:
	return NULL;
}

char tasker_run(IOSelect * select) {
//	reads data from db-files, alternates it and rewrites a modified version in the end.
	char rc = 0;
	
	Connection *conn = Connection_create();
	check(conn != NULL, "failed to create connection");
	
	rc = DataBase_read(conn);
	check(rc == 0, "failed to read database");

	switch(*select) {	
			
		case ADD_TASK:
			run_info("%s", INFO_ADD);	
			
			rc = node_array_expand(conn);
			check(rc == 0, "failed to resize node array");
			
			rc = node_array_add(conn);
			check(rc == 0, "failed to add node");
			break;	
	
		case MOV_TASK:
			if(run_warn(conn->ldata->end, 3)) { run_info(WARN_FORM, WARN_LESS2); break; }
			run_info(MENU_FORM, MENU_MANAGE, INFO_SWAP);			
			
			rc = DataBase_print(conn);	
			if (rc == - 1 || conn->hits < 2) { break; }
			
			rc = node_array_swap(conn);
			check(rc == 0, "failed to swap nodes");
			break;
			
		case RMV_TASK:
			if (run_warn(conn->ldata->end, 2)) { run_info(WARN_FORM, WARN_LESS1); break; }
			run_info(MENU_FORM, MENU_MANAGE, INFO_REMOVE);
			
			rc = DataBase_print(conn);
			if (rc == - 1 || conn->hits < 1) { break; }
			
			rc = node_array_reduce(conn);
			check(rc == 0, "failed remove record");
			break;

		case GET_TASK:
			if (run_warn(conn->ldata->end, 2)) { run_info(WARN_FORM, WARN_LESS1); break; }
			run_info(MENU_FORM, MENU_MANAGE, INFO_READ);
			DataBase_print(conn);
			break;

		case CMP_TASK:
			if (run_warn(conn->ldata->end, 2)) { run_info(WARN_FORM, WARN_LESS1); break; }
			run_info(MENU_FORM, MENU_MANAGE, INFO_FILING);

			rc = DataBase_print(conn);
			if (rc == - 1 || conn->hits < 1) { break; }

			rc = node_array_set_complete(conn);
			check(rc == 0, "failed to file task");
			break;
			
		case GET_CMP:
			if (run_warn(conn->ldata->cmp, 1)) { run_info(WARN_FORM, WARN_FILED); break; }
			run_info(MENU_FORM, MENU_MANAGE, INFO_FILED);
			
			conn->archive++;

			rc = DataBase_print(conn);
			if (rc == - 1 || conn->hits < 1) { break; }

			break;

		default:
			run_info(WARN_FORM, WARN_INPUT);
	}
	rc = DataBase_write(conn);
	check(rc == 0, "failed to update database");	
	
	conn_kill(conn);
	return 0;
error:
	conn_kill(conn);
	return 1;
}

char DataBase_set() {
//	sets (if new) database.
	char rc = 0;
	const char *database = "database.dat";	
	Connection *conn = Connection_create();
	check(conn != NULL, "failed to create connection");
	
	conn->file = fopen(database, "r");
	if (!conn->file) {
		rc = node_array_create(conn);
		check(rc == 0, "failed to set array");
		rc = DataBase_write(conn);
		check(rc == 0, "failed to set database");
	}
	conn_kill(conn);
	return 0;
error:
	conn_kill(conn);
	return 1;
}

char DataBase_write(Connection * conn) {
//	writing data to files.
	char rc = 0;

	rc = db_data_set(conn);
	check(rc == 0, "failed to write ldata");
	rc = db_node_set(conn);
	check(rc == 0, "failed to write ndata");
	
	return 0;
error:
	return 1;
}

char DataBase_read(Connection * conn) {
//	reading data from files.
	char rc = 0;
	
	rc = db_data_get(conn);
	check(rc == 0, "failed to read ldata");
	rc = db_node_get(conn);
	check(rc == 0, "failed to read ndata");
	
	return 0;
error:
	return 1;
}

char db_data_set(Connection * conn) {											//
//	writing database.
	const char *database = "database.dat";	
	short rc = 0;

	conn->file = fopen(database, "w");
	check(conn->file != NULL, "failed to open file");
	
	rewind(conn->file);
	
	rc = fwrite(conn->ldata, sizeof(DataBase), 1, conn->file);
	check(rc == 1, "failed to write file");
	
	rc = fflush(conn->file);
	check(rc == 0, "failed to flush");
	
	file_kill(conn);
	return 0;
error:
	file_kill(conn);
	return 1;
}

char db_node_set(Connection * conn) {
//	writing nodedata (separate file).
	const char *nodebase = "nodebase.dat";	
	short rc = 0;

	conn->file = fopen(nodebase, "w");
	check(conn->file != NULL, "failed to open file");

	rewind(conn->file);
	
	rc = fwrite(conn->ldata->ndata, sizeof(NodeData),
		conn->ldata->end, conn->file);
	check(rc == conn->ldata->end, "failed to write file");

	rc = fflush(conn->file);
	check(rc == 0, "failed to flush");
	
	file_kill(conn);
	return 0;
error:
	file_kill(conn);
	return 1;
}

char db_data_get(Connection * conn) {												//
//	reading database.
	const char *database = "database.dat";
	short rc = 0;

	conn->file = fopen(database, "r");
	check(conn->file, "failed to open file");
	
	rc = fread(conn->ldata, sizeof(DataBase), 1, conn->file);
	check(rc == 1, "failed to read file");
	
	file_kill(conn);
	return 0;
error:
	file_kill(conn);
	return 1;
}

char db_node_get(Connection * conn) {
//	reading nodedata (separate file).
	const char *nodebase = "nodebase.dat";	
	short rc = 0;

	conn->ldata->ndata = calloc(conn->ldata->end, sizeof(NodeData));
	check(conn->ldata->ndata != NULL, "failed to create ndata");
	
	conn->file = fopen(nodebase, "r");
	check(conn->file != NULL, "failed to open file");
	
	rc = fread(conn->ldata->ndata, sizeof(NodeData),
		conn->ldata->end, conn->file);
	check(rc == conn->ldata->end, "failed to read file");

	file_kill(conn);
	return 0;
error:
	file_kill(conn);
	return 1;
}

void NodeData_copy(NodeData * to, NodeData * from, int i) {
//	copy node. used inside for-loops.
	to->id = i;
	to->filed = from->filed;
	to->date_created[0] = from->date_created[0];
	to->date_created[1] = from->date_created[1];
	to->date_created[2] = from->date_created[2];

	for(i = 0; i < SLEN; i++) {
		to->lname[i] = from->lname[i];
		to->topic[i] = from->topic[i];
		to->title[i] = from->title[i];
	}
	for(i = 0; i < TELL; i++)
		to->entry[i] = from->entry[i];
}

char node_array_create(Connection * conn) {											
//	sets (first member) in an array of nodes.
	conn->ldata->end = 1;
	
	conn->ldata->ndata = calloc(conn->ldata->end, sizeof(NodeData));
	check(conn->ldata->ndata != NULL, "failed to alloc memory");
			
	NodeData ndata = { .filed = 0 };
	conn->ldata->ndata[conn->ldata->end - 1] = ndata;
	
	return 0;
error:
	return 1;
}

char node_array_add(Connection * conn) {
//	adding nodedata.	
	char rc = 0;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	check(conn->ldata->end < 65535, "max nodes reached");
	
	NodeData *new = &conn->ldata->ndata[conn->ldata->end - 1];
	check(new != NULL, "failed to create new node");
	new->id = conn->ldata->end - 1;
	
	new->date_created[0] = tm.tm_year + 1900;
	new->date_created[1] = tm.tm_mon + 1;
	new->date_created[2] = tm.tm_mday;

	printf("lname>\t");
	rc = std_strwrap(new->lname);
	check(rc == 0, "failed to set lname");
	
	printf("topic>\t");
	rc = std_strwrap(new->topic);
	check(rc == 0, "failed to set topic");
	
	printf("title>\t");
	rc = std_strwrap(new->title);
	check(rc == 0, "failed to set title");
	
	printf("\ndescribe task\n\n");
	rc = std_strwrap(new->entry);
	check(rc == 0, "failed to set entry");			

	conn->ldata->end++;

	return 0;
error:
	return 1;
}

char node_array_expand(Connection * conn) {
//	adding memory (for a new node).
	unsigned short i = 0;
	unsigned short new_size = conn->ldata->end + 1;
	unsigned short old_size = conn->ldata->end;

	NodeData *temp_buffer = calloc(new_size, sizeof(NodeData));
	check(temp_buffer != NULL, "failed to create temp_buffer");
	// copy old data.
	for (i = 0; i < old_size; i++) {
		NodeData *temp = &temp_buffer[i];
		NodeData *from = &conn->ldata->ndata[i];
		NodeData_copy(temp, from, i);
	}
	// kill old, rebuild++.
	prev_kill(conn);
	conn->ldata->ndata = calloc(new_size, sizeof(NodeData));
	check(conn->ldata->ndata != NULL, "failed to resize conn");
	// copy modified array in temp "back"++.
	for (i = 0; i < new_size; i++) {
		NodeData *temp = &temp_buffer[i];
		NodeData *update = &conn->ldata->ndata[i];
		NodeData_copy(update, temp, i);
	}
	free(temp_buffer);
	temp_buffer = NULL;
	return 0;
error:
	if(temp_buffer) free(temp_buffer);
	temp_buffer = NULL;	
	return 1;
}

char node_array_reduce(Connection * conn) {
// 	pops a given node before reducing the array.
	char val = 0;
	unsigned short i = 0, id = 0, jump = 0, rmv = 0;
	unsigned short arr_size = conn->ldata->end;

	run_info(INFO_FORM, INFO_REDUCE);
	printf("id>\t");
	id = std_numnwrap(0, arr_size - 2,
			"\t\033[33mid doesn't exist.\033[0m\nid>\t");
			
	run_info(INFO_FORM, INFO_VALIDATE);
	printf("tell>\t");
	val = std_validate("\033[33mwrong input, try again\033[0m\ntell>\t");
	if (val == 1) return 0;
	
	NodeData temp_buffer[arr_size];
	
	for(i = 0; i < arr_size; i++) {
		NodeData new = { .id = i };
		temp_buffer[i] = new;
	}
	// copy old data.	
	for (i = 0; i < arr_size; i++) {
		NodeData *from = &conn->ldata->ndata[i];	
		NodeData *to = &temp_buffer[i];
		NodeData_copy(to, from, i);
	}
	// kill old, rebuild.	
	prev_kill(conn);
	conn->ldata->ndata = calloc(arr_size, sizeof(NodeData));
	check(conn->ldata->ndata != NULL, "failed to resize conn");
	
	for (i = 0; i < arr_size; i++) {
	// if id of choosed rmv isn't "hit", carry on...	
		rmv = i < id ? 0 : 1;
		if (rmv == 0) {
			NodeData *from = &temp_buffer[i];
			check(from != NULL, "pointerfailure");	
			NodeData *to = &conn->ldata->ndata[i];
			check(to != NULL, "pointerfailure");	
			NodeData_copy(to, from, i);
		}
	//...else copy one node ahead from now on...	
		else {
			jump = i + 1;
			NodeData *from = &temp_buffer[jump];
			check(from != NULL, "pointerfailure");	
			NodeData *to = &conn->ldata->ndata[i];
			check(to != NULL, "pointerfailure");	
			NodeData_copy(to, from, i);
		}
	}
	//...and reduce arr_size ignoring the last "offsetnode". 
	conn->ldata->end--;
	return 0;
error:
	return 1;
}

char node_array_swap(Connection * conn) {
//	swapping nodes.
	unsigned short arr_size = conn->ldata->end;
	unsigned short n1 = 0, n2 = 0, i = 0;

	run_info(INFO_FORM, INFO_SWAP_NODES);
	
	printf("swap>\t");
	n1 = std_numnwrap(0, arr_size - 2,
		"\t\033[33mid doesn't exist\033[0m\nswap>\t");
	printf("with>\t");
	n2 = std_numnwrap(0, arr_size - 2,
		"\t\033[33mid doesn't exist\033[0m\nwith>\t");

	NodeData *origin = conn->ldata->ndata;
	NodeData *temp_buffer = calloc(arr_size, sizeof(NodeData));
	// copy old data while inserting "swapnodes" in right order.
	for (i = 0; i < arr_size; i++) {
		NodeData *from = &origin[i];
		NodeData *to = &temp_buffer[i];
		if (i == n1) {
			from = &origin[n2];
			NodeData_copy(to, from, i);			
		} else if (i == n2) {
			from = &origin[n1];		
			NodeData_copy(to, from, i);
		} else {
			NodeData_copy(to, from, i);
		}
	}
	// kill old, rebuild.	
	prev_kill(conn);
	conn->ldata->ndata = calloc(arr_size, sizeof(NodeData));
	check(conn->ldata->ndata != NULL, "failed alloc array");
	// copy modified array in temp "back".		
	for (i = 0; i < arr_size; i++) {
		NodeData *from = &temp_buffer[i];
		NodeData *to = &conn->ldata->ndata[i];
		NodeData_copy(to, from, i);
	}
	free(temp_buffer);
	temp_buffer = NULL;

	return 0;
error:
	if (temp_buffer)
		free(temp_buffer);
	temp_buffer = NULL;
	return 1;
}

char node_array_set_complete(Connection * conn) {
//	flags given node as complete.
	unsigned short arr_size = conn->ldata->end;
	unsigned short id = 0, i = 0;
	
	run_info(INFO_FORM, INFO_COMPLETE);
	
	printf("id>\t");
	id = std_numnwrap(0, arr_size - 2,
		"\t\033[33mid doesn't exist\033[0m\nswap>\t");

	NodeData *node_array = conn->ldata->ndata;

	for (i = 0; i < arr_size; i++) {
		NodeData *cur = &node_array[i];
		check(cur != NULL, "pointerfailure");			
		if (i == id) {
			cur->filed = 1;
		}
	}	
	conn->ldata->cmp++;
	return 0;
error:
	return 1;
}

char DataBase_print(Connection *conn) {
//	pointing to a given print/"display"-func by enum.
	void (*seek_arr_ptr[])(Connection *conn) = { seek_all, seek_lname, seek_topic, seek_title };
	LISelect select = (int)std_numnwrap(READ_ALL + 1, LEND, WARN_INPUT) - 1;
	
	if (select == LBACK)
		return -1;
	seek_arr_ptr[select](conn);
	return 0;
}

void seek_all(Connection *conn) {

	unsigned short i = 0;
	
	NodeData *ndata = conn->ldata->ndata;	
	
	REFRESH();
	run_info(SHOW_FORM, INFO_SHOW_ALL, "database.dat");

	if (!conn->archive) {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (!ndata[i].filed) {
				node_print(ndata+i);
		 		conn->hits++;	
			}
		}
	} else if (conn->archive) {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (ndata[i].filed) {
				node_print(ndata+i);
		 		conn->hits++;	
			}
		}			
	}
	if(!conn->hits)
		run_info(WARN_FORM, WARN_SEEKN);
}
	
void seek_lname(Connection *conn) {

	char seek[SLEN];
	unsigned short i = 0;
	run_info(INFO_FORM, INFO_SEEK_LNAME);
	NodeData * ndata = conn->ldata->ndata;
	
	printf("lname>\t");
	std_strwrap(seek);
	
	REFRESH();
	run_info(SHOW_FORM, INFO_SHOW_LNAME, seek);
	
	if (!conn->archive) {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].lname, 4) == 0 && !ndata[i].filed) {
				node_print(ndata+i);
		 		conn->hits++;	
			}
		}
	} else {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].lname, 4) == 0 && ndata[i].filed) {
				node_print(ndata+i);
			}
		}			
	}
	if(!conn->hits)
		run_info(WARN_FORM, WARN_SEEKN);
}

void seek_topic(Connection *conn) {

	char seek[SLEN];
	unsigned short i = 0;
	run_info(INFO_FORM, INFO_SEEK_LNAME);
	NodeData * ndata = conn->ldata->ndata;
	
	printf("topic>\t");
	std_strwrap(seek);

	REFRESH();
	run_info(SHOW_FORM, INFO_SHOW_TOPIC, seek);
	if (!conn->archive) {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].topic, 4) == 0 && !ndata[i].filed) {
				node_print(ndata+i);
		 		conn->hits++;	
			}
		}
	} else {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].topic, 4) == 0 && ndata[i].filed) {
				node_print(ndata+i);
			}
		}			
	}
	if(!conn->hits)
		run_info(WARN_FORM, WARN_SEEKN);
}

void seek_title(Connection *conn) {
	
	char seek[SLEN];
	unsigned short i = 0;
	run_info(INFO_FORM, INFO_SEEK_LNAME);
	NodeData * ndata = conn->ldata->ndata;
	
	printf("title>\t");
	std_strwrap(seek);
	
	REFRESH();
	run_info(SHOW_FORM, INFO_SHOW_TITLE, seek);
	
	if (!conn->archive) {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].title, 4) == 0 && !ndata[i].filed) {
				node_print(ndata+i);
		 		conn->hits++;	
			}
		}
	} else {
		for (i = 0; i < conn->ldata->end - 1; i++) {
			if (strncmp(seek, ndata[i].title, 4) == 0 && ndata[i].filed) {
				node_print(ndata+i);
			}
		}			
	}
	if(!conn->hits)
		run_info(WARN_FORM, WARN_SEEKN);
}

void title() {
//	pure art.
	short i = 0, j = 0;
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 25; j++) {
			printf("%c", PURE_ART[i][j]);
		}
		printf("\n");
	}
}
