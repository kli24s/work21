#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  // ==========================================================================

  int mkfifo_call = mkfifo(WKP, 0777);

  if (mkfifo_call == -1) {
    printf("WKP: Not Created | %s\n", strerror(errno));
    return -1;
  }
  else printf("WKP: Created\n");

  // ==========================================================================

  int open_call = open(WKP, O_RDONLY);

  if (open_call == -1) {
    printf("WKP: Not Open | %s\n", strerror(errno));
    return -1;
  }
  else printf("WKP: Open\n");

  // ==========================================================================

  char input[1000];
  read(open_call, input, HANDSHAKE_BUFFER_SIZE);

  printf("Handshake: Received | %s\n", input);

  int from_client = open_call;

  // ==========================================================================

  remove(WKP);
  printf("WKP: Removed\n");

  // ==========================================================================

  close(open_call);

  open_call = open(input, O_WRONLY);

  if (open_call == -1) {
    printf("Secret Pipe: Not Open | %s\n", strerror(errno));
    return -1;
  }
  else printf("Secret Pipe: Open\n");

  write(open_call, ACK, sizeof(ACK));

  *to_client = open_call;

  // ==========================================================================

  char response[1000];
  read(open_call, response, HANDSHAKE_BUFFER_SIZE);

  // ==========================================================================

  close(open_call);

  open_call = open(input, O_RDONLY);

  if (open_call == -1) {
    printf("WKP: Not Open | %s\n", strerror(errno));
    return -1;
  }
  else printf("WKP: Open\n");

  char message[1000];
  read(open_call, message, HANDSHAKE_BUFFER_SIZE);

  printf("Final Message: Received | %s\n", message);

  // ==========================================================================

  return from_client;

}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  char input[1000];
  sprintf(input, "%d", getpid());

  // ==========================================================================

  int mkfifo_call = mkfifo(input, 0777);

  if (mkfifo_call == -1) {
    printf("Secret Pipe: Not Created | %s\n", strerror(errno));
    return -1;
  }
  else printf("Secret Pipe: Created\n");

  // ==========================================================================

  int open_call = open(WKP, O_WRONLY);

  if (open_call == -1) {
    printf("WKP: Not Open | %s\n", strerror(errno));
    return -1;
  }
  else printf("WKP: Opened\n");

  write(open_call, input, HANDSHAKE_BUFFER_SIZE);

  *to_server = open_call;

  printf("Sent to Server\n");

  // ==========================================================================

  close(open_call);

  open_call = open(input, O_RDONLY);

  if (open_call == -1) {
    printf("Secret Pipe: Not Open | %s\n", strerror(errno));
    return -1;
  }
  else printf("Secret Pipe: Open\n");

  char message[1000];

  read(open_call, message, HANDSHAKE_BUFFER_SIZE);

  printf("Message: Received | %s\n", message);

  int from_server = open_call;

  // ==========================================================================

  close(open_call);

  open_call = open(input, O_WRONLY);
  write(open_call, ACK, strlen(ACK));

  printf("ACK: Sent\n");

  close(open_call);

  // ==========================================================================

  remove(input);
  printf("Secret Pipe: Removed\n");

  // ==========================================================================

  return from_server;

}
