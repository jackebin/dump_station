#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

static int CHECKER[9];

struct Marker{
  char player_game_marker[5][7];
  char player_turn_marker[7][28];
  char player_win_marker[8][39];
};

struct Player {
  int  id;
  int  turn;
  int  choice;
  int  win;
  int  loss;
  int  draw;
  struct Marker * marker;
};

struct Matrix {
  char title_matrix[8][39];
  char empty_matrix[25][39];
  char session_matrix[25][39];
  char winning_matrix[25][39];
};

char player_mark_one[5][7];
char player_mark_two[5][7];
char player_turn_one[7][28];
char player_turn_two[7][28];
char player_win_one[8][39]; 
char player_win_two[8][39];

char empty_matrix[25][39];
char session_matrix[25][39];
char winning_matrix[25][39];
char title_matrix[8][39];

int make_num(char ch);
int num_input(void);

struct Matrix * matrix_setup(void);
struct Player * player_setup(int id, int turn, int choice, int win, int loss, int draw);

_Bool is_draw (void);
_Bool is_rematch (void);
_Bool is_winning (struct Player * player);

void load_turn_marker (struct Player * player);
void load_win_marker (struct Player * player);
void load_matrix (struct Matrix * matrix, int which);
void load_stats (struct Player * player_one, struct Player * player_two);
void load_fallout (struct Player * player_one, struct Player * player_two, struct Matrix * matrix, int DRAW);
void load_session (struct Player * player_one, struct Player * player_two, struct Matrix * matrix, int turn, int sess); 

void make_markers (struct Player * player);
void set_marker (struct Player * player, struct Matrix * matrix, int SESS);
void clear_session (struct Matrix * matrix);
void mark_squares (struct Player * player, struct Matrix * matrix);
void update_stats (struct Player * winner, struct Player * looser);
void destroy(struct Player * player_one, struct Player * player_two, struct Matrix * matrix);
void panic (const char * message, struct Player * player1, struct Player * player2, struct Matrix * matrix);
