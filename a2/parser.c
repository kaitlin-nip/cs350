#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "str_node.h"
#include "cmd_node.h"



int I_FLAG;
int O_FLAG;
int BG_FLAG;

struct str_node *str_head;

int isoperator(char c){
	return c == '&' || c == '<' || c == '>';
}
void skip_whitespace(char** cmdptr){
	while(**cmdptr!='\0' && isspace(**cmdptr)){
		*cmdptr = *cmdptr + 1;
	}
}
char* parse_token(char** cmdptr){
	skip_whitespace(cmdptr);
	char* start = *cmdptr;
	int length = 1;
	while(!isspace(**cmdptr) && !isoperator(**cmdptr)){
		*cmdptr = *cmdptr + 1;
		length += 1;
	}
	
	char* token = malloc(sizeof(char)*(length));
	strncpy(token, start, length);
	token[length-1] = '\0';
	return token;
}

void emergency_clean_up(char* in, char* out, struct str_node* head){
	if(in != NULL){
		free(in);
	}
	if(out != NULL){
		free(out);
	}
	str_free_list(head);
}

int parse_command(char** cmdptr, struct cmd_node* cmd_head){
	char* in = NULL;
	char* out = NULL;
	int type;
	skip_whitespace(cmdptr);
	struct str_node* str_head = str_init();
	while(**cmdptr != '\0'){
		if(BG_FLAG != 0){
			emergency_clean_up(in, out, str_head);
			return -1;
		}
		//assert(BG_FLAG==0); //assume & comes last
		switch(**cmdptr){
			case '<':
				if(I_FLAG != 0){
					emergency_clean_up(in, out, str_head);
					return -1;
				}
				//assert(I_FLAG==0); // assume only one input redirect
				I_FLAG =1;
				*cmdptr = *cmdptr + 1;
				in = parse_token(cmdptr);
				skip_whitespace(cmdptr);
				break;
			case '>':
				if(O_FLAG !=0){
					emergency_clean_up(in, out, str_head);
					return -1;
				}
				//assert(O_FLAG==0);
				O_FLAG = 1;
				*cmdptr = *cmdptr + 1;
				out = parse_token(cmdptr);
				skip_whitespace(cmdptr);
				break;
			case '&':
				if(BG_FLAG != 0){
					emergency_clean_up(in, out, str_head);
					return -1;
				}	
				BG_FLAG = 1;
				*cmdptr = *cmdptr + 1;
				skip_whitespace(cmdptr); // since we assume & comes last, there should only be whitespace following
				break;
			default:
				if(O_FLAG != 0){
					emergency_clean_up(in, out, str_head);
					return -1;
				}
				//assert(O_FLAG==0);
				str_append(str_head, parse_token(cmdptr));
				skip_whitespace(cmdptr);
		}
	}
	if(str_head->size > 1){ // there is a dummy node, checking whether it is empty cmd
		char** args = str_to_arr(str_head);
		cmd_append(cmd_head, args, str_head->size-2 , BG_FLAG, in, out); 
		//-2 because 0 is dummy, 1 is command name
	}
	if(in != NULL){
		free(in);
	}
	if(out!=NULL){
		free(out);
	}
	str_free_list(str_head);
	return 0;
}

struct cmd_node* parse_line(char** line){
	//printf("->parse_line\n");
	I_FLAG = 0; O_FLAG = 0; BG_FLAG = 0;

	struct cmd_node* cmd_head = cmd_init();
	char *cmd = *line;

	while(cmd!=NULL){
        strsep(line, "|");
		if(parse_command(&cmd, cmd_head) != 0){
			printf("bad command\n");
			cmd_free_list(cmd_head);
			return cmd_init();
		}
		cmd = *line;
	}
	cmd_head->is_bg = BG_FLAG;
		//printf("<-parse_line\n" );
	return cmd_head;
}
