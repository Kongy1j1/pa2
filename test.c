#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#define DEVICE "/dev/pa2_character_device"
#define BUFFER_SIZE 1024

int debug = 1, fd = 0;

int device_read(){
	ssize_t status;
	int r_size = 0;
	char* data = (char*)malloc(BUFFER_SIZE * sizeof(char));
	
	printf("Enter the number of bytes you want to read:\n");
	scanf("%d", &r_size);
	
	if (debug){
		printf("Bytes read from the device are: %d\n", r_size);
	}
	memset(data, 0, sizeof(data));
	data[0] = '\0';
	status = read(fd, data, r_size);
	printf("Data read from the device: %s\n", data);
	if(status == -1){
		printf("Failed to read from device\n");
	}
	printf("Read successfully\n");
	free(data);
	return 0;
}

int device_write(){
	ssize_t status = 0;
	int w_size;
	char *data = (char *)malloc(BUFFER_SIZE * sizeof(char));
	
	printf("Enter data you want to write to the device\n");
	scanf("%[^\n]", data); /*enable white space read*/
	w_size = strlen(data);
	if(debug){
		printf("the length of data written is %d\n", w_size);
	}
	status = write(fd, data, w_size);
	if(status == -1){
		printf("written failed\n");
	}
	printf("written success\n");
	free(data);
	return 0;
}

int device_seek(){
	int o = 0;
	int w = 0;
	printf("Enter an offset value: \n");
	scanf("%d",&o);
	if(debug){
		printf("offset selected is %d\n",o);
	}
	printf("Enter a value for whence(third parameter)\n");
	scanf("%d",&w);
	
	switch(w){
		case 1: lseek(fd,o,SEEK_SET);
				return 0;
				break;
		case 2: lseek(fd,o,SEEK_CUR);
				return 0;
				break;
		case 3: lseek(fd,o,SEEK_END);
				return 0;
				break;
		default:
			printf("Invaild input, please try again\n");
	}
	return 0;
}

int main(){
	char c;
	while(c != 'q'){
		printf("Press the following keys:\n\
			r to read from the device;\n\
			w to write in the device;\n\
			s to seek in the device;\n\
			q to quit\n\
			or keep reading or writing by pressing a random key\n\n\
			Please enter:");
		scanf("%s", &c);
		switch(c){
			case 'r': printf("Reading device\n");
					fd = open(DEVICE, O_RDWR);
					device_read();
					break;
			case 'w': printf("Writing in device\n");
					fd = open(DEVICE, O_RDWR);
					device_write();
					break;
			case 's': printf("Seeking in device\n");
					fd = open(DEVICE, O_RDWR);
					device_seek();
					break;
			case 'e': printf("Quit\n");
					break;
		}
	}
	return 0;
}
