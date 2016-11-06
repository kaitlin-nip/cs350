#include <stdio.h>
#include <stdlib.h>
//#include <bsd/stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include "str_node.h"
#include "cmd_node.h"
#include "proc_node.h"
#include "parser.h"
#include "sig_handler.h"
#include "utils.h"
#include "cs350sh.h"

#define DEBUG 0


struct cmd_node * command_line;

void registerPID(int pid, char* name){
	char* p_name = copy_string(name);
	proc_append(processes, pid, p_name);
}

void fg(struct cmd_node * cmds){
	if(cmds->next->arg_num != 1){
		printf("fg: wrong number of arguments\n");
		return;
	}

	char* pid = cmds->next->args[1];

	char* end;
	pid_t child_pid = strtol(pid, &end, 10);
	if(!proc_contains(processes, child_pid)){
		printf("fg: %s is not a background process.\n", pid);
		return;
	}
	int status;
	while(waitpid(child_pid, &status, 0)!=child_pid){
		//empty body
	}
}

void listjobs(struct cmd_node * cmds){	
	if(cmds->next->arg_num != 0){
		printf("listjobs: needs no arguments\n");
		return;
	}
	if(cmds->next->is_bg == 1){
		int pid = fork();
		if(pid < 0){
			perror("fork failed");
			exit(FORK_ERROR);
		}else if(pid == 0){
			proc_print_list(processes);
			exit(0);
		}
	}else{
		proc_print_list(processes);
	}
}

int is_valid_fd(int fd){
	int valid = !(fcntl(fd, F_GETFL) < 0 && errno == EBADF);
	if(!valid){
		perror("bad file descriptor");
	}
	return valid;
}

int spawn_process(int in, int out, char** args){
	int status;
	int pid = fork();
	if(pid < 0){
		perror("fork failed");
		return FORK_ERROR;
	}else if(pid == 0){

		if (!is_valid_fd(in) || !is_valid_fd(out)){
    		finalize();
    		exit(FD_ERROR);
		}
		
		if(in!=0){
			dup2(in, 0);
			close(in);
		}
		if(out!=1){
			dup2(out, 1);
			close(out);
		}
		execvp(args[0], args);
		perror(*args);
		finalize();
		exit(EXEC_ERROR);
		
	}else{
		registerPID(pid, args[0]);
		while(wait(&status) != pid);
		return 0;
	}

}

void run_external(struct cmd_node* cmds){

	struct cmd_node* cur = cmds->next;
	int in = 0;
	int out =1;

	if(cur->in != NULL){
		in = open(cur->in, O_RDONLY, 0666);
	}
	int fd[2];
	int status;

	while(cur->next){
		pipe(fd);
		int ret = spawn_process(in, fd[1], cur->args);
		
		if(ret == FORK_ERROR){
			close(fd[0]);
			close(fd[1]);
			return;
		}

		close(fd[1]); 
		in = fd[0];
		cur=cur->next;
	}
	
    if(cur->out != NULL){
    	out = open(cur->out, O_WRONLY| O_CREAT | O_TRUNC, 0666);
    }

    int ret = spawn_process(in, out, cur->args);

	if(ret == FORK_ERROR){
		return;
	}
}

void run_external_bg(struct cmd_node* cmds){
	int pid = fork();
	if(pid < 0){
		perror("fork failed");
		exit(FORK_ERROR);
	}else if(pid ==0){
		run_external(cmds);
		cmd_free_list(cmds);
		command_line = NULL;
		finalize();//printf("back from run_external");
		exit(0);
	}else{
		registerPID(pid, cmds->next->args[0]);
	}
}

int get_internal_id(struct cmd_node* commands){
	int size = sizeof(INTERNAL_CMDS) / sizeof(INTERNAL_CMDS[0]);
	int i;
	for(i =0; i < size; i++){
		if(strcmp(commands->next->args[0], INTERNAL_CMDS[i]) == 0){
			return i;
		}
	}
	return -1;
}

int is_empty_command(struct cmd_node* commands){
	//printf(commands->next == NULL? "EMPTY\n":"NOT_EMPTY\n");
	return commands->next==NULL;//the first cmd_node is dummy
} 

void run_command(char* line){
	char *saved = line; // parse_line will ruin the char*
	command_line = parse_line(&saved); 
	free(line);

	if (!is_empty_command(command_line)){
		
		int cmd_id = get_internal_id(command_line);

		if(cmd_id == -1){
			if(command_line->is_bg == 1){
				run_external_bg(command_line);
			}else{
				run_external(command_line);
			}
		}else{
			//internal function table
			CMDS[cmd_id](command_line);
		}
	}
	cmd_free_list(command_line);
	command_line = NULL;
}

void start_loop(){
	int done = 0;
	size_t INIT_SIZE = 0;

	while(!done){
		char* line = NULL;
		printf("cs350sh> ");
		size_t read = getline(&line, &INIT_SIZE, stdin);
		if (read == -1){
			perror("getline failed\n");
			exit(READ_LINE_ERROR);
		}else {
			if(strcmp(line, "exit\n") == 0){
				free(line);
				done = 1;
			}else{
				run_command(line);
		  	}
		}
	}
}

void initialize(){
	processes =proc_init();
	signal(SIGINT, ctrl_c_handler);
}

void finalize(){
	proc_free_list(processes);	
	cmd_free_list(command_line);
	command_line = NULL;
}

int main(){
	initialize();
	start_loop();
	finalize();
	return 0;	
}