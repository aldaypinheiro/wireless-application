#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <packedobjects/packedobjects.h>

#define XML_SCHEMA "product.xsd"
#define MAX_CHARACTER 256
#define HOST_IP "127.0.0.1"
#define HOST_PORT 6969

#include "functions.h"
#include "parses.h"
#include "packedobj.h"
#include "user.h"

int main(int argc, char **argv) {
	char buffer[MAX_CHARACTER];
	xmlDocPtr doc = NULL;
	packedobjectsContext *pc = NULL;
	char *pdu = NULL;

	ssize_t bytes_sent;
	int sock;
	struct sockaddr_in servaddr;

	// setup socket
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "Error creating socket.\n");
		exit(EXIT_FAILURE);
	}
	
	// setup addressing
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(HOST_IP);
	servaddr.sin_port        = htons(HOST_PORT); 

	//initializing packedobjects
	pc = initialize_po();

	// create the data
	doc = xmlNewDoc(BAD_CAST "1.0");

	//adding data to the xml
	add_xml_data(doc, buffer);

	//dumping xml to terminal and file
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlSaveFormatFileEnc("product.xml", doc, "UTF-8", 1);

	//encoding xml
	pdu = encode_xml(doc, pc); //it is not altering memory value

	//decode_xml(doc, pc, pdu);

	// make network connection
	if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf(stderr, "Error calling connect()\n");
		exit(EXIT_FAILURE);
	}

	// send the pdu across the network
	bytes_sent = send(sock, pdu, pc->bytes, 0);

	if (bytes_sent != pc->bytes) {
		fprintf(stderr, "Error calling send()\n");
		exit(EXIT_FAILURE);
	}

	if ( close(sock) < 0 ) {
		fprintf(stderr, "Error calling close()\n");
		exit(EXIT_FAILURE);
	}

	// free the DOM
	xmlFreeDoc(doc);

	// free memory created by packedobjects
	free_packedobjects(pc);

	//clean parser
	xmlCleanupParser();
	return (0);
}