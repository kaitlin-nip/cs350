#ifndef CS350SH_H
#define CS350SH_H

#include "cmd_node.h"

int FORK_ERROR = 1;
int EXEC_ERROR = 2;
int FD_ERROR = 3;
int READ_LINE_ERROR = 4;

//function table for internal commands - for future extension

void fg(struct cmd_node* cmds);
void listjobs(struct cmd_node* cmds);

static void (*CMDS[])(struct cmd_node *)={listjobs, fg};

static char *INTERNAL_CMDS[]={"listjobs","fg"};

struct proc_node* processes;


void registerPID(int pid, char* name);

int is_valid_fd(int fd);
int spawn_process(int in, int out, char** args);
void run_external(struct cmd_node* cmds);
void run_external_bg(struct cmd_node* cmds);
//void run_internal_bg(struct cmd_node* cmds);
//void run_internal(struct cmd_node* cmds);
int get_internal_id(struct cmd_node* commands);
int is_empty_command(struct cmd_node* commands);
void run_command(char* line);
void start_loop();
void initialize();
void finalize();

#endif