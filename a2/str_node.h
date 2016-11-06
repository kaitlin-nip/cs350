#ifndef STR_NODE_H
#define STR_NODE_H

struct str_node{
	char* string;
	struct str_node* next;
	int size;
};

struct str_node* str_init();
void str_append(struct str_node* head, char* s);
char** str_to_arr(struct str_node* head);
void str_free_list(struct str_node* head);
struct str_node* str_make_node(char* s);
void str_print_list(struct str_node* head);

#endif