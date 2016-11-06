#include <assert.h>
struct Node{
	struct Node* next;
	char* name;
	pid_t pid;
	int status;
};
int contains(struct Node *head, pid_t pid);
void append(struct Node *head, pid_t pid, char* name);
int delete(struct Node *head, pid_t pid);
void print_list(struct Node *head);
struct Node* initNode();
void freeList(struct Node *head);
