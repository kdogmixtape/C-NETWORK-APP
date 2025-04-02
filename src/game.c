#include "game.h"
#include "defs.h"
#include <stdio.h>

void validate_board(uint64_t *ships, game_data *gameData)
{
  // go over every ship and:
  // - check for collisions
  // - make sure there's no "wrapping"
  // - make sure lengths are correct
  //
  // build board, fill gameData struct
}

bool check_game_over(game_data *gameData)
{
  // if player 1 has hit every position on player 2's board
  bool p1_win =
      (gameData->p2_board & gameData->p1_hit_board) == gameData->p2_board;
  // or the opposite
  bool p2_win =
      (gameData->p1_board & gameData->p2_hit_board) == gameData->p1_board;

  // both should never be true at the same time
  assert(!(p2_win && p1_win));

  if (p1_win) {
    gameData->winner = P1;
    return TRUE;
  }

  if (p2_win) {
    gameData->winner = P2;
    return TRUE;
  }

  return FALSE;
}

int parse_game_msg(unsigned char ws_data[126])
{
  int opcode = ws_data[0] & 0xF0 >> 4;

  switch (opcode) {
  case GAME_MSG_BOARD_SETUP:
    printf("Board setup game message received\n");
    break;
    // add more here for other msg types (shot, resign, etc)
  default:
    printf("Unrecognized game msg\n");
    break;
  }
  return opcode;
}
