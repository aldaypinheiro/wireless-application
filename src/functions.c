struct tm *get_time() {
	time_t now = time (NULL);
	return localtime(&now);
}

char *get_timestamp(char *buffer) {
	strftime(buffer, MAX_CHARACTER, "%F %T%z", get_time());
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