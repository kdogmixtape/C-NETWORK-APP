#ifndef GAME_H
#define GAME_H

#include "defs.h"
#include "ws.h"
#include <assert.h>
#include <stdint.h>

#define P1 0
#define P2 1
#define bool int
#define TRUE 1
#define FALSE 0
#define NUM_SHIPS 6 // 1a, 2a, 2b, 3a, 3b, 4
#define MAX_GAMES 20

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
  client *players[2];

  int player_turn; // default to 0 for player 1

  // other stuff we talked about here
} typedef game_data;

/**
 * Below are some of the opcodes for our game
 * protocol (subject to change). Server and client
 * will send messages back and forth with information
 * of these types
 */

enum GAME_MSG_OP {
  // client -> server (put me in a game)
  GAME_MSG_READY,
  // in case of disconnect or refresh (includes all game data needed
  // client-side)
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
} typedef game_msg_op;

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
int handle_game_msg(unsigned char ws_data[MAX_WS_MSG_SIZE], client *conn,
                    game_data *gd);

/**
 *  sends a game message using ws helpers using the params
 *  specified in the game_msg struct. If player_idx is negative,
 *  all players will be notified of the message
 */
int send_game_msg(game_msg_op opcode, char *msg, int len, int player_idx,
                  game_data *gd);

/*
 * quick helper for creating a new game with players'
 * client ids. Id's should be the index of the client
 * struct in the clients list for quick O(1) access
 */
game_data *start_new_game(int idx, client *player1, client *player2);

#endif
