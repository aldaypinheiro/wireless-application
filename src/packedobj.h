packedobjectsContext *initialize_po();

void decode_xml(xmlDocPtr doc, packedobjectsContext *pc, char *pdu);

void old_encode_xml(xmlDocPtr doc, packedobjectsContext *pc, char *pdu);

char *encode_xml(xmlDocPtr doc, packedobjectsContext *pc);