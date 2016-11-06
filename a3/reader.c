#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define buffer_length 1

int main(){
	char buffer[buffer_length];
	int bytes_read;
	/* allocate memory for character buffers HERE before you use them */

	int fd = open("/dev/process_list", O_RDONLY);
	if (fd == -1 ){
		perror("can not open device");
		exit(-1);
	}
	
	/* check for errors HERE */
	do{
		bytes_read = read(fd, buffer, buffer_length);
		//printf("bytes_read=%d\n", bytes_read);
		int i;
		for(i=0;i<bytes_read;i++){
			printf("%c", buffer[i]);
		}
	
	}while(bytes_read > 0);
	

	close(fd);
}