#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<linux/sched.h>
#include<linux/fs.h>
#include<linux/kernel.h>


#define buf_size 1024
///< The receive buffer from the LKM




int main() {
   int fd;
   printf("User program starting...\n");
   fd = open("/dev/process_list", O_RDONLY);
     char *buffer;
        buffer = (char *)malloc(sizeof(char)*buf_size);
        int bytes =0;
   // Open the device with read/write access
   if (fd < 0) {
      perror("Failed to open the device...");
      return errno;
   }

   printf("Reading from the device...\n");
   do
     {
        bytes = read(fd,buffer,buf_size);
        if(bytes ==0 ){
            printf("Error in reading message");
        }
        printf("%s\n", buffer);
     }while(bytes != -1);
   
    printf("End of the program\n");
    close(fd);
    free(buffer);

    return 0;
}
