#include <stdio.h>
#include <stdlib.h>
#include <bsd/stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <signal.h>
#include <limits.h>
#include "LinkedList.h"
#include "sig_handler.h"
#define BUF_SIZE 100
#define DEBUG 0

#define START_ARG_NUM 1
size_t MAX_LENGTH = BUF_SIZE;
int MAX_ARG_NUM = 50;
struct Node* processes;
int is_background;
int list_size=0;

void listjobs(){
	if(DEBUG){
		printf("->listjobs\n");
	}
	print_list(processes);
	
	if(DEBUG){
		printf("<-listjobs\n");
	}
}

void registerPID(int pid, char* name){
	char* p_name = malloc(strlen(name)+1);
	strcpy(p_name, name);
	append(processes, pid, p_name);
}

void run_command(char** arguments, int num){
	if(DEBUG){
		printf("-> run_command\n");
		int i;
		for(i=0; i <= num; i++){
			printf("%d %s ", i, arguments[i]==NULL?"null":arguments[i]);
		}
		printf("\n");
	}

	if(num == 1 && strcmp(arguments[0],"listjobs") == 0){
		listjobs();
		return;
	}

	if(num == 2 && strcmp(arguments[0], "fg")==0){
		char* end;
		pid_t child_pid = strtol(arguments[1], &end, 10);
		if(!contains(processes, child_pid)){
			printf("not a background process.\n");
			return;
		}
		int status;
		while(waitpid(child_pid, &status, 0)!=child_pid);
		return;
	}
	
	int pid = fork();
	if(pid < 0){
		printf("fork failed\n");
		exit(1);
	}
	if(pid == 0){
		if(execvp(*arguments, arguments)){
			printf("%s: command not found.\n", arguments[0]);
		}
		exit(2);
	}
	if(pid > 0){
		registerPID(pid, arguments[0]);
		if(!is_background){
			int status;
			while(wait(&status) != pid);
			delete(processes, pid);
			if(DEBUG){
				printf("child has returned\n");
			}
		}
	}

	if(DEBUG)
		printf("<- run_command\n");
}


char* get_token(char* string, int* s_idx){
	if(DEBUG)
		printf("-> get_token\n");
	
	while(isspace(string[*s_idx])){
	 	*s_idx = *s_idx+1;
	}

	if(string[*s_idx]=='\0'){
		return NULL;
	}

	char* start = &(string[*s_idx]);
	int length = 0;

	while(!isspace(string[*s_idx]) && string[*s_idx] != '\0'){

		if(string[*s_idx]=='&'){
			is_background = 1;
			*s_idx = *s_idx + 1;
			continue;
		}
		length++;
		*s_idx = *s_idx + 1;
	}
	char* token = malloc(sizeof(char) * (length+1));
	assert(token!=NULL);
	strncpy(token, start, length);
	token[length]='\0';

	if(DEBUG)
		printf("<- get_token\n");

	return token;
}

int parse_command(char*** arguments, char* string){
	if(DEBUG)
		printf("-> parse_command\n");

	int s_idx = 0;
	int arg_idx = 0;
	list_size = 1;
	char** arg= NULL;
	arg = calloc(list_size, sizeof(char*)); 
	assert(arg!=NULL);	
	while(string[s_idx] != '\0'){
		char* token = get_token(string, &s_idx);

		if(DEBUG){
			printf("token=%s\n",(token==NULL?"is null":token));
		}

		if(token!=NULL && strcmp(token, "&")!=0){
			if(arg_idx >= list_size){
				list_size*=2;
				char** temp=(char**) realloc(arg,  (sizeof(char*)) * list_size);
				assert(temp!=NULL);
				arg = temp;
			}
			if(DEBUG){
				printf("list_size=%d\n",list_size);
			}
			arg[arg_idx++] = token;
		}else{
			if(token!=NULL){
				free(token);
			}
		}
	}
	if(DEBUG){
		printf("list_size=%d, arg_idx=%d\n",list_size, arg_idx);
	}

	if(arg_idx >= list_size){
		list_size++;
		char** temp=(char**) realloc(arg,  (sizeof(char*)) * list_size);
		assert(temp!=NULL);
		arg = temp;
	}
	arg[arg_idx]='\0';		
	if(DEBUG){
		printf("list_size=%d, arg_idx=%d\n",list_size, arg_idx);
	}
	if(DEBUG)
		printf("<- parse_command\n");

	*arguments = arg;
	return arg_idx;
}


void free_args(char** arguments, int arg_num){
	int i;
	for(i=0;i<list_size;i++){
		if(arguments[i]!=NULL){
			free(arguments[i]);
		}
	}
}

void start_loop(){

	//char** arguments; 
	int exit = 0;
	while(!exit){
		char** arguments;
		char* buffer = NULL;
		size_t cap = 0;
		
		printf("cs350sh> ");
		size_t cmd_len = getline(&buffer, &cap, stdin);
		is_background = 0;
		
		int arg_num = parse_command(&arguments, buffer);
		
		if(arg_num == 1 && strcmp(arguments[0], "exit")==0){
			exit=1;	
		}else if(arg_num > 0){
			run_command(arguments, arg_num);
		}
		
		free_args(arguments, arg_num);		
		if(arguments!=NULL){
			free(arguments);
		}
		if(buffer!=NULL){
			free(buffer);
		}
	}
}

void initialize(){
	processes = initNode();
	signal(SIGINT, ctrl_c_handler);
}

int main(){
	initialize();
	start_loop();
	freeList(processes);
	return 0;	
}
