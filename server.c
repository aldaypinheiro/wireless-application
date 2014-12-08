#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <packedobjects/packedobjects.h>

#define XML_SCHEMA "product.xsd"
#define MAX_CHARACTER (1000*1024)
#define HOST_IP "127.0.0.1"
#define HOST_PORT 6969

int main(int argc, char **argv) {
  xmlDocPtr doc = NULL;
  packedobjectsContext *pc = NULL;
  char buffer[MAX_CHARACTER];
  
  int list_s, conn_s;                                
  struct sockaddr_in servaddr;
  ssize_t bytes_received = 0;

  // setup socket
  if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Error creating listening socket.\n");
    exit(EXIT_FAILURE);
  }

  // setup addressing
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(HOST_PORT);

  // setup to accept connections
  if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    fprintf(stderr, "Error calling bind()\n");
    exit(EXIT_FAILURE);
  }
  if ( listen(list_s, 5) < 0 ) {
    fprintf(stderr, "Error calling listen()\n");
    exit(EXIT_FAILURE);
  }
  if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
    fprintf(stderr, "Error calling accept()\n");
    exit(EXIT_FAILURE);
  }
                                                                      
  // receive the pdu
  bytes_received = read(conn_s, buffer, MAX_CHARACTER);

  // initialise packedobjects
  if ((pc = init_packedobjects(XML_SCHEMA, bytes_received, 0)) == NULL) {
    printf("failed to initialise libpackedobjects");
    exit(1);
  }
  
  // decode the PDU into DOM
  doc = packedobjects_decode(pc, buffer);
  if (pc->decode_error) {
    printf("Failed to decode with error %d.\n", pc->decode_error);
    exit(1);
  }
  
  // output the received DOM
  packedobjects_dump_doc(doc);

  // free the DOM
  xmlFreeDoc(doc);
  
  // free memory created by packedobjects
  free_packedobjects(pc);
  
  xmlCleanupParser();
  return(0);
}
