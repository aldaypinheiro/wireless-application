#include <packedobjects/packedobjects.h>

#define XML_SCHEMA "product.xsd"


char *encode_xml(xmlDocPtr doc, packedobjectsContext *pc) {
	char *encode = NULL;

	////////////////////// Encoding //////////////////////
	// encode the XML DOM
	encode = packedobjects_encode(pc, doc);
	if (pc->bytes == -1) {
		printf("Failed to encode with error %d.\n", pc->encode_error);
	}

	// free the DOM
	xmlFreeDoc(doc);

	return encode;
}

void old_encode_xml(xmlDocPtr doc, packedobjectsContext *pc, char *pdu) {
	////////////////////// Encoding //////////////////////
	// encode the XML DOM
	pdu = packedobjects_encode(pc, doc);// i need to change the real value of pdu
	if (pc->bytes == -1) {
		printf("Failed to encode with error %d.\n", pc->encode_error);
	}

	// free the DOM
	xmlFreeDoc(doc);
}

void decode_xml(xmlDocPtr doc, packedobjectsContext *pc, char *pdu) {
	////////////////////// Decoding //////////////////////
	// decode the PDU into DOM
	doc = packedobjects_decode(pc, pdu);
	if (pc->decode_error) {
		printf("Failed to decode with error %d.\n", pc->decode_error);
		exit(1);
	}

	// output the DOM for checking
	packedobjects_dump_doc(doc);
}

packedobjectsContext *initialize_po() {
	packedobjectsContext *context = NULL;

	// initialise packedobjects
	if ((context = init_packedobjects(XML_SCHEMA, 0, 0)) == NULL) {
		printf("failed to initialise libpackedobjects");
		exit(1);
	}

	return context;
}