#include <stdlib.h>
#include <string.h>
char* copy_string(char* source){
	
	if(source == NULL){
		return NULL;
	}
	return strdup(source);
	
}