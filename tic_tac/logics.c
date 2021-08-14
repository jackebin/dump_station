#include "headers.h"

// global used for checking draw- and winning conditions.

static int CHECKER[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

// creates and initiates members of a Player structure.

struct Player * player_setup(int id, int turn, int choice, int win, int loss, int draw) {

  struct Player * player = malloc(sizeof(struct Player));
  assert (player != NULL);
  
  player->id = id;
  player->turn = turn;
  player->choice = choice;
  player->win = win;
  player->loss = loss;
  player->draw = draw;
  
  // structure within Player for player unique matrices.
  
  make_markers(player);
  return player;
}

// creates and initiates members of the Matrix structure.

struct Matrix * matrix_setup(void) {
  
  int i, j;
  struct Matrix * matrix = malloc(sizeof(struct Matrix));
  assert (matrix != NULL);
  
  for(i = 0; i < 25; i++){
    for(j = 0; j < 39; j++){
      matrix->empty_matrix[i][j] = empty_matrix[i][j];
    }
  }
  for(i = 0; i < 25; i++){
    for(j = 0; j < 39; j++){
      matrix->session_matrix[i][j] = session_matrix[i][j];
    }
  }
  for(i = 0; i < 25; i++){
    for(j = 0; j < 39; j++){
      matrix->winning_matrix[i][j] = winning_matrix[i][j];
    }
  }
  for(i = 0; i < 8; i++){
    for(j = 0; j < 39; j++){
      matrix->title_matrix[i][j] = title_matrix[i][j];
    }
  }
  return matrix;
}

// initiates members of the Marker structure, itself a member of the Player structure.

void make_markers(struct Player * player) {
  
  player->marker = malloc(sizeof(struct Marker));
  int i, j;
  
  if (player->id == 1) {
    for(i = 0; i < 5; i++){
      for(j = 0; j < 7; j++){
        player->marker->player_game_marker[i][j] = player_mark_one[i][j];
      }
    }
    for(i = 0; i < 8; i++){
      for(j = 0; j < 28; j++){
        player->marker->player_turn_marker[i][j] = player_turn_one[i][j];
      }
    }
    for(i = 0; i < 8; i++){
      for(j = 0; j < 39; j++){
        player->marker->player_win_marker[i][j] = player_win_one[i][j];
      }
    }
  }
  if (player->id == 2) {
    for(i = 0; i < 5; i++){
      for(j = 0; j < 7; j++){
        player->marker->player_game_marker[i][j] = player_mark_two[i][j];
      }  
    }
    for(i = 0; i < 8; i++){
      for(j = 0; j < 28; j++){
        player->marker->player_turn_marker[i][j] = player_turn_two[i][j];
      }
    }
    for(i = 0; i < 8; i++){
      for(j = 0; j < 39; j++){
        player->marker->player_win_marker[i][j] = player_win_two[i][j];
      }
    }
  }
}

// writing markers to the session matrix during gameplay.

void mark_squares(struct Player * player, struct Matrix * matrix) {

  int i, j, x, y;
  if (player->choice == 0) { 
    for(i=2, x=0; i<7; i++, x++) {
      for(j=2, y=0; j<8; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 1) { 
    for(i=2, x=0; i<7; i++, x++) {
      for(j=11, y=0; j<17; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 2) { 
    for(i=2, x=0; i<7; i++, x++) {
      for(j=20, y=0; j<26; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 3) { 
    for(i=10, x=0; i<15; i++, x++) {
      for(j=2, y=0; j<8; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 4) { 
    for(i=10, x=0; i<15; i++, x++) {
      for(j=11, y=0; j<17; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 5) { 
    for(i=10, x=0; i<15; i++, x++) {
      for(j=20, y=0; j<26; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 6) { 
    for(i=18, x=0; i<23; i++, x++) {
      for(j=2, y=0; j<8; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 7) { 
    for(i=18, x=0; i<23; i++, x++) {
      for(j=11, y=0; j<17; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
  if (player->choice == 8) { 
    for(i=18, x=0; i<23; i++, x++) {
      for(j=20, y=0; j<26; j++, y++) {
        matrix->session_matrix[i][j] = player->marker->player_game_marker[x][y];
      }
    }
  }
}

// "crash func" stolen from Zed A. Shaw's book "Learn C the hard way".

void panic(const char * message, struct Player * player_one, struct Player * player_two, struct Matrix * matrix) {

  if (errno) {
    perror(message);
  } else {
      printf("ERROR: %s", message);
  }
  
  free(matrix);
  free(player_one);
  free(player_two);
  free(player_one->marker);
  free(player_two->marker);
  
  exit(1);
}

void destroy(struct Player * player_one, struct Player * player_two, struct Matrix * matrix) {

  free(matrix);
  free(player_one);
  free(player_two);
  free(player_one->marker);
  free(player_two->marker);

}

/*

buggy lol-solutions for "safe"
user inputs in the upcoming funcs:

num_input, make_sum, set__marker

the core idea is to let the user go medieval on the
keyboard without crashing the program but yes, I know..

*/

int num_input(void) {

  char temp[100];
  int c, n = 0;
  while((c = getchar()) != '\n')
    temp[n++] = c;
  temp[n] = '\0';
  c = temp[0];
  if (isdigit(c)) {
    c = make_num(c);
    return c;
  } else {
      return num_input();
  }
}

int make_num(char ch) {

  int i = 0;
  int num;
  char ch_num[] = "123456789";
  for (int i = 0; i < 9; i++) {
    if (ch_num[i] == ch) {
      return i;
    } 
  }
}

void set_marker(struct Player * player, struct Matrix * matrix, int SESS) {

  load_matrix(matrix, 1);
  load_matrix(matrix, 3);
  printf("\t SESSION %d\n", SESS);
  load_turn_marker(player);
  
  printf("\n\t  PLAYER %d!\n       Pick a square:\n\t     ", player->id);
  
  while (1) {
    player->choice = num_input();
    if (player->choice < 1 || player->choice > 9) {
      printf("\tType a digit!\n\t     ");
    } else if (CHECKER[player->choice] != -1) {
        printf("   Square %d already taken!\n\t     ", player->choice+1);
    }
    if (CHECKER[player->choice] == -1) {
      //printf(" "); 
      CHECKER[player->choice] = player->id;
      mark_squares(player, matrix);
      break;
    }
  }
}

void update_stats(struct Player * winner, struct Player * looser) {
  
  winner->win += 1;
  looser->loss += 1;
}

// render matrices.

void load_matrix(struct Matrix * matrix, int which) {

  int i, j;
  
  if (which == 1) { 
    for(i = 0; i < 8; i++){
      for(j = 0; j < 39; j++){
        printf("%c", matrix->title_matrix[i][j]);
      }
      printf("\n");
    }
  }
  if (which == 2) { 
    for(i = 0; i < 25; i++){
      for(j = 0; j < 39; j++){
        printf("%c", matrix->empty_matrix[i][j]);
      }
      printf("\n");
    }
  }
  if (which == 3) { 
    for(i = 0; i < 25; i++){
      for(j = 0; j < 39; j++){
        printf("%c", matrix->session_matrix[i][j]);
      }
      printf("\n");
    }
  }
  if (which == 4){
    for(i = 0; i < 25; i++){
      for(j = 0; j < 39; j++){
        printf("%c", matrix->winning_matrix[i][j]);
      }
      printf("\n");
    }
  }  
}

// render turn-matrices mapped to current player.

void load_turn_marker(struct Player * player) {

  int i, j;
  for(i=0; i<7; i++) {
    for(j=0; j<28; j++) {
      printf("%c", player->marker->player_turn_marker[i][j]);
    }
    printf("\n");
  }
}

// render win-matrices mapped to current winner.

void load_win_marker(struct Player * player) {
  
  int i, j;
  for(i = 0; i < 8; i++) {
    for(j = 0; j < 39; j++){
      printf("%c", player->marker->player_win_marker[i][j]);
    }
    printf("\n");
  }
}

// session.

void load_session(struct Player * player_one, struct Player * player_two, struct Matrix * matrix, int turn, int sess) {
  
  int SESS = sess;
  int TURN = turn;
  int DRAW = 0;
  int * p_TURN = &TURN;
  int * p_DRAW = &DRAW;
  int * p_SESS = &SESS;
  
  while (1)
  { 
    if (player_one->turn == *p_TURN) {
      system("clear");
      set_marker(player_one, matrix, *p_SESS);
      if (!set_marker) {
        panic("Something went wrong while setting marker\n",
              player_one, player_two, matrix);
      } if (is_winning(player_one)) {
          player_one->choice = 20;
          *p_TURN = 2;
          break;
      } if (is_draw()) {
          *p_DRAW = 1;
          break;
      }
      *p_TURN = 2;
    }
    if (player_two->turn == *p_TURN) {
      system("clear");
      set_marker(player_two, matrix, *p_SESS);
      if (!set_marker) {
        panic("Something went wrong while setting marker\n",
              player_one, player_two, matrix);
      } if (is_winning(player_two)) {
          player_two->choice = 20;
          *p_TURN = 1;
          break;
      } if (is_draw()) {
          *p_DRAW = 1;
          break;
      }
    }
    *p_TURN = 1;
  }
  
  system("clear");
  
  sess++;
  
  load_fallout(player_one, player_two, matrix, DRAW);
  if (!load_fallout)
    panic("Something went wrong while loading fallout\n",
           player_one, player_two, matrix);
           
  if (is_rematch()) { 
    clear_session(matrix); 
    load_session(player_one, player_two, matrix, TURN, sess);
  }
  
  if(!is_rematch || !clear_session || !load_session)
    panic("Something went wrong while loading session\n",
    player_one, player_two, matrix);
}

// summarizes the session.

void load_fallout(struct Player * player_one, struct Player * player_two, struct Matrix * matrix, int DRAW) {

  load_matrix(matrix, 1);

  if (DRAW == 1) {
    player_one->draw += 1;
    player_two->draw += 1; 
    load_matrix(matrix, 3);
    load_stats(player_one, player_two);
    
    printf("\n\t    IT's A TIE!\n\t"
           "      Rematch?\n");
  }
  
  if (player_one->choice == 20) {
    update_stats(player_one, player_two);
    load_matrix(matrix, 4);
    load_win_marker(player_one);
    load_stats(player_one, player_two);
    
    printf("\n\t..and you SUCK, PLAYER %d.\n\t"
           "     Rematch[y/n]?!\n\t\t   ", 
           player_two->id);
  }
  
  if (player_two->choice == 20) {
    update_stats(player_two, player_one);
    load_matrix(matrix, 4);
    load_win_marker(player_two);
    load_stats(player_one, player_two);
    
    printf("\n\t..and you SUCK, PLAYER %d.\n\t"
           "     Rematch[y/n]?!\n\t\t   ", 
           player_one->id);
  }
}

// printing stats.

void load_stats(struct Player * player_one, struct Player * player_two) {
 
  printf("\n\t   CURRENT SCOREBOARD\n\n");
  printf("Player   \t Wins \t Draws \t Losses\n");
  
  printf("PLAYER %d \t %d \t %d \t %d\n",
          player_one->id, player_one->win,
          player_one->draw, player_one->loss);
          
  printf("PLAYER %d \t %d \t %d \t %d\n",
          player_two->id, player_two->win,
          player_two->draw, player_two->loss);
}

// removes marks from current session, restores CHECKER values. 

void clear_session(struct Matrix * matrix) {

  int i, j, x, y;
  int mirror[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
  for (i=0; i<9; i++) {
    CHECKER[i] = mirror[i];
  }
  for(i=0, x=0; i<25; i++, x++) {
    for(j=0, y=0; j<39; j++, y++) {
      matrix->session_matrix[i][j] = matrix->empty_matrix[x][y];
    }
  }
}

// draw check.

_Bool is_draw(void) {

  int marked = 0;
  for (int i=0; i<9; i++) {
    if (CHECKER[i] != -1) {
      marked++;
    }
  }
  if (marked == 9) {
    return 1;
  }
  else {
    return 0;
  }
}

// rematch check.

_Bool is_rematch(void) {
  char answ;
  scanf("%c", &answ);
  return answ == 'y' || answ == 'Y';
}

// win check

_Bool is_winning(struct Player * player) {
    
  return (player->id == CHECKER[0] && player->id == CHECKER[4] && player->id == CHECKER[8]) ||
         (player->id == CHECKER[2] && player->id == CHECKER[4] && player->id == CHECKER[6]) ||
         
         (player->id == CHECKER[0] && player->id == CHECKER[1] && player->id == CHECKER[2]) ||
         (player->id == CHECKER[3] && player->id == CHECKER[4] && player->id == CHECKER[5]) ||
         (player->id == CHECKER[6] && player->id == CHECKER[7] && player->id == CHECKER[8]) ||
         
         (player->id == CHECKER[0] && player->id == CHECKER[3] && player->id == CHECKER[6]) ||
         (player->id == CHECKER[1] && player->id == CHECKER[4] && player->id == CHECKER[7]) ||
         (player->id == CHECKER[2] && player->id == CHECKER[5] && player->id == CHECKER[8]);
}
