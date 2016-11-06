#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "proc_node.h"

#define DEBUG 0

struct proc_node* proc_init(){
	struct proc_node* head = malloc(sizeof(struct proc_node));
	assert(head!=NULL);
	head->next = NULL;
	head->pid = getpid();
	head->name = malloc(1);
	return head;
}

void proc_append(struct proc_node *head, pid_t pid, char* name){
	struct proc_node *newNode = malloc(sizeof(struct proc_node));
	assert(newNode!=NULL);
	
	newNode->next = NULL;
	newNode->pid = pid;
	newNode->name = name;

	struct proc_node *cur = head;
	while(cur->next != NULL){
		cur = cur->next;
	}
	cur->next = newNode;
}

int proc_delete(struct proc_node *head, pid_t pid){
	if(DEBUG)
		printf("->delete, pid = %d\n", pid);
	struct proc_node *cur = head;
	while(cur->next!=NULL && cur->next->pid != pid){
		cur = cur->next;
	}
	if(cur->next == NULL){
		if(DEBUG)
			printf("<-delete\n");
		return 0;
	}

	struct proc_node *del = cur->next;
	cur->next = del->next;
	free(del->name);
	free(del);
	if(DEBUG)
		printf("<-delete\n");
	return 1;
}

void proc_print_list(struct proc_node *head){
	if(DEBUG)
		printf("->printList\n");
	struct proc_node *cur = head->next;

	pid_t pid; int status;
	while(cur!=NULL){
		pid = waitpid(cur->pid, &status, WNOHANG);
		printf("%s with PID %d Status: %s\n", cur->name, cur->pid, (pid? "FINISHED": "RUNNING"));
		int del = cur->pid;	
		cur = cur->next;
		if(pid != 0){
//			printf("pid=%d\n", pid);
			proc_delete(head, del);
		}
		
	}

	if(DEBUG)
		printf("<-printList\n");
	
}

int proc_contains(struct proc_node *head, pid_t pid){
	struct proc_node* cur = head->next;
	while(cur){
		if(DEBUG){
			printf("curpid=%d, pid = %d\n", cur->pid, pid);
		}
		if(cur->pid == pid){
			return 1;
		}	
		cur=cur->next;
	}
	return 0;

}

void proc_free_list(struct proc_node* head){
	struct proc_node* cur;
	while(head){
		cur = head;
		head = head->next;
		free(cur->name);
		free(cur);
	}
}

/*int main(){
	return 0;	
}*/