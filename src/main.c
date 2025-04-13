/**
 *  Created by: Kevin, Matt, and Isaac
 *  Date: 17 February 2025
 *  Server using IO multiplexing (select)
 *  To compile: gcc -Wall main.c -o main
 *  To run: ./main
 */

#include "game.h"
#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/select.h>

/*
 * Prepares a socket for use within the program.
 * pre: sockfd_ptr is the address to an integer
 * post: sockfd_ptr points to a listening socket
 */
void setup_socket(int *sockfd_ptr);

/*
 * Accept clients on the provided listening fd
 * pre: sockfd is a socket setup using the setup_socket function,
 *      clients is an array of client* to store the new client
 * post: a client connection has been allocated set up
 */
client *accept_conn(int sockfd, client **clients, int *maxi);

/*
 * Closes and deallocates a client struct
 * pre: client is an open connection and filled struct
 * post: client* is freed and socket is closed, removed from io mult set
 */
void close_client(client **clients, int idx, fd_set *set);

/*
 * Processes clients listening on the provided fd
 * pre: sockfd is a socket setup using the setup_socket function
 * post: the program is done processing clients
 */
void process_clients(int sockfd);

int main(int argc, char **argv)
{
  int serverfd = -1;

  setup_socket(&serverfd);
  printf("Server listening for connections: %d\n", serverfd);
  process_clients(serverfd);

  return 0;
}

void setup_socket(int *sockfd_ptr)
{
  struct sockaddr_in serv_addr;

  *sockfd_ptr = socket(AF_INET, SOCK_STREAM, 0);
  if (*sockfd_ptr < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  bzero(&serv_addr, sizeof(serv_addr));

  // for development, so we dont get port/address already in use
  if (setsockopt(*sockfd_ptr, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) < 0) {
    perror("socket option failed");
    exit(EXIT_FAILURE);
  }

  // for development, so we dont get port/address already in use
#ifdef SO_REUSEPORT
  if (setsockopt(*sockfd_ptr, SOL_SOCKET, SO_REUSEPORT, &(int){1},
                 sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEPORT) failed");
    exit(EXIT_FAILURE);
  }
#endif

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  int err = bind(*sockfd_ptr, (SA *)&serv_addr, sizeof(serv_addr));
  if (err < 0) {
    perror("Error binding socket");
    exit(EXIT_FAILURE);
  }

  err = listen(*sockfd_ptr, LISTENQ);
  if (err < 0) {
    perror("Error listening socket");
    exit(EXIT_FAILURE);
  }
}

client *accept_conn(int sockfd, client **clients, int *maxi)
{
  // check param
  if (sockfd < 0) {
    fprintf(stderr, "Error invalid fd at accept_conn: %d\n", sockfd);
    exit(EXIT_FAILURE);
  }

  client *new_client = malloc(sizeof(client));
  bzero(new_client, sizeof(client));
  new_client->cli_addr_len = sizeof(new_client->cli_addr);
  new_client->fd = accept(sockfd, (SA *)&(new_client->cli_addr),
                          &(new_client->cli_addr_len));

  // store ip str
  memcpy(new_client->ip, inet_ntoa(new_client->cli_addr.sin_addr),
         sizeof(new_client->ip));

  int i = 0; // find an unused client to store the socket id
  while (clients[i] != NULL && clients[i]->fd > 0 && i < FD_SETSIZE)
    i++;
  if (i < FD_SETSIZE) {
    clients[i] = new_client;
  }
  else {
    fprintf(stderr, "Too many clients!\n");
    close(new_client->fd);
    free(new_client);
    return NULL;
  }

  if (i > *maxi)
    *maxi = i;
  printf("Connection accepted:\n\tIndex:%d\n\tFile Desc: %d\n", i,
         new_client->fd);

  return new_client;
}

void close_client(client **clients, int idx, fd_set *set)
{
  close(clients[idx]->fd);
  FD_CLR(clients[idx]->fd, set);
  free(clients[idx]);
  clients[idx] = NULL;
}

void process_clients(int sockfd)
{
  // check param
  if (sockfd < 0) {
    fprintf(stderr, "Error invalid fd at process_clients: %d\n", sockfd);
    exit(EXIT_FAILURE);
  }

  int i, maxi, maxfd;
  int nready_http, nready_ws;
  client *clients[FD_SETSIZE];
  fd_set rset, http_allset, ws_allset;
  game_data *games[MAX_GAMES];
  int num_games = 0;

  maxfd = sockfd;
  maxi = -1;

  FD_ZERO(&http_allset);
  FD_ZERO(&ws_allset);
  FD_SET(sockfd, &http_allset);

  for (;;) {
    rset = http_allset;
    for (int i = 0; i <= maxfd; i++) {
      if (FD_ISSET(i, &ws_allset)) {
        FD_SET(i, &rset);
      }
    }

    nready_http = select(maxfd + 1, &rset, NULL, NULL, NULL);

    // handle new http connections
    if (FD_ISSET(sockfd, &rset)) {
      // setup new client
      client *new_client = accept_conn(sockfd, clients, &maxi);

      // if websocket (persistent), add to multiplexing set
      if (new_client != NULL) {

        // for testing, add to a game with self
        games[num_games] = start_new_game(num_games, new_client, new_client);
        num_games++;

        FD_SET(new_client->fd, &http_allset);
        if (new_client->fd > maxfd)
          maxfd = new_client->fd;
        if (--nready_http <= 0)
          continue; // no more readable descriptors
      }
    }

    // read from current connections
    for (i = 0; i <= maxi; i++) {
      if (clients[i] == NULL || clients[i]->fd < 0)
        continue;
      if (FD_ISSET(clients[i]->fd, &rset)) {
        if (FD_ISSET(clients[i]->fd, &http_allset)) { // Connection is HTTP
          int result = parse_http_request(clients[i]);
          if (result != 0) {
            close_client(clients, i, &http_allset);
            continue;
          }

          result = route_request(clients[i]);
          if (result == 1) { // if 1, request was upgraded to WS
            FD_SET(clients[i]->fd, &ws_allset);
            FD_CLR(clients[i]->fd, &http_allset);
            continue;
          }

          // all connections are non-persistent so we can close them after
          // reading
          close_client(clients, i, &http_allset);

          if (--nready_http <= 0)
            break;
        }
        else if (FD_ISSET(clients[i]->fd, &ws_allset)) { // Connection is WS
          // check for end of connection, close socket and deallocate
          ws_frame *frame = malloc(sizeof(ws_frame));
          bzero(frame, sizeof(ws_frame));
          if (receive_ws_data(frame, clients[i]) == OP_CLOSE) {
            close_client(clients, i, &ws_allset);
            clients[i] = NULL;
          }

          else {
            char *message = "Hello from server";
            int game_msg_opcode;
            switch (frame->opcode) {
            case OP_TEXT: // for testing
              send_ws_message(clients[i], message, strlen(message));
              break;
            case OP_BIN: // for game messages
              game_msg_opcode = handle_game_msg(frame->msg, clients[i],
                                                games[clients[i]->game_id]);
              printf("Game msg opcode: %d\n", game_msg_opcode);
              break;
            case OP_PING:
              printf("Received Ping\n");

              unsigned char pong[] = {0x8A, 0x00};
              write(clients[i]->fd, pong, 2);
              break;
            case OP_CLOSE:
              printf("Websocket close request received\n");
              send_ws_close(clients[i]);
              close_client(clients, i, &ws_allset);
              break;
            default:
              break;
            }
          }

          free(frame);
          if (--nready_ws <= 0)
            break;
        }
      }
    }
  }
}
