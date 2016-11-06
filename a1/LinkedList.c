#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "LinkedList.h"

#define DEBUG 0

struct Node* initNode(){
	struct Node* head = malloc(sizeof(struct Node));
	assert(head!=NULL);
	head->next = NULL;
	head->pid = getpid();
	head->name = malloc(1);
	return head;
}

void append(struct Node *head, pid_t pid, char* name){
	struct Node *newNode = malloc(sizeof(struct Node));
	assert(newNode!=NULL);
	
	newNode->next = NULL;
	newNode->pid = pid;
	newNode->name = name;

	struct Node *cur = head;
	while(cur->next != NULL){
		cur = cur->next;
	}
	cur->next = newNode;
}

int delete(struct Node *head, pid_t pid){
	if(DEBUG)
		printf("->delete, pid = %d\n", pid);
	struct Node *cur = head;
	while(cur->next!=NULL && cur->next->pid != pid){
		cur = cur->next;
	}
	if(cur->next == NULL){
		if(DEBUG)
			printf("<-delete\n");
		return 0;
	}

	struct Node *del = cur->next;
	cur->next = del->next;
	free(del->name);
	free(del);
	if(DEBUG)
		printf("<-delete\n");
	return 1;
}

void print_list(struct Node *head){
	if(DEBUG)
		printf("->printList\n");
	struct Node *cur = head->next;

	pid_t pid; int status;
	while(cur!=NULL){
		pid = waitpid(cur->pid, &status, WNOHANG);
		printf("%s with PID %d Status: %s\n", cur->name, cur->pid, (pid? "FINISHED": "RUNNING"));
		int del = cur->pid;	
		cur = cur->next;
		if(pid != 0){
//			printf("pid=%d\n", pid);
			delete(head, del);
		}
		
	}

	if(DEBUG)
		printf("<-printList\n");
	
}

int contains(struct Node *head, pid_t pid){
	struct Node* cur = head->next;
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

void freeList(struct Node* head){
	struct Node* cur;
	while(head){
		cur = head;
		head = head->next;
		free(cur->name);
		free(cur);
	}
}

/*int main(){

	struct Node* head = initNode();
	char* c = malloc(5);
	char* d = malloc(6);
	char* e = malloc(7);
	append(head, 1, c);
	append(head,2,d);
	append(head, 3, e);
	delete(head, 1);
	freeList(head);
		

}*/
