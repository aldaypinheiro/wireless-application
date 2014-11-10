#include <stdio.h>
#include <time.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

char *user_input(char *buffer, char *node_name) {
	printf("Enter the %s:\n", node_name);
	if (fgets(buffer, 256, stdin) == NULL) {
		fprintf(stderr, "failed to read string\n");
		return NULL;
	}
	strtok(buffer, "\n");
	return buffer;
}

int main(int argc, char **argv) {
	int i, j;
	char buffer[256];
	time_t rawtime;
	struct tm *info;
	char *end = NULL;
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL, products = NULL, node = NULL;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "basket");
	xmlDocSetRootElement(doc, root_node);

	info = localtime(&rawtime);

	xmlNewChild(root_node, NULL, BAD_CAST "purchase-timestamp", BAD_CAST "2014-10-10 12:12:12+00:00");//strftime(buffer, 80, "%x %X", info));//
	products = xmlNewChild(root_node, NULL, BAD_CAST "products", NULL);
	
	//j = atoi(user_input(buffer, "number of products"));
	j = strtoul(user_input(buffer, "number of products"), &end, 10);
	
	for(i = 1; i <= j; i++) {
		node = xmlNewChild(products, NULL, BAD_CAST "product", NULL);
			
		xmlNewChild(node, NULL, BAD_CAST "id", BAD_CAST user_input(buffer, "product id"));
		
		xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST user_input(buffer, "product name"));
		
		xmlNewChild(node, NULL, BAD_CAST "department", BAD_CAST user_input(buffer, "product department"));
	
		xmlNewChild(node, NULL, BAD_CAST "price", BAD_CAST user_input(buffer, "product price"));
		
		xmlNewChild(node, NULL, BAD_CAST "weight", BAD_CAST user_input(buffer, "product weight"));
		
		xmlNewChild(node, NULL, BAD_CAST "fragile", BAD_CAST user_input(buffer, "fragile type (true or false)"));
	}

	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1); // prints on console
	xmlSaveFormatFileEnc("product.xml", doc, "UTF-8", 1); //save file
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return (0);
}