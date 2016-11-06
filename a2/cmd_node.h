#ifndef CMD_NODE_H
#define CMD_NODE_H
/*
TYPES:
NO_DIRECT = 0
I_DIRECT = 1
O_DIRECT =2
IO_DIRECT = 3
PIPE = 4
*/

struct cmd_node{
	int is_bg;
	int arg_num;
	char** args;
	char* in;
	char* out;
	struct cmd_node* next;
};

struct cmd_node* cmd_init();
void cmd_append(struct cmd_node* head, char** args, int arg_num, int is_bg, char* in, char* out);
//struct cmd_node* cmd_make_node(char** args, int type, int is_bg, char* in, char* out);
void cmd_free_list(struct cmd_node* head);
void cmd_print_list(struct cmd_node* head);
//char** copy_char_arr(char** arg);
#endif