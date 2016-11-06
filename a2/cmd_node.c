#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cmd_node.h"
#include "utils.h"

struct cmd_node* cmd_init(){
	struct cmd_node* head = malloc(sizeof(struct cmd_node));
	assert(head != NULL);
	head->is_bg = 0;
	head->arg_num=0;
	head->args = NULL;
	head-> in = NULL;
	head->out = NULL;
	head->next = NULL;
	return head;
}

void cmd_append(struct cmd_node* head, char** args, int arg_num, int bg, char* in, char* out){
	struct cmd_node* cur = head;

	while(cur->next != NULL){
		cur = cur->next;
	}
	struct cmd_node* new_node = malloc(sizeof(struct cmd_node));
	assert(new_node != NULL);
	new_node->is_bg = bg;
	new_node->args = args;
	new_node->arg_num = arg_num;

	new_node->in = copy_string(in);
	new_node->out = copy_string(out); 

	new_node->next = NULL;
	cur->next = new_node;
}

void cmd_free_list(struct cmd_node* head){
	//printf("->cmd_free_list\n");
	struct cmd_node* cur;

	while(head!=NULL){
		cur = head;
		head = head->next;
		free(cur->in);
		free(cur->out);
		char** ctr = cur->args;
		while(ctr && *ctr){
			free(*ctr);
			ctr++;	
		}
		free(cur->args);
		free(cur);
	}
	//printf("<-cmd_free_list\n");
}

void cmd_print_list(struct cmd_node* head){
	//printf("->cmd_print_list\n");
	struct cmd_node* cur = head->next;
	while(cur != NULL){
		printf("cmd = ");
		char** arg = cur->args;
		while(*arg){
			printf("%s ", *arg);
			arg++;
		}
		printf("\nin=%s\nout=%s\narg_num=%d\nbg=%d\n", cur->in, cur->out, cur->arg_num, cur->is_bg);
		cur = cur->next;
	}
	//printf("<-cmd_print_list\n");
}

/*int main(){
	struct cmd_node* head = init();
	char* arg1[4] = {"1","1","1",NULL};
	cmd_append(head, arg1, 0, 0,"in1", "out1");
	//cmd_print_list(head);

	//char* arg2[5] = {"2","2","2","2",NULL};
	//cmd_append(head, arg2, 0, 0,"in2", "out2");
	//cmd_print_list(head);
	cmd_free_list(head);

	return 0;
}*/