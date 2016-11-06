#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "str_node.h"
#include "utils.h"

struct str_node* str_init(){
	struct str_node* head = malloc(sizeof(struct str_node));
	assert(head != NULL);
	head->string = strdup("dummy");
	head->next = NULL;
	head->size = 1;
	return head;
}
struct str_node* str_make_node(char* s){
	struct str_node* new_node = malloc(sizeof(struct str_node));
	assert(new_node != NULL);
	new_node->string = copy_string(s);
	new_node->next = NULL;
	return new_node;
}

void str_append(struct str_node* head, char* s){
	struct str_node* cur = head;
	while(cur->next != NULL){
		cur = cur->next;
	}
	cur->next = str_make_node(s);
	free(s);
	head->size+=1;
}

char** str_to_arr(struct str_node* head){
	char** arr = malloc(sizeof(char*) * head->size); 
	assert(arr != NULL);
	struct str_node* cur = head->next;
	int ctr = 0;
	while(cur != NULL){
		
		arr[ctr] = copy_string(cur->string);
		ctr++;
		//printf("%s\n", cur->string);
		cur = cur->next;
	}
	arr[ctr] = NULL;
	return arr;
}
void str_print_list(struct str_node* head){
	struct str_node* cur = head->next;
	while(cur){
		printf("%s ", cur->string);
		cur = cur ->next;
	}
	printf("\n");
}
void str_free_list(struct str_node* head){
	struct str_node* cur;
	while(head){
		cur = head;
		head = head->next;
		if(cur->string != NULL){
			free(cur->string);
		}
		free(cur);
	}
}

/*int main(){
	struct str_node* head = str_init();
	str_append(head, "node1");
	str_append(head, "node2");
	str_append(head, "node3");
	str_print_list(head);
	char** a = str_to_arr(head);
	int i;
	for (i=0; i < 4; i++){
		printf("%s\n", a[i]? a[i]: "null");
		if(a[i]) free(a[i]);
	}
	free(a);
	str_free_list(head);
	return 0;
*/