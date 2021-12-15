#include "pipe_networking.h"


int main() {

  while (42) {

    int to_client;
    int from_client;

    from_client = server_handshake( &to_client );

    while (43) {

      char input[1000];

      int read_call = read(from_client, input, sizeof(input));

      int i;
      for (i = 0; i < strlen(input); i++) {
        input[i] = toupper(input[i]);
      }

      write(to_client, input, strlen(input));

    }

  }

}
