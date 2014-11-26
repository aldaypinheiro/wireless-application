#include <stdio.h>
#include <time.h>
#include <xlocale.h>
#include <string.h>
#include <errno.h>
#include <packedobjects/packedobjects.h>

#define XML_SCHEMA "product.xsd"
#define MAX_CHARACTER 256

struct tm *get_time() {
	time_t now = time (NULL);
	return localtime(&now);
}

char *get_timestamp(char *buffer) {
	strftime(buffer, MAX_CHARACTER, "%F %T%z", get_time());
	return buffer;
}

char *user_input(char *buffer, char *node_name) {
	printf("Enter the %s:\n", node_name);
	if (fgets(buffer, MAX_CHARACTER, stdin) == NULL) {
		fprintf(stderr, "failed to read string\n");
		return NULL;
	}
	strtok(buffer, "\n");
	return buffer;
}

int parse_int(char *buffer) {
	int r = 0;
	char *end = NULL;
	errno = 0;

	r = strtoul(buffer, &end, 10);

	if (errno != 0) {
		fprintf(stderr, "Conversion error, %s\n", strerror(errno));
		exit (EXIT_FAILURE); 
	} else if (*end) {
		printf("Warning: converted partially: %i, non-convertible part: %s\n", r, end);
	}

	return r;
}

char *parse_char(char *buffer, int value) {
	sprintf(buffer, "%d", value);

	if (errno != 0) {
		fprintf(stderr, "Conversion error, %s\n", strerror(errno));
		exit (EXIT_FAILURE); 
	}

	return buffer;
}

void add_xml_data(xmlDocPtr doc, char *buffer) {
	int i, j, pid;
	xmlNodePtr root_node = NULL, products = NULL, node = NULL;

	root_node = xmlNewNode(NULL, BAD_CAST "basket");

	xmlDocSetRootElement(doc, root_node);

	xmlNewChild(root_node, NULL, BAD_CAST "purchase-timestamp", BAD_CAST get_timestamp(buffer));

	products = xmlNewChild(root_node, NULL, BAD_CAST "products", NULL);
	
	j = parse_int(user_input(buffer, "number of products"));

	for(i = 1; i <= j; i++) {
		node = xmlNewChild(products, NULL, BAD_CAST "product", NULL);

		user_input(buffer, "product id");
		pid = parse_int(buffer);
		buffer = parse_char(buffer, pid);

		xmlNewChild(node, NULL, BAD_CAST "id", BAD_CAST buffer);
	
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST user_input(buffer, "product name"));
		
		xmlNewChild(node, NULL, BAD_CAST "department", BAD_CAST user_input(buffer, "product department"));
	
		xmlNewChild(node, NULL, BAD_CAST "price", BAD_CAST user_input(buffer, "product price"));
		
		xmlNewChild(node, NULL, BAD_CAST "weight", BAD_CAST user_input(buffer, "product weight"));
		
		xmlNewChild(node, NULL, BAD_CAST "fragile", BAD_CAST user_input(buffer, "fragile type (true or false)"));
	}
}

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

	xmlFreeDoc(doc);
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