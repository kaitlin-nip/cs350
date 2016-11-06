#ifndef PROC_NODE_H
#define PROC_NODE_H

#include <unistd.h>
struct proc_node{
	struct proc_node* next;
	char* name;
	pid_t pid;
	int status;
};
int proc_contains(struct proc_node *head, pid_t pid);
void proc_append(struct proc_node *head, pid_t pid, char* name);
int proc_delete(struct proc_node *head, pid_t pid);
void proc_print_list(struct proc_node *head);
struct proc_node* proc_init();
void proc_free_list(struct proc_node *head);

#endif
