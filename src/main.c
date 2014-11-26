#include <stdio.h>
#include <time.h>
#include <xlocale.h>
#include <string.h>
#include <errno.h>
#include <packedobjects/packedobjects.h>

#include "functions.h"
#include "parses.h"
#include "packedobj.h"

#include "user.h"

#define XML_SCHEMA "product.xsd"
#define MAX_CHARACTER 256

int main(int argc, char **argv) {
	char buffer[MAX_CHARACTER];
	xmlDocPtr doc = NULL;
	packedobjectsContext *pc = initialize_po();
	char *pdu = NULL;
	
	// create the data
	doc = xmlNewDoc(BAD_CAST "1.0");

	add_xml_data(doc, buffer);

	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

	pdu = encode_xml(doc, pc); //it is not altering memory value
	
	decode_xml(doc, pc, pdu);

	// free memory created by packedobjects
	free_packedobjects(pc);

	xmlCleanupParser();
	return (0);
}