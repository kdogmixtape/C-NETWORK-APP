#ifndef GAME_H
#define GAME_H

#include "defs.h"
#include "structs.h"
#include <assert.h>
#include <stdint.h>

#define P1 0
#define P2 1
#define bool int
#define TRUE 1
#define FALSE 0
#define NUM_SHIPS 5 // 1a, 2a, 2b, 3a, 3b, 4

struct {
  uint64_t p1_board;
  uint64_t p1_shot_board;
  uint64_t p1_hit_board; // might need this to track hits specifically
  uint64_t p1_ships[NUM_SHIPS];

  uint64_t p2_board;
  uint64_t p2_shot_board;
  uint64_t p2_hit_board; 
  uint64_t p2_ships[NUM_SHIPS];

  int8_t winner; // default to -1

  int player_turn; // default to 0 for player 1
  int players[2]; // tracks player ids for quick lookup in clients

  // other stuff we talked about here
} typedef game_data;

/**
 * Below are some of the opcodes for our game
 * protocol (subject to change). Server and client
 * will send messages back and forth with information
 * of these types
 */

enum GAME_MSG {
  // client -> server (put me in a game)
  GAME_MSG_READY,
  // in case of disconnect or refresh (includes all game data needed client-side)
  GAME_MSG_SYNC,
  // client -> server (my board is ready, validate it)
  GAME_MSG_BOARD_SETUP,
  // server -> client (all)
  GAME_MSG_TURN,
  // client -> server (x, y)
  GAME_MSG_SHOT,
  GAME_MSG_RESIGN,
  // general error
  GAME_MSG_ERROR,
  GAME_MSG_CLOSE,
  // are we still in active game?
  GAME_MSG_PING,
  // game still active
  GAME_MSG_PONG
};

/**
 * Parses and handles game message according to the type (GAME_MSG enum above)
 *
 * General packet structure:
 *
 * 4 bits - opcode
 * _ bits (defined by opcode) - args
 * ... more args
 * ____ ____ ____
 * opcd arg1 arg2
 *
 * Example from client: Shot packet with coordinates x=2, y=3
 *
 * 4 bits - opcode
 * 4 bits - x coord
 * 4 bits - y coord
 * 1 bit - hit true or false
 *
 * 0101 010 011 0
 *
 * Example response to all clients: hit
 *
 * 4 bits - opcode
 * 4 bits - x coord
 * 4 bits - y coord
 * 1 bit - hit true or false
 *
 * 0101 010 011 1
 *
 */
int handle_game_msg(unsigned char ws_data[126], client *conn);

#endif
