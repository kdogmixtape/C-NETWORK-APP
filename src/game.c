#include "game.h"

/**
 * helper function for parsing a board string from client into
 * integers for our server-side validation
 *
 * incoming string should be 64 bytes long (8x8 board), with
 * the only valid characters being 0 to 4 (for ship types)
 *
 * Validates ship placement before the start of a game
 *
 * Valid 7x5 Board (for brevity) view - **in memory as char array**:
 * 0 0 0 0 0 0 0
 * 2 1 0 3 3 3 0
 * 2 0 2 0 0 0 0
 * 0 0 2 4 4 4 4
 * 1 0 0 0 0 0 0
 *
 * Board in memory:
 * 0 0 0 0 0 0 0
 * 1 1 0 1 1 1 0
 * 1 0 1 0 0 0 0
 * 0 0 1 1 1 1 1
 * 1 0 0 0 0 0 0
 *
 * Each ship is also an integer storing positions on the grid:
 * Ship 1a:
 * 0 0 0 0 0 0 0
 * 0 1 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 *
 * Ship 2:
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 1 0 0 0 0
 * 0 0 1 0 0 0 0
 * 0 0 0 0 0 0 0
 *
 * Ship 4:
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 1 1 1 1
 * 0 0 0 0 0 0 0
 *
 * This makes it so that when we hit a ship on the board, we can clear the bit
 * we hit in the corresponding ship and check it against 0. if 0, we know we
 * sunk a ship.
 *
 * To validate the board, we can AND them and check against 0 (all combinations)
 * if (ship4 & ship1a != 0) {
 *   Invalid position for ship1a
 * }
 *
 * We'll also need to make sure you cant wrap your ship around the board:
 *
 * Ship 4 (Invalid):
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 0 0 0 0
 * 0 0 0 0 1 1 1
 * 1 0 0 0 0 0 0
 *
 * And that you make longer ships the correct length, like ship 4 should have 4
 * 1's together
 *
 * @param char array from client-side GAME_MSG_BOARD_SETUP message
 * @param uint64_t array with ship locations.
 * precision as the board (64 bits for a 8x8 board)
 */
int parse_validate_board(char *board_str, uint64_t ships[NUM_SHIPS])
{
  // run through string
  // flip bits in corresponding ships
  // go over every ship and:
  // - check for collisions
  // - make sure there's no "wrapping"
  // - make sure lengths are correct
  //
  // build board, fill gameData struct
  return 0;
}

/**
 * Checks each player_board against their opponent's hit_board to
 * see if the game has been won (all ships are sunk)
 *
 * If the game is won, it sets the winner to 0 or 1 and returns TRUE.
 * If the game is not won, returns FALSE and winner remains -1 (default)
 *
 * @param game_data struct at any point in the game
 */
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

int handle_game_msg(unsigned char ws_data[MAX_WS_MSG_SIZE], client *conn)
{
  int opcode = (ws_data[0] & 0xF0) >> 4;

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
