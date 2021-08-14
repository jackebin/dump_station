#include "headers.h"

int main(int argc, char * argv[])
{ 
  system("clear");
  
  struct Matrix * matrix = matrix_setup();
  struct Player * player_one = player_setup(1, 1, -1, 0, 0, 0);
  struct Player * player_two = player_setup(2, 2, -1, 0, 0, 0);
  
  if (!player_one || !player_two || !matrix)
    panic("Something went wrong while setting-up structures\n", 
           player_one, player_two, matrix);

  load_session(player_one, player_two, matrix, 1, 1);
  if(!load_session)
    panic("Something went wrong while loading the game\n",
          player_one, player_two, matrix);
  
  destroy(player_one, player_two, matrix);
  
  return 0;
}
